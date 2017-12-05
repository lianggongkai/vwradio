#ifndef KWP1281_H
#define KWP1281_H

#include <stdint.h>

void connect(uint8_t address);
void send_read_ram_block(uint16_t address, uint8_t length);
void send_read_eeprom_block(uint16_t address, uint8_t length);
void send_group_reading_block(uint8_t group);
void send_login_block(uint16_t safe_code, uint8_t fern, uint16_t workshop);
void send_f0_block();
void send_ack_block();
void receive_block();
void read_all_ram();

#endif
