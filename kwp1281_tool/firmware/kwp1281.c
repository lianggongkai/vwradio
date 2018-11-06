#include "main.h"
#include "kwp1281.h"
#include "uart.h"
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>


// Return a string description of a result
const char * kwp_describe_result(kwp_result_t result) {
    switch (result) {
        case KWP_SUCCESS:           return "Success";
        case KWP_TIMEOUT:           return "Timeout";
        case KWP_BAD_ECHO:          return "Bad echo received";
        case KWP_BAD_COMPLEMENT:    return "Bad complement received";
        case KWP_BAD_BLK_LENGTH:    return "Bad block length received";
        case KWP_BAD_BLK_END:       return "Bad block end marker byte received";
        case KWP_RX_OVERFLOW:       return "RX buffer overflow";
        case KWP_UNEXPECTED:        return "Unexpected block title received";
        case KWP_MEM_TOO_SHORT:     return "Length of memory returned is shorter than requested";
        case KWP_MEM_TOO_LONG:      return "Length of memory returned is longer than requested";
        default:                    return "???";
    }
}


// If result is success, do nothing.  Otherwise, report the error and halt.
void kwp_panic_if_error(kwp_result_t result)
{
    if (result == KWP_SUCCESS) { return; }

    const char *msg = kwp_describe_result(result);
    uart_flush_tx(UART_DEBUG);
    uart_puts(UART_DEBUG, "\n\n*** KWP PANIC: ");
    uart_puts(UART_DEBUG, (char *)msg);
    uart_puts(UART_DEBUG, "\n");
    while(1);
}


// Send byte only
static kwp_result_t _send_byte(uint8_t tx_byte)
{
    _delay_ms(1);

    // send byte
    uart_blocking_put(UART_KLINE, tx_byte);

    // consume its echo
    uint8_t echo;
    uint8_t uart_ready = uart_blocking_get_with_timeout(UART_KLINE, 3000, &echo);
    if (!uart_ready) { return KWP_TIMEOUT; }
    if (echo != tx_byte) { return KWP_BAD_ECHO; }

    uart_puthex(UART_DEBUG, tx_byte);
    uart_put(UART_DEBUG, ' ');
    return KWP_SUCCESS;
}


// Send byte and receive its complement
static kwp_result_t _send_byte_recv_compl(uint8_t tx_byte)
{
    kwp_result_t result = _send_byte(tx_byte);
    if (result != KWP_SUCCESS) { return result; }

    uint8_t complement;
    uint8_t uart_ready = uart_blocking_get_with_timeout(UART_KLINE, 3000, &complement);
    if (!uart_ready) { return KWP_TIMEOUT; }
    if (complement != (tx_byte ^ 0xff)) { return KWP_BAD_COMPLEMENT; }

    return KWP_SUCCESS;
}


// Receive byte only
static kwp_result_t _recv_byte(uint8_t *rx_byte_out)
{
    uint8_t uart_ready = uart_blocking_get_with_timeout(UART_KLINE, 3000, rx_byte_out);
    if (!uart_ready) { return KWP_TIMEOUT; }

    uart_puthex(UART_DEBUG, *rx_byte_out);
    uart_put(UART_DEBUG, ' ');
    return KWP_SUCCESS;
}


// Receive byte and send its complement
static kwp_result_t _recv_byte_send_compl(uint8_t *rx_byte_out)
{
    kwp_result_t result = _recv_byte(rx_byte_out);
    if (result != KWP_SUCCESS) { return result; }

    _delay_ms(1);

    // send complement byte
    uint8_t complement = *rx_byte_out ^ 0xFF;
    uart_blocking_put(UART_KLINE, complement);

    // consume its echo
    uint8_t echo;
    uint8_t uart_ready = uart_blocking_get_with_timeout(UART_KLINE, 3000, &echo);
    if (!uart_ready) { return KWP_TIMEOUT; }
    if (echo != complement) { return KWP_BAD_ECHO; }

    return KWP_SUCCESS;
}


// Wait for the 0x55 0x01 0x8A sequence during initial connection
static kwp_result_t _wait_for_55_01_8a()
{
    const uint8_t expected_rx_bytes[] = { 0x55, 0x01, 0x8a };
    uint8_t rx_byte;
    uint8_t i = 0;

    while (1) {
        kwp_result_t result = _recv_byte(&rx_byte);
        if (result != KWP_SUCCESS) { return result; }

        if (rx_byte == expected_rx_bytes[i]) {
            if (++i == 3) { return KWP_SUCCESS; }
        } else {
            i = 0;
        }
    }
}


// Send a block
kwp_result_t kwp_send_block(uint8_t *buf)
{
    uint8_t block_length = buf[0];
    uint8_t buf_size = block_length + 1;

    buf[1] = ++kwp_block_counter;   // insert block counter
    buf[buf_size - 1] = 0x03;       // insert block end

    uart_puts(UART_DEBUG, "SEND: ");

    // send each byte and receive its complement, except block end
    kwp_result_t result;
    uint8_t i;
    for (i=0; i<(buf_size-1); i++) {
        result = _send_byte_recv_compl(buf[i]);
        if (result != KWP_SUCCESS) { return result; }
    }

    // send block end, do not receive complement
    result = _send_byte(buf[i]);
    if (result != KWP_SUCCESS) { return result; }

    uart_puts(UART_DEBUG, "\n");
    return KWP_SUCCESS;
}


// Receive a block
kwp_result_t kwp_receive_block()
{
    uart_puts(UART_DEBUG, "RECV: ");

    kwp_rx_size = 0;
    memset(kwp_rx_buf, 0, sizeof(kwp_rx_buf));

    uint8_t bytes_remaining = 1;
    uint8_t c;
    kwp_result_t result;

    while (bytes_remaining) {
        if ((kwp_rx_size == 0) || (bytes_remaining > 1)) {
            result = _recv_byte_send_compl(&c);
            if (result != KWP_SUCCESS) { return result; }
        } else {
            // do not send complement for last byte in block (0x03 block end)
            result = _recv_byte(&c);
            if (result != KWP_SUCCESS) { return result; }
            if (c != 0x03) { return KWP_BAD_BLK_END; }
        }

        kwp_rx_buf[kwp_rx_size++] = c;
        if (kwp_rx_size == sizeof(kwp_rx_buf)) { return KWP_RX_OVERFLOW; }

        switch (kwp_rx_size) {
            case 1:  // block length (must be at least 3 for title, counter, end)
                if (c < 3) { return KWP_BAD_BLK_LENGTH; }
                bytes_remaining = c;
                break;
            case 2:  // block counter
                if (kwp_is_first_block) {   // set initial value
                    kwp_block_counter = c;
                    kwp_is_first_block = false;
                } else {
                    kwp_block_counter++;
                    // we used to detect a mismatch in the block counter here but had
                    // to remove the check.  it worked fine on several radios but not
                    // on the vw rhapsody (technisat), which does not always send the
                    // correct block counter.
                }
                // fall through
            default:
                bytes_remaining--;
        }
    }

    _delay_ms(10);
    uart_puts(UART_DEBUG, "\n");
    return KWP_SUCCESS;
}


// Receive a block; halt unless it has the expected title
kwp_result_t kwp_receive_block_expect(uint8_t title)
{
    kwp_result_t result = kwp_receive_block();
    if (result != KWP_SUCCESS) { return result; }
    if (kwp_rx_buf[2] == title) { return KWP_SUCCESS; }

    uart_flush_tx(UART_DEBUG);
    uart_puts(UART_DEBUG, "\n\nExpected to receive title 0x");
    uart_puthex(UART_DEBUG, title);
    uart_puts(UART_DEBUG, ", got 0x");
    uart_puthex(UART_DEBUG, kwp_rx_buf[2]);
    uart_put(UART_DEBUG, '\n');

    return KWP_UNEXPECTED;
}


kwp_result_t kwp_send_ack_block()
{
    uart_puts(UART_DEBUG, "PERFORM ACK\n");
    uint8_t block[] = {
        0x03,       // block length
        0,          // placeholder for block counter
        KWP_ACK,    // block title
        0,          // placeholder for block end
    };
    return kwp_send_block(block);
}


kwp_result_t kwp_send_login_block(uint16_t safe_code, uint8_t fern, uint16_t workshop)
{
    uart_puts(UART_DEBUG, "PERFORM LOGIN\n");
    uint8_t block[] = {
        0x08,               // block length
        0,                  // placeholder for block counter
        KWP_LOGIN,          // block title
        HIGH(safe_code),    // safe code high byte
        LOW(safe_code),     // safe code low byte
        fern,               // fern byte
        HIGH(workshop),     // workshop code high byte
        LOW(workshop),      // workshop code low byte
        0,                  // placeholder for block end
    };
    return kwp_send_block(block);
}


// login using the safe code and read the secret group 0x19
// should work on any radio if the safe code is correct
kwp_result_t kwp_login_safe(uint16_t safe_code)
{
    kwp_result_t result = kwp_send_login_block(safe_code, 0x01, 0x869f);
    if (result != KWP_SUCCESS) { return result; }

    result = kwp_receive_block_expect(KWP_ACK);
    if (result != KWP_SUCCESS) { return result; }

    result = kwp_send_group_reading_block(0x19);
    if (result != KWP_SUCCESS) { return result; }

    // all radios require reading group 0x19 to unlock the protected commands.
    // some return ack, some lie and return nak (treat it like ack).
    //   ack: premium 4 (clarion), audi chorus (blaupunkt)
    //   nak: premium 5 (delco), gamma 5 (technisat)
    result = kwp_receive_block();
    return result;
}


kwp_result_t kwp_send_group_reading_block(uint8_t group)
{
    uart_puts(UART_DEBUG, "PERFORM GROUP READ\n");
    uint8_t block[] = {
        0x04,               // block length
        0,                  // placeholder for block counter
        KWP_GROUP_READING,  // block title
        group,              // group number
        0,                  // placeholder for block end
    };
    return kwp_send_block(block);
}


int _send_read_mem_block(uint8_t title, uint16_t address, uint8_t length)
{
    uart_puts(UART_DEBUG, "PERFORM READ xx MEMORY\n");
    uint8_t block[] = {
        0x06,           // block length
        0,              // placeholder for block counter
        title,          // block title
        length,         // number of bytes to read
        HIGH(address),  // address high
        LOW(address),   // address low
        0,              // placeholder for block end
    };
    return kwp_send_block(block);
}


static kwp_result_t _read_mem(uint8_t req_title, uint8_t resp_title,
                              uint16_t start_address, uint16_t total_size, uint8_t chunk_size)
{
    uint16_t address = start_address;
    uint16_t remaining = total_size;

    while (remaining != 0) {
        if (remaining < chunk_size) { chunk_size = remaining; }

        kwp_result_t result = _send_read_mem_block(req_title, address, chunk_size);
        if (result != KWP_SUCCESS) { return result; }

        result = kwp_receive_block_expect(resp_title);
        if (result != KWP_SUCCESS) { return result; }

        uint8_t datalen = kwp_rx_buf[0] - 3;  // block length - (counter + title + end)
        if (datalen < chunk_size) { return KWP_MEM_TOO_SHORT; }
        if (datalen > chunk_size) { return KWP_MEM_TOO_LONG; }

        uart_puts(UART_DEBUG, "MEM: ");
        uart_puthex16(UART_DEBUG, address);
        uart_puts(UART_DEBUG, ": ");
        for (uint8_t i=0; i<datalen; i++) {
            uart_puthex(UART_DEBUG, kwp_rx_buf[3 + i]);
            uart_put(UART_DEBUG, ' ');
        }
        uart_puts(UART_DEBUG, "\n");

        address += chunk_size;
        remaining -= chunk_size;

        result = kwp_send_ack_block();
        if (result != KWP_SUCCESS) { return result; }

        kwp_receive_block_expect(KWP_ACK);
        if (result != KWP_SUCCESS) { return result; }
    }
    return KWP_SUCCESS;
}

kwp_result_t kwp_read_ram(uint16_t start_address, uint16_t total_size, uint8_t chunk_size)
{
    return _read_mem(KWP_READ_RAM, KWP_R_READ_RAM,
                     start_address, total_size, chunk_size);
}

kwp_result_t kwp_read_rom_or_eeprom(uint16_t start_address, uint16_t total_size, uint8_t chunk_size)
{
    return _read_mem(KWP_READ_ROM_EEPROM, KWP_R_READ_ROM_EEPROM,
                     start_address, total_size, chunk_size);
}

kwp_result_t kwp_read_eeprom(uint16_t start_address, uint16_t total_size, uint8_t chunk_size)
{
    return _read_mem(KWP_READ_EEPROM, KWP_R_READ_ROM_EEPROM,
                     start_address, total_size, chunk_size);
}

// VW Premium 4 only ========================================================

static kwp_result_t _send_f0_block()
{
    uart_puts(UART_DEBUG, "PERFORM TITLE F0\n");
    uint8_t block[] = {
        0x04,           // block length
        0,              // placeholder for block counter
        KWP_SAFE_CODE,  // block title
        0,              // 0=read
        0,              // placeholder for block end
    };
    return kwp_send_block(block);
}

kwp_result_t kwp_p4_read_safe_code_bcd(uint16_t *safe_code)
{
    kwp_result_t result = _send_f0_block();
    if (result != KWP_SUCCESS) { return result; }

    result = kwp_receive_block_expect(KWP_SAFE_CODE);
    if (result != KWP_SUCCESS) { return result; }

    *safe_code = (kwp_rx_buf[3] << 8) + kwp_rx_buf[4];
    return KWP_SUCCESS;
}

// VW Premium 5 mfg mode (address 0x7c) only ================================

static kwp_result_t _delco_login_mfg()
{
    kwp_result_t result = kwp_send_login_block(0x4f43, 0x4c, 0x4544);  // "OCLED"
    if (result != KWP_SUCCESS) { return result; }
    kwp_panic_if_error(result);
    result = kwp_receive_block_expect(KWP_ACK);
    return result;
}

static kwp_result_t _delco_read_safe_code_bcd(uint16_t eeprom_address, uint16_t *safe_code)
{
    kwp_result_t result = _send_read_mem_block(KWP_READ_ROM_EEPROM, eeprom_address, 0x02);
    if (result != KWP_SUCCESS) { return result; }

    kwp_receive_block_expect(KWP_R_READ_ROM_EEPROM);
    if (result != KWP_SUCCESS) { return result; }

    *safe_code = (kwp_rx_buf[3] << 8) + kwp_rx_buf[4];
    return KWP_SUCCESS;
}

kwp_result_t kwp_p5_login_mfg()
{
    return _delco_login_mfg();
}

kwp_result_t kwp_p5_read_safe_code_bcd(uint16_t *safe_code)
{
    uint16_t eeprom_address = 0x0014;
    return _delco_read_safe_code_bcd(eeprom_address, safe_code);
}

static kwp_result_t _send_calc_rom_checksum_block()
{
    uart_puts(UART_DEBUG, "PERFORM ROM CHECKSUM\n");
    uint8_t block[] = {
        0x05,       // block length
        0,          // placeholder for block counter
        KWP_CUSTOM, // block title
        0x31,       // unknown constant
        0x32,       // subtitle (rom checksum)
        0,          // placeholder for block end
    };
    return kwp_send_block(block);
}

kwp_result_t kwp_p5_calc_rom_checksum(uint16_t *rom_checksum)
{
    kwp_result_t result = _send_calc_rom_checksum_block();
    if (result != KWP_SUCCESS) { return result; }

    result = kwp_receive_block_expect(KWP_CUSTOM);
    if (result != KWP_SUCCESS) { return result; }

    *rom_checksum = (kwp_rx_buf[5] << 8) + kwp_rx_buf[6];
    return KWP_SUCCESS;
}

// Seat Liceo mfg mode (address 0x7c) only ==================================

kwp_result_t kwp_sl_login_mfg()
{
    return _delco_login_mfg();
}

kwp_result_t kwp_sl_read_safe_code_bcd(uint16_t *safe_code)
{
    uint16_t eeprom_address = 0x0028;
    return _delco_read_safe_code_bcd(eeprom_address, safe_code);
}

// ==========================================================================

// Switch to 5 baud, send address byte, then switch back to previous settings
// This is blocking so it will hang for about 2 seconds
void kwp_send_address(uint8_t address)
{
    // TODO: add support for other uart
    if (UART_KLINE != UART1) { while(1); }

    UCSR1B &= ~_BV(RXEN1);  // Disable RX (PD2/TXD1)
    UCSR1B &= ~_BV(TXEN1);  // Disable TX (PD3/TXD1)
    DDRD |= _BV(PD3);       // PD3 = output

    uint8_t parity = 1;
    for (uint8_t i=0; i<10; i++) {
        uint8_t bit;
        switch (i) {
            case 0:     // start bit
                bit = 0;
                break;
            case 8:     // parity bit
                bit = parity;
                break;
            case 9:     // stop bit
                bit = 1;
                break;
            default:    // 7 data bits
                bit = (uint8_t)((address & (1 << (i - 1))) != 0);
                parity ^= bit;
        }

        if (bit == 1) {
            PORTD |= _BV(PD3);  // high
        } else {
            PORTD &= ~_BV(PD3); // low
        }
        _delay_ms(200);         // 1000ms / 5bps = 200ms per bit
    }

    UCSR1B |= _BV(TXEN1);   // Enable TX (PD3/TXD1)
    UCSR1B |= _BV(RXEN1);   // Enable RX (PD2/TXD1)
    DDRD &= ~_BV(PD3);      // PD3 = input
}

kwp_result_t kwp_connect(uint8_t address, uint32_t baud)
{
    uart_puts(UART_DEBUG, "\nCONNECT ");
    uart_puthex(UART_DEBUG, address);
    uart_puts(UART_DEBUG, ": ");

    uart_init(UART_KLINE, baud);

    // Initialize connection state
    kwp_baud_rate = baud;
    kwp_is_first_block = true;
    memset(kwp_vag_number,  0, sizeof(kwp_vag_number));
    memset(kwp_component_1, 0, sizeof(kwp_component_1));
    memset(kwp_component_2, 0, sizeof(kwp_component_2));

    // Send address at 5 baud and perform initial handshake
    kwp_send_address(address);
    kwp_result_t result = _wait_for_55_01_8a();
    if (result != KWP_SUCCESS) { return result; }
    _delay_ms(30);
    result = _send_byte(0x75);
    if (result != KWP_SUCCESS) { return result; }
    uart_put(UART_DEBUG, '\n');

    // Receive 0xF6 ASCII/data blocks until we have control of the connection
    // Most modules will send 4 ASCII/data blocks: 3 with component info, 1 with workshop/coding
    // Premium 5 radio in mfg mode (address 0x7c) sends no ASCII/data blocks
    uint8_t ascii_blocks = 0;
    while (1) {
        result = kwp_receive_block();
        if (result != KWP_SUCCESS) { return result; }

        if (kwp_rx_buf[2] == KWP_ACK) { return KWP_SUCCESS; }
        if (kwp_rx_buf[2] != KWP_R_ASCII_DATA) { return KWP_UNEXPECTED; }

        switch (ascii_blocks++) {
            case 0:    // 0xF6 (ASCII/Data): "1J0035180D  "
                memcpy(&kwp_vag_number,  &kwp_rx_buf[3], 12); break;
            case 1:    // 0xF6 (ASCII/Data): " RADIO 3CP  "
                memcpy(&kwp_component_1, &kwp_rx_buf[3], 12); break;
            case 2:    // 0xF6 (ASCII/Data): "        0001"
                memcpy(&kwp_component_2, &kwp_rx_buf[3], 12); break;
            case 0xFF: // insane case of too many blocks
                return KWP_UNEXPECTED;
            default:   // ignore block
                break;
        }

        result = kwp_send_ack_block();
        if (result != KWP_SUCCESS) { return result; }
    }
}

kwp_result_t kwp_autoconnect(uint8_t address)
{
    const uint16_t baud_rates[2] = { 9600, 10400 };

    for (uint8_t try=0; try<2; try++) {
        for (uint8_t baud_index=0; baud_index<2; baud_index++) {
            kwp_result_t result = kwp_connect(address, baud_rates[baud_index]);
            if (result == KWP_SUCCESS) { return result; }

            const char *msg = kwp_describe_result(result);
            uart_puts(UART_DEBUG, (char *)msg);
            _delay_ms(2000); // delay before next try
        }
    }
    return KWP_TIMEOUT;
}

kwp_result_t kwp_disconnect()
{
    _delay_ms(5000);
    return KWP_SUCCESS;
}

void kwp_print_module_info()
{
    uart_puts(UART_DEBUG, "VAG Number: \"");
    for (uint8_t i=0; i<12; i++) {
        uart_put(UART_DEBUG, kwp_vag_number[i]);
    }
    uart_puts(UART_DEBUG, "\"\n");

    uart_puts(UART_DEBUG, "Component:  \"");
    for (uint8_t i=0; i<12; i++) {
        uart_put(UART_DEBUG, kwp_component_1[i]);
    }
    for (uint8_t i=0; i<12; i++) {
        uart_put(UART_DEBUG, kwp_component_2[i]);
    }
    uart_puts(UART_DEBUG, "\"\n");
}
