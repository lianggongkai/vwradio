# EEPROM Layout

This information came from studying the PU-1666A firmware.  However, I removed the EEPROM from radios with part numbers PU-1569A, PU-1582A, PU-1634A, PU-1635A, PU-1666A, and PU-1667A.  The SAFE code location was the same in all of them.

Mode (FM1/FM2/AM) do not seem to affect the EEPROM.

Fade and Balance do not seem affect the EEPROM.

| Address | Contents |
|---------|----------|
|0x00     | FM1 Preset 1 (0x00=87.9 MHz, 0x64=107.9 MHz)
|0x01     | FM1 Preset 2
|0x02     | FM1 Preset 3
|0x03     | FM1 Preset 4
|0x04     | FM1 Preset 5
|0x05     | FM1 Preset 6
|0x06     | Flags: CD MIX, TAPE SKIP, SAFE mode
|0x07     | ONVOL
|...      | ...
|0x0a     | SAFE attempts (0, 1, 2)
|...      | ...
|0x0e     | SAFE code BCD high byte (protected by firmware, always returns 0x00)
|0x0f     | SAFE code BCD low byte  (protected by firmware, always returns 0x00)
|...      | ...
|0x10     | FM2 Preset 1 (0x00=87.9 MHz, 0x64=107.9 MHz)
|0x11     | FM2 Preset 2
|0x12     | FM2 Preset 3
|0x13     | FM2 Preset 4
|0x14     | FM2 Preset 5
|0x15     | FM2 Preset 6
|...      | ...
|0x16     | Bass
|0x17     | Treble
|...      | ...
|0x20     | AM Preset 1 (0x00=530 KHz, 0x76=1710 KHz)
|0x21     | AM Preset 2
|0x22     | AM Preset 3
|0x23     | AM Preset 4
|0x24     | AM Preset 5
|0x25     | AM Preset 6
|...      | ...
|0x50     | 0x03    Fault code: 0x0354 Loudspeaker(s) Front
|0x51     | 0x54
|0x52     | 0x24    Fault sub-code
|0x53     | 0x32
|...      | ...
|0x54     | 0x03    Fault code: Loudspeaker(s) Rear
|0x55     | 0x55
|0x56     | 0x24
|0x57     | 0x32    Fault sub-code
|...      | ...
|0x58     | 0xFF    Fault code: Internal memory (??)
|0x59     | 0xFF
|0x5A     | 0x80    Fault sub code (??)
|0x5B     | 0x25
|...      | ...
|0x5C     | 0x04    Fault code: 0x0414 Control Module Incorrectly Coded
|0x5D     | 0x14
|0x5E     | 0x23    Fault sub-code 0x2332
|0x5F     | 0x24
