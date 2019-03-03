# Faceplate

## Signals

| Name | Description |
|------|-------------|
|POW   | Do not read pin directly; read via the signals described in Keys below. |
|EJE   | Do not read pin directly; read via the signals described in Keys below. |
|STB   | Connects to STB on uPD16432B. |
|CLK   | Connects to SCK on uPD16432B. |
|DAT   | Connects to DATA on uPD16432B. |
|BUS   | Connects to KEY REQ on uPD16432B.  Normally 5V, drops to 0V when the uPD16432B has latched key data. |
|RST   | Connects to /RESET on uPD16432B.  If ignition power is on but the radio is turned off by pushing the POWER key, radio holds RST high.  When the POWER key is pressed to turn on, RST pulses low briefly. |
|LOF   | Connects to /LCDOFF on uPD16432B.  If ignition power is on but the radio is turned off by pushing the POWER key, radio holds LOF low. When the radio is turned on, radio holds LOF high. |
|M5V   | Main 5V power controlled by radio.  If ignition power is on but the radio is turned off by pushing the POWER key, M5V drops to 0V. |
|ILL   | Illumination power.  Measures slightly less than B+ (13V) when the car's dash illumination power is on, otherwise measures 0V. |
|L/I   | LCD Illumination power.  Measures 8.75V when the car's dash illumination is on, otherwise measures 0V. |
|CAT   | Clarion Anti-Theft Security LED. |
|VLC   | LCD Drive Voltage.  Measures 6.0V.  If ignition power is on but the radio is turned off by pushing the POWER key, VLC drops to 0V. |

## Keys

All of the keys on faceplate are readable by the uPD16432B except for the Power and Eject keys:

|Key         |(Byte 0-3)  |Bit (0-7)| Notes     |
|------------|------------|---------|-----------|
|"NO CODE"   |   3        | 7       | Hidden    |
|"INITIAL"   |   3        | 6       | Hidden    |
|Volume Down |   3        | 3       | Hidden    |
|Volume Up   |   3        | 2       | Hidden    |
|Seek Down   |   3        | 1       | Hidden    |
|Seek Up     |   3        | 0       | Hidden    |
|Tape Side   |   2        | 5       |           |
|Seek Up     |   2        | 4       |           |
|Mix/Dolby   |   2        | 3       |           |
|Scan        |   2        | 1       |           |
|Seek Down   |   2        | 0       |           |
|FM          |   1        | 7       |           |
|AM          |   1        | 6       |           |
|Tune Up     |   1        | 5       |           |
|Bal         |   1        | 4       |           |
|CD          |   1        | 3       |           |
|Tape        |   1        | 2       |           |
|Tune Down   |   1        | 1       |           |
|Fade        |   1        | 0       |           |
|Preset 6    |   0        | 7       |           |
|Preset 5    |   0        | 6       |           |
|Preset 4    |   0        | 5       |           |
|Bass        |   0        | 4       |           |
|Preset 3    |   0        | 3       |           |
|Preset 2    |   0        | 2       |           |
|Preset 1    |   0        | 1       |           |
|Treb        |   0        |  0       |           |

The keys marked "Hidden" above aren't physically present on the faceplate but the radio responds if the uPD16432B sends them.

The Power and Eject keys are read by the main MCU via two lines.  One is like an interrupt line; it is held low whenever either key is pressed.  The other line distinguishes which of the two is pressed.

| Main MCU Pin   | Name         |  Description |
|----------------|--------------|--------------|
|1               |!POWER_EJECT  |Low while either Power or Eject are pressed.|
|61              |EJECT         |Indicates which key: low=Power, high=Eject  |

Note that these signals are not the same as the POW and EJE pins on the faceplate header.  Those pins go through electronics on the radio board that generates these signals somehow.

## Pictographs

- Pictographs are symbols on the display like the Dolby logo
- 64 possible: 8 bytes, 1 bit per pictograph, bit on = pictograph displayed
- 4 pictographs total on the LCD

|Pictograph  |Byte (0-7)|  Bit (0-7)| Notes  |
|------------|----------|-----------|--------|
|Metal       | 7        | 3         |        |
|Dolby       | 6        | 5         |        |
|Mix         | 6        | 0         |        |
|Period      | 3        | 6         |        |
|Mode: AM/FM | 2        | 3         | Hidden |
|Mode: CD    | 1        | 0         | Hidden |
|Mode: Tape  | 1        | 5         | Hidden |

The pictographs marked "Hidden" above aren't on the LCD but the radio sends them.


## Dot Matrix Characters

Every character is a 5x7 matrix but some are physically smaller on the LCD
11 characters total on the LCD (4 small on left, 7 large on right)
Leftmost character on display is address 0x0c in display RAM
Rightmost character on display is address 0x02 in display RAM

```text
Display:     "     SAFE  "
Display RAM: [?, ?, 0x20, 0x20, 0x45, 0x46, 0xe0, 0x53, 0x20, 0x20, 0x20, 0x20, 0x20, ?...]

Display:     "     1234  "
Display RAM: [?, ?, 0x20, 0x20, 0xe7, 0xe6, 0xf3, 0xe5, 0x20, 0x20, 0x20, 0x20, 0x20, ?...]

Display:     "     4321  "
Display RAM: [?, ?, 0x20, 0x20, 0xe5, 0xf3, 0xe6, 0xe7, 0x20, 0x20, 0x20, 0x20, 0x20, ?...]
```

## Character ROM

Character codes 0x10-0xFF are stored in ROM.  These have been seen on the LCD:

```text
0x20: " "
0x2b: "+"
0x2d: "-"
0x37: "7"
0x38: "8"
0x43: "C"
0x44: "D"
0x45: "E"
0x46: "F"
0x47: "G"
0x48: "H"
0x49: "I"
0x4b: "K"
0x4c: "L"
0x4d: "M"
0x4f: "O"
0x50: "P"
0x52: "R"
0x53: "S"
0x54: "T"
0x55: "U"
0x57: "W"
0x58: "X"
0x59: "Y"
0x5a: "Z"
0xe0: "A"
0xe1: "B"
0xe2: "N"
0xe3: "V"
0xe4: "0"
0xe5: "1"  for both full-size 1 and preset 1
0xe6: "3"  for full-size 3
0xe7: "4"  for full-size 4
0xe8: "5"  for full-size 5
0xe9: "6"  for full-size 6
0xea: "9"  for full-size 9
0xeb: "1"  for FM1
0xec: "2"  for FM2
0xed: "2"  for preset 2
0xee: "3"  for preset 3
0xef: "4"  for preset 4
0xf0: "5"  for preset 5
0xf2: "6"  for preset 6
0xf3: "2"  for full-size 2
```

## Character RAM

Character codes 0x00-0x0F are programmable.

Unlike the Premium 5, the Premium 4 does not send any programmable characters.  The "special" characters like the narrow numbers for the presets are included in the ROM character set on the Premium 4.

## Messages

```text
SAFE mode
  Decoded Display RAM: '    NO CODE'
  Decoded Display RAM: '     1000  '
  Decoded Display RAM: '     SAFE  '
  Decoded Display RAM: '1    1000  '
  Decoded Display RAM: '1    SAFE  '
  Decoded Display RAM: '2    1000  '
  Decoded Display RAM: '2    SAFE  '

Radio AM
  Decoded Display RAM: 'AM 1 670KHZ'
  Decoded Display RAM: 'AM 3 530KHZ'
  Decoded Display RAM: 'SCAN 530KHZ'
  Decoded Display RAM: 'SCAN1710KHZ'

Radio FM1
  Decoded Display RAM: 'FM1  887MHZ'
  Decoded Display RAM: 'FM11 915MHZ'
  Decoded Display RAM: 'SCAN 879MHZ'
  Decoded Display RAM: 'SCAN1079MHZ'

Radio FM2
  Decoded Display RAM: 'FM211041MHZ'
  Decoded Display RAM: 'FM22 881MHZ'
  Decoded Display RAM: 'FM23 981MHZ'
  Decoded Display RAM: 'FM241041MHZ'
  Decoded Display RAM: 'FM251079MHZ'
  Decoded Display RAM: 'FM261079MHZ'

CD
  Decoded Display RAM: 'CD 1 TR 01 '
  Decoded Display RAM: 'CD 2 NO CD '
  Decoded Display RAM: 'CD1 CD ERR ' (no space in "CD1"!)
  Decoded Display RAM: 'CUE   122  '
  Decoded Display RAM: 'REV   310  '
  Decoded Display RAM: 'CD 1  419  ' (shown when exiting CUE or REV)
  Decoded Display RAM: 'SCANCD1TR04'
  Decoded Display RAM: 'CHK MAGAZIN'
  Decoded Display RAM: 'NO  MAGAZIN'
  Decoded Display RAM: 'NO  CHANGER'
  Decoded Display RAM: '    NO DISC'

Tape
  Decoded Display RAM: 'TAPE PLAY A'
  Decoded Display RAM: 'TAPE PLAY B'
  Decoded Display RAM: 'TAPE SCAN A'
  Decoded Display RAM: 'TAPE SCAN B'
  Decoded Display RAM: 'TAPE  BLS  '
  Decoded Display RAM: 'TAPEMSS FF '
  Decoded Display RAM: 'TAPEMSS REW'
  Decoded Display RAM: 'TAPE ERROR '
  Decoded Display RAM: '    NO TAPE'
  Decoded Display RAM: 'TAPE  FF   '
  Decoded Display RAM: 'TAPE  REW  '
  Decoded Display RAM: 'TAPE METAL '

Balance
  Decoded Display RAM: 'BAL CENTER '
  Decoded Display RAM: 'BAL LEFT  7'
  Decoded Display RAM: 'BAL RIGHT 3'

Bass
  Decoded Display RAM: 'BASS  - 9  '
  Decoded Display RAM: 'BASS    0  '
  Decoded Display RAM: 'BASS  + 9  '

Treble
  Decoded Display RAM: 'TREB  + 7  '
  Decoded Display RAM: 'TREB  - 4  '

Fade
  Decoded Display RAM: 'FADEFRONT 9'
  Decoded Display RAM: 'FADECENTER '
  Decoded Display RAM: 'FADEREAR  5'

Volume
  Decoded Display RAM: 'FM1   MAX  '
  Decoded Display RAM: 'FM1   MIN  '

Setting Options
  Decoded Display RAM: 'SET ONVOL13'
  Decoded Display RAM: 'SET ONVOL63'
  Decoded Display RAM: 'SET CD MIX1'
  Decoded Display RAM: 'SET CD MIX6'
  Decoded Display RAM: 'TAPE SKIP N'
  Decoded Display RAM: 'TAPE SKIP Y'

Initializing
  Decoded Display RAM: '    INITIAL'

Code Change
  Decoded Display RAM: '     CODE  '
  Decoded Display RAM: '     1000  '

Diagnostics (KWP1281)
  Decoded Display RAM: '     DIAG  '

Test Mode
  Decoded Display RAM: 'RAD 3CP T7 '
  Decoded Display RAM: 'VER  0702  '
  Decoded Display RAM: 'FERN   OFF '
```
