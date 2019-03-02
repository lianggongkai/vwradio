# Faceplate

## Keys

|Key         |(Byte 0-3)  |Bit (0-7)|
|------------|------------|---------|
|AM          |   3        |   7     |
|Preset 6    |   3        |   6     |
|Preset 5    |   3        |   5     |
|Preset 4    |   3        |   4     |
|SCAN        |   3        |   3     |
|Tune Down   |   3        |   2     |
|Tune Up     |   3        |   1     |
|Mix/Dolby   |   3        |   0     |
|FM          |   2        |   7     |
|Seek Up     |   2        |   6     |
|Seek Down   |   2        |   5     |
|CD          |   2        |   3     |
|Preset 1    |   2        |   2     |
|Preset 2    |   2        |   1     |
|Preset 3    |   2        |   0     |
|Treb        |   1        |   7     |
|Mid         |   1        |   6     |
|Bass        |   1        |   5     |
|F/B         |   1        |   4     |
|Tape        |   1        |   3     |
|Tape Side   |   1        |   0     |

TODO How are the Power and Eject keys read on Premium 5?


# Pictographs

Pictographs are symbols on the display like the Dolby logo.

64 possible: 8 bytes, 1 bit per pictograph, bit on = pictograph displayed

4 pictographs total on the LCD

|Pictograph  | Byte (0-7)  | Bit (0-7)|
|------------|-------------|----------|
|Period      |    4        |   5      |
|Metal       |    2        |   7      |
|Dolby       |    1        |   2      |
|Mix         |    5        |   1      |

# Dot Matrix Characters

Every character is a 5x7 matrix but some are physically smaller on the LCD
11 characters total on the LCD (4 small on left, 7 large on right)
Leftmost character on display is address 0 in display RAM
Rightmost character on display is address 0x0a in display RAM

```text
Display:     "NO CHANGER"
Display RAM: [0x4e, 0x4f, 0x20, 0x20, 0x43, 0x48, 0x41, 0x4e, 0x47, 0x45, 0x52, ?...]

Display:     "    1234   "
Display RAM: [0x20, 0x20, 0x20, 0x20, 0x31, 0x32, 0x33, 0x34, 0x20, 0x20, 0x20, ?...]

Display:     "    4321   "
Display RAM: [0x20, 0x20, 0x20, 0x20, 0x34, 0x33, 0x32, 0x31, 0x20, 0x20, 0x20, ?...]
```

# Character ROM

Character codes 0x10-0xFF are stored in ROM.  These have been sent from the
radio and seen on the LCD:

```text
0x20: " "
0x2b: "+"
0x2d: "-"
0x30: "0"
0x31: "1"
0x32: "2"
0x33: "3"
0x34: "4"
0x35: "5"
0x36: "6"
0x37: "7"
0x38: "8"
0x39: "9"
0x41: "A"
0x42: "B"
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
0x4e: "N"
0x4f: "O"
0x50: "P"
0x52: "R"
0x53: "S"
0x54: "T"
0x55: "U"
0x56: "V"
0x57: "W"
0x58: "X"
0x59: "Y"
0x5a: "Z"
0x65: "e"
0x6b: "k"
0x72: "r"
0x73: "s"
0x7a: "z"
```

Character RAM
-------------

Character codes 0x00-0x0F are programmable.  The radio sends these:

```text
  0x00:  0x01:  0x02:  0x03:  0x04:  0x05:  0x06:  0x07:  0x08:
  .O...  OOO..  ....O  ..OOO  ..OOO  ..O.O  ..OOO  ..OOO  OOOOO
  .O...  ..O..  ....O  ....O  ....O  ..O.O  ..O..  ..O..  OOOOO
  .O...  ..O..  ....O  ....O  ....O  ..O.O  ..O..  ..O..  OOOOO
  .O...  OOO..  ....O  ..OOO  ...OO  ..OOO  ..OO.  ..OOO  OOOOO
  .O...  O....  ....O  ..O..  ....O  ....O  ....O  ..O.O  OOOOO
  .O...  O....  ....O  ..O..  ....O  ....O  ....O  ..O.O  OOOOO
  .O...  OOO..  ....O  ..OOO  ..OOO  ....O  ..OO.  ..OOO  OOOOO
```

 - 0x00: Narrow "1" for FM 1
 - 0x01: Narrow "2" for FM 2
 - 0x02: Narrow "1" for AM/FM preset 1
 - 0x03: Narrow "2" for AM/FM preset 2
 - 0x04: Narrow "3" for AM/FM preset 3
 - 0x05: Narrow "4" for AM/FM preset 3
 - 0x06: Narrow "5" for AM/FM preset 3
 - 0x07: Narrow "6" for AM/FM preset 3
 - 0x08: All bits on; has not been seen on LCD

# Messages

```text
SAFE mode
  Decoded Display RAM: '    1000   '
  Decoded Display RAM: '1    SAFE  ' (note extra space before SAFE)
  Decoded Display RAM: '1   1000   '

Radio AM
  Decoded Display RAM: 'AM  1000kHz'
  Decoded Display RAM: 'AM <cgram:0x02> 570kHz'
  Decoded Display RAM: 'AM <cgram:0x07>1440kHz'
  Decoded Display RAM: 'SCAN 530kHz'
  Decoded Display RAM: 'SCAN1710kHz'

Radio FM1/2
  Decoded Display RAM: 'FM<cgram:0x00><cgram:0x02>1003MHz'
  Decoded Display RAM: 'FM<cgram:0x00><cgram:0x03>1011MHz'
  Decoded Display RAM: 'FM<cgram:0x00><cgram:0x04>1035MHz'
  Decoded Display RAM: 'FM<cgram:0x00><cgram:0x05>1043MHz'
  Decoded Display RAM: 'FM<cgram:0x00><cgram:0x06>1051MHz'
  Decoded Display RAM: 'FM<cgram:0x00><cgram:0x07> 931MHz'
  Decoded Display RAM: 'FM<cgram:0x01><cgram:0x02> 925MHz'
  Decoded Display RAM: 'SCAN 881MHz'
  Decoded Display RAM: 'SCAN1079MHz'

CD
  Decoded Display RAM: 'CD 1 TR 01 '
  Decoded Display RAM: 'CD 2 NO CD '
  Decoded Display RAM: 'CD 1CD ERR '
  Decoded Display RAM: 'CUE   245  '
  Decoded Display RAM: 'REV   147  '
  Decoded Display RAM: 'CD 1  251  ' (shown when exiting CUE or REV)
  Decoded Display RAM: 'SCANCD1TR04'
  Decoded Display RAM: 'CHK MAGAZIN'
  Decoded Display RAM: 'NO  CHANGER'
  Decoded Display RAM: 'NO  MAGAZIN'
  Decoded Display RAM: '    NO DISC'

Tape
  Decoded Display RAM: '    NO TAPE'
  Decoded Display RAM: 'TAPE LOAD  '
  Decoded Display RAM: 'TAPE METAL '
  Decoded Display RAM: 'TAPE PLAY A'
  Decoded Display RAM: 'TAPE PLAY B'
  Decoded Display RAM: 'TAPE SCAN A'
  Decoded Display RAM: 'TAPE SCAN B'
  Decoded Display RAM: 'TAPEMSS FF '
  Decoded Display RAM: 'TAPEMSS REW'
  Decoded Display RAM: 'TAPE ERROR '
  Decoded Display RAM: 'TAPE  FF   '
  Decoded Display RAM: 'TAPE  REW  '

Balance
  Decoded Display RAM: 'BAL CENTER '
  Decoded Display RAM: 'BAL LEFT  1'
  Decoded Display RAM: 'BAL LEFT  9'
  Decoded Display RAM: 'BAL RIGHT 1'
  Decoded Display RAM: 'BAL RIGHT 9'

Bass
  Decoded Display RAM: 'BASS  - 9  '
  Decoded Display RAM: 'BASS  - 1  '
  Decoded Display RAM: 'BASS    0  '
  Decoded Display RAM: 'BASS  + 1  '
  Decoded Display RAM: 'BASS  + 9  '

Treble
  Decoded Display RAM: 'TREB  - 9  '
  Decoded Display RAM: 'TREB  - 1  '
  Decoded Display RAM: 'TREB    0  '
  Decoded Display RAM: 'TREB  + 1  '
  Decoded Display RAM: 'TREB  + 9  '

Fade
  Decoded Display RAM: 'FADECENTER '
  Decoded Display RAM: 'FADEFRONT 1'
  Decoded Display RAM: 'FADEFRONT 9'
  Decoded Display RAM: 'FADEREAR  1'
  Decoded Display RAM: 'FADEREAR  9'

Midrange
  Decoded Display RAM: 'MID   - 9  '
  Decoded Display RAM: 'MID   - 1  '
  Decoded Display RAM: 'MID     0  '
  Decoded Display RAM: 'MID   + 1  '
  Decoded Display RAM: 'MID   + 9  '

Volume
  Decoded Display RAM: 'FM<cgram:0x00>   MAX  '
  Decoded Display RAM: 'FM<cgram:0x00>   MIN  '

Setting Options
  Decoded Display RAM: 'SET ONVOL 8'
  Decoded Display RAM: 'SET ONVOL45'
  Decoded Display RAM: 'SET CD MIX1'
  Decoded Display RAM: 'SET CD MIX6'
  Decoded Display RAM: 'TAPE SKIP N'
  Decoded Display RAM: 'TAPE SKIP Y'

Monsoon
  Decoded Display RAM: '    MONSOON'

Diagnostics (KWP1281)
  Decoded Display RAM: '     DIAG  '

Test Mode
  Decoded Display RAM: 'RAD   DE2  '
  Decoded Display RAM: 'FERN   OFF '
  Decoded Display RAM: 'VersA99CZ23'
  Decoded Display RAM: ' 5300 2 6 F' (Signal Strength: AM   530KHz = 0x026F)
  Decoded Display RAM: '17100 1 2 3' (Signal Strength: AM  1710KHz = 0x0123)
  Decoded Display RAM: ' 8770 5 3 0' (Signal Strength: FM  87.7Mhz = 0x0530)
  Decoded Display RAM: '10770 6 4 0' (Signal Strength: FM 107.0MHz = 0x0640)
```