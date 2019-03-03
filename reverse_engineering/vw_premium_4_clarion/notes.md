# Notes

## Test Mode

If the radio is in FM1 or FM2 mode only, hold the "BAL" key for 10+ seconds to enter test mode.

Press "TAPE" to move forward through the test mode displays.  After the last display ("VER"), it wraps around to the first display ("RAD").

Press "CD" to moves backward through the test mode displays.  After the first display ("RAD"), it wraps around to the last display ("VER").

No other key seems functional.  The radio seems to stay in test mode indefinitely.  Hold the "BAL" key for 10+ seconds again to exit test mode.

Test mode displays:
```text
  'RAD 3CP T7 '
  'FERN   OFF '
  'VER  0702  '
```

## Set Options

Hold the "SCAN" key for 5+ seconds enters a mode to set options.

Press "TUNE UP" or "SEEK UP" to move forward through the options.  After the last option ("TAPE SKIP"), it wraps around to the first option ("SET ONVOL").

Press "TUNE DOWN" or "SEEK DOWN" to move backward through the options.  After the first option ("SET ONVOL"), it wraps around to the last option ("TAPE SKIP").

Press "SCAN" to move forward through the options.  If "SCAN" is pressed on the last option ("TAPE SKIP"), no matter what keys were used to get there, the radio exits this mode.

Turn the volume knob to change the current option.  The hidden volume keys do not work to change the option.

No other key seems functional.  If no key is pressed and the volume knob is not turned for 30+ seconds, the radio will automatically exits this mode.

Set options displays:
```text
  'SET ONVOL13'
  'SET CD MIX1'
  'TAPE SKIP N'
```

# Security Code

The security code can be disabled or changed, even if the radio is locked out.

Hold the hidden "INITIAL" key for 5+ seconds.  The radio will display "INITIAL" and then shut itself off.  This will erase all of the radio's previous settings, including its security code.

Press the "POWER" key to turn the radio back on.  The radio will display "FM1  88.3MHZ".  The radio will not respond to any keys on the faceplate.

Press the hidden "NO CODE" key for 5+ seconds.  The radio will show "NO CODE" on the display.  The security code has now been disabled and the radio will work normally.  If the radio is powered off, it will display "NO CODE" when powered on again and will work normally.

If the security code has been disabled ("NO CODE" state), the radio will allow a new security code to be set.  It will not allow a new code to be set if the code has not been disabled.  To set a new code, hold the "TAPE SIDE" key for 10+ seconds.  This will display "CODE" and then "1000".  Use the preset buttons to toggle in any four digits, then press the "TAPE SIDE" key.  The radio will display "SAFE" and then prompt for a code.  The radio can now be unlocked with the new code.

## Diagnostics (KWP1281)

The radio is address 56 on KWP1281.  Connecting to the radio on KWP1281 will cause it to stop whatever it is doing and enter diagnostics mode.  No faceplate keys seem to work in diagnostics mode.  The radio returns to its previous operation after exiting diagnostics mode.

Diagnostics displays:
```text
  '     DIAG  '
```

Special case: if the radio is in Test Mode at the time it is accessed on KWP1281, it will enter diagnostics mode but it will not show the "DIAG" display. It will leave the current Test Mode display but the faceplate keys won't work. After exiting diagnostics mode, the Test Mode will continue to work as it was before it was interrupted.

Coding: The Premium 4 will control a CD changer regardless if the soft coding is set for a CD changer or not.  However, it appears that the Premium 4 will only set the 00855 fault code for CD changer communication if it has been soft coded for a CD changer.

## Power

The main MCU has constant 5V power on pin 55 (Vdd).  It appears that Vdd is always on (TODO: check what happens to Vdd with ignition power on/off).

The Main 5V line (M5V) that supplies the faceplate and the "SCA-R" header is switched on/off by the MCU.  The +14V line on the "SCA-4.4" header is also switched by the MCU.  When power is first applied to the radio, M5V seems to turn on and off a couple of times, then stays off unit it is powered up with the POWER button.
