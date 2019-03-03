# MB89677AR Main-MCU I/O Pin Usage

| Pin | MCU Name | Board Name       | In/Out | Description |
|-----|----------|------------------|--------|-------------|
|40   | P00/AD0  | EEPROM_DI        | I      | Data input terminal from 93C46 |
|39   | P01/AD1  | SCA_ENABLE       | O      | Chip select for cassette mechanism |
|38   | P02/AD2  | FM_SD            | I      | FM station detect. High = station detected |
|37   | P03/AD3  | AM_SD            | I      | AM station detect. High = station detected |
|36   | P04/AD4  | PLL_CE           | O      | Chip select output for LC72191 |
|35   | P05/AD5  | EEPROM_CS        | O      | Chip select for 93C46 EEPROM |
|34   | P06/AD6  | PLL_DI           | I      | Data input from LC72191 |
|33   | P07/AD7  | DO               | O      | Data output 93C46 EEPROM and LC72911 |
|32   | P10/A08  | SK               |  O     | Clock output to 93C46 and LC72191 |
|31   | P11/A09  | VOLUME_1         |  I     | Volume knob rotary encoder input |
|30   | P12/A10  | VOLUME_2         |  I     | Volume knob rotary encoder input |
|29   | P13/A11  | EQ_BOSE_CLOCK    |  O     | Clock output for LC3100M, TC9212F |
|28   | P14/A12  | EQ_BOSE_DATA     |  O     | Data output for LC3100M, TC9212F |
|27   | P15/A13  | EQ_ENABLE        |  O     | Chip select for LC3100M (TODO) |
|26   | P16/A14  | BOSE_ON          |  O     | Output "Hi" in BOSE compliant (TODO: PU1582 only?) |
|25   | P17/A15  | BOSE_STB         |  O     | Output terminal to TC9212F strobe (TODO: PU1582 only?) |
|24   | P20/BUFC | PHANTOM_ON       | O      | Turns this port to "Hi" in antenna diagnosis |
|23   | P21/HAK  | /TAPE_ON         | O      | Chip select for cassette? (TODO) |
|22   | P22/HRQ  | VOL_CLK          | O      | Clock output to M62419FP |
|21   | P23/RDY  | VOL_DATA         | O      | Data output to M62419FP |
|20   | P24/CLK  | /AUDIO_MUTE      | O      | Mutes audio when low |
|19   | P25/WR   | REM_AMP_ON       | O      | External amplifier power control (hi = power on) |
|18   | P26/RD   | MAIN_14V         | O      | 14V power supply control |
|17   | P27/ALE  | MAIN_5V          | O      | 5V power supply control |
|48   |P30/PWM20 | FL_CLIP_ON       | ?      | Clipping, unknown (TODO) |
|47   |P31/PWM21 | RL_CLIP_ON       | ?      | Clipping, unknown (TODO) |
|46   |P32/UDZ1  | RR_CLIP_ON       | ?      | Clipping, unknown (TODO) |
|45   |P33/UDB1  | FR_CLIP_ON       | ?      | Clipping, unknown (TODO) |
|44   |P34/UDA1  | APC_DET          | I      | Detection singal input for blanks between music |
|43   |P35/UDZ2  | /TAPE_DOLBY_ON   | O      | Dolby on = low |
|42   |P36/UDB2  | TAPE_TRACK_SW    | O      | Output terminal to Dolby IC for tape side (TODO) |
|41   |P37/UDA2  | SCA_ME_FE        | I      | Cassette metal input.  High = metal |
|57   | P40/PWM00 | CD_DAT_OUT      | O      | CD changer data output |
|56   | P41/PWM01 | S2M_DAT_IN      | I      | Sub-to-Main SPI bus data input |
|54   | P42/PWM10 | /CONTROL+       | I      | Input to detect when REM AMP ON is shorted.  Low = shorted |
|53   | P43/PWM11 | /SUB_RESET      | O      | Reset output to Sub-MCU (low = reset) |
|52   | P44/TCI   | M2S_DAT_OUT     | O      | Main-to-Sub SPI bus data output |
|51   | P45/TCO1  | /M2S_CLK_OUT    | O      | Main-to-Sub SPI bus clock output |
|50   | P46/TCO2  | EE_INITIAL      | I      | When "low" for 2 seconds, makes initial EEPROM setup except CATS code |
|49   | P47/EC    | CATS_LED        | O      | CATS LED output |
|76   | P50/AN0   | AF_LEVEL_IN     | I      | Input to judge FM modulation level |
|75   | P51/AN1   | NEIGHBOR_IN     | I      | Input to judge interference level of adjacent FM channel |
|74   | P52/AN2   | MULTIPASS_IN    | I      | Input to judge multipass level in FM |
|73   | P53/AN3   | S_METER_IN      | I      | Input to detect electric field strength measuring in TEST mode |
|72   | P54/AN4   | BACKUP_AD       | I      | Backup voltage detection input |
|71   | P55/AN5   | Not used?       | ?      | ? |
|70   | P56/AN6   | Not used?       | ?      | ? |
|69   | P57/AN7   | DIAG_PHANTOM    | I      | Input terminal to judge PHANTOM circuit status |
|68   | P60/INT0  | /VOLUME_IN      | I      |  Rotary switch pulse input to detect quick rotation of knob |
|67   | P61/INT1  | /S2M_CLK_IN     | I      |  Sub-to-Main SPI bus clock input |
|66   | P62/INT2  | /DIAG_RX        | I      |  Signal input from self-diagnostic tester |
|65   | P63/INT3  | /SCA_CLK_IN     | I      |  Sub-to-Main SPI bus clock input |
|64   | P64/INT4  | SCA_SWITCH      | I      |  Cassette presence input.  Low = cassette is in mechanism |
|63   | P65/INT5  | /S2M_ENA_IN     | I      |  Sub-to-Main SPI bus enable input |
|62   | P66/INT6  | /ACC_IN         | I      |  ACC input terminal.  ACC ON when low |
|61   | P67/INT7  | /POWER_EJECT_SW | I      |  Low when either POWER or EJECT is pressed |
|6    | P70/BZ1   | BEEP            | O      | Beep signal output.  Outputs 0.6kHz for 100ms |
|5    | P71/UCK   | SCA_DATA        | I/O    | Data input/output to cassette mechanism |
|4    | P72/UO    | TX              | O      | Data output to self-diagnostic tester U537 |
|3    | P73/UI    | RX              | I      | Data input from self-diagnostic tester U537 |
|2    | P74/SCK   | /CD_CLK         | I      | Clock input from CD changer |
|1    | P75/SO    | /POWER_EJECT    | I      | Used to differentiate POWER vs EJECT key press.  Low = POWER |
|80   | P76/SI    | CD_DATA_IN      | I      | CD changer data input |
|10   | P80       | Not used?       | ?      | ? |
|9    | P81       | Not used?       | ?      | ? |
|8    | P82       | Not used?       | ?      | ? |
|7    | P83       | /SCA_CLK_OUT    | O      | Clock output terminal to cassette mechanism |
|60   | P84       | AMP_ON          | O      | Internal amplifier control. High = Amp on.  Low when BOSE compliant |
|59   | P85       | /M2S_ENA_OUT    | O      | Main-to-Sub SPI bus enable output |
