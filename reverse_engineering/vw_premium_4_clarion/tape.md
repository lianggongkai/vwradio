# Cassette Tape

# SCA4.4 (Cassette Mechanism with TDA3612)

```text
FE/ME:
    output
    metal tape indicator:
        low = non-metal tape
        high = metal tape

MONITOR:
    output
    error condition indicator:
        low = error
        high = no error

SWITCH:
    output
    no tape = constant low
    tape in, not playing = constant high
    play tape:
        ENABLE high->low
        286 ms delay
        SWITCH high->low
        337 ms delay
        SWITCH low->high
        77 ms delay
        SWITCH high->low
        (SWITCH stays low until STOP is pressed, then goes constant high)

ENABLE:
    input
    idles high, radio pulls low to select TDA3612

DATA:
    input
    idles high

CLOCK:
    bidirectional
    idles high
    if ENABLE=low, the radio has selected the TDA3612, and drives
      CLOCK to shift bits out on DATA
    if ENABLE=high, and a cassette is playing, the TDA3612 pulses
      the CLOCK line continuously (tachometer?): 25ms off, 1ms on.
    while playing, if the clock pulses are interrupted for about 500ms(?),
      the radio will report a side switch (e.g. "PLAY A" -> "PLAY B").
```

## Radio Board Connections

SCA4.4      MB89677A            Notes
MONITOR     N/C                 Appears completely unconnected
DATA         5 SCA DATA         47K pull-up to Main 5V
CLOCK        7 SCA CLK OUT      47K pull-up to Main 5V
            65 SCA CLOCK IN     Pins 4 & 65 shorted together
ENABLE      39 SCA ENABLE       ?? No pull-up or pull-down ??
FE/ME       41 SCA ME/FE        47K pull-up to Main 5V
SWITCH      64 SCA SWITCH       ?? No pull-up or pull-down ??

SCA4.4 board itself has 220 Ohm resistors inline with CLOCK, DATA,
ENABLE.

?? There doesn't seem to be pull-up resistors on ENABLE and SWITCH. ??

Audi Concert/Chorus schematic shows 47K resistors to 5V on CLOCK,
DATA, ENABLE, and SWITCH.  It has a 10K resistor to 5V on FE/ME.


# SPI Commands

```text
PU-1582 with Tape Simulator:
  Initial 12V Power Up:       0xF0, 0x9A, 0xDB, 0xA8, 0xF0, 0x00
  Tape inserted during SAFE:  0xAC, 0xA8, 0xAA, 0x9A, 0xDB, 0xC2, 0xF0, 0x00
  Tape inserted:              0xAC, 0xA8, 0xAA, 0x9A, 0xDB, 0xC2, 0xDF, 0x52
  Tape Side during PLAY A:    0xAE, 0xE7, 0x62
  Tape Side during PLAY B:    0x9E, 0xD7, 0x52
  Stop during PLAY A:         0x52, 0xAA, 0xF0, 0x00
  Stop during PLAY B:         0x62, 0x9A, 0xF2, 0xF0, 0x00
  Play after stop on side A:  0xA8, 0xAA, 0x9A, 0xDB, 0xC2, 0xDF, 0x52
  Play after stop on side B:  0xA8, 0xAA, 0x9A, 0xDB, 0xC2, 0xAE, 0xE7, 0x62
  MSS FF during PLAY A:       0xC0, 0xD2, 0xEA, 0xF0, 0x00
  MSS FF during PLAY B:       0xC0, 0xE2, 0xDA, 0xF0, 0x00
  MSS REW during PLAY A:      0xC0, 0xEA, 0xE2, 0xDA, 0xF0, 0x00
  MSS REW during PLAY B:      0xC0, 0xDA, 0xD2, 0xEA, 0xF0, 0x00

PU-1666A with Real SCA4.4:
  Initial 12V Power Up:       0xF8, 0x9A, 0xD8, 0xA8, 0xF0, 0x00, 0xFF, 0, 0, 0, 0, 0, 0, ...
  Tape inserted:              0xF4, 0xAC, 0xA8, 0xAA, 0x9A, 0xD8, 0xC2, 0xDF, 0x52, 0xDF, 0x56
  Tape inserted during SAFE:  0xF4, 0xAC, 0xA8, 0xAA, 0x9A, 0xD8, 0xC2, 0xF0, 0x00
  Tape Side during PLAY A:    0xAE, 0xE7, 0x62, 0xEF, 0x66
  Tape Side during PLAY B:    0x9E, 0xD7, 0x52, 0xDF, 0x56
  Stop during PLAY A:         0x52, 0xAA, 0xF0, 0x00
  Stop during PLAY B:         0x62, 0x9A, 0xF2, 0xF0, 0x00
  Eject during stopped on A:  0xCC, 0xA8, 0xF0, 0x00
  Eject during stopped on B:  0xCC, 0xA8, 0xF0, 0x00
  Play after stop on side A:  0xD4, 0xAA, 0x9A, 0xDB, 0xC2, 0xDF, 0x52, 0xDF, 0x56
  Play after stop on side B:  0xD4, 0xAA, 0x9A, 0xDB, 0xC2, 0xAE, 0xE7, 0x62, 0xEF, 0x66
  MSS FF during PLAY A:       0xC0, 0xD2
  MSS FF during PLAY B:       0xC0, 0xE2
  MSS REW during PLAY A:      0xC0, 0xEA, 0xE2
  MSS REW during PLAY B:      0xC0, 0xDA, 0xD2
```text

Note: there doesn't seem to be a difference between normal FF/REW and MSS FF/REW.


## Simulating Play

```text
configure FE/ME as output, set it permanently low (non-metal tape)
configure MONITOR as output, set it permanently high (no error condition)
configure ENABLE as input, CLOCK as input, SWITCH as output
set SWITCH high
wait for ENABLE high->low
delay 286 ms
set SWITCH low
delay 337 ms
set SWITCH high
delay 77 ms
set SWITCH low
set CLOCK as output
loop forever:
  set CLOCK high for 25ms
  set CLOCK low for 1ms
```

## Simulating Stop

set SWITCH high
stop pulsing CLOCK


## TEA0675 (Cassette Preamplifier with Dolby NR)

To inject external audio, disconnect pin 1 (OUTA) and pin 24 (OUTB) from
the board.  These are the outputs of the TEA0675's preamplifier.  Connect an
audio source to the pads on the board.  Headphone jack audio from a Macbook Pro
and a Raspberry Pi 3 Model B both worked (see Raspberry Pi section below).

Pin 19 (AMSEQ) always seems to be high, regardless of whether there is
audio from the tape head or not.  The radio doesn't appear to use the
AMS feature.  It uses the LA2000 instead (see below).

Audio will only be heard from the TEA0675 if the LA2000's pin 6 is high.  This
means that either a cassette with music must be playing, or the pin has to
be faked.

If the TEA0675 is completely removed from the circuit, the LA2000 will no
longer detect cassette audio.  The TEA0675 can be removed, but the LA2000 must
also be removed and its pin 6 faked.


## LA2000 (Audio Level Sensor)

Pin 6 (APC DET) goes immediately high when there is audio from the tape head and
immediately low when there not.  It does not seem to detect audio from any
other source.  Pin 6 is connected directly to pin 44 on the MCU at IC802.

If the LA2000 is removed and pin 6 tied high, the MCU will always think tape
audio is present and will keep the tape audio active even if the tape head
is disconnected.

Pin 6 can be tied permanently high, but MSS FW/REW will not stop (it will act
like regular FW/REW).  When MSS FW/REW is on, the MCU waits for pin 6 high->low
(end of current song) followed by pin 6 low->high (start of next song).

On the PU-1666A (matrix display), the radio will ignore the LA2000 for BLS
if the "TAPE SKIP" option is set to "Y".  The non-matrix display radios don't
seem to have the "TAPE SKIP" option.


## Raspberry Pi Headphone Jack Audio

The Raspberry Pi 3 Model B's headphone jack will produce acceptable sound when
connected directly to the TEA0675 output pads if the Raspberry Pi is powered
from an external AC adapter.  If the Raspberry Pi is powered from the same 12V
supply as the radio, a filter must be used.

When the Raspberry Pi headphone jack is connected directly to the TEA0675 pads,
and power for the Raspberry Pi is derived from the same 12V supply as the
radio, the sound has significant buzzing and is unusable.  This was true using
both Hobbywing 5V/6V 3A UBEC (switch mode supply) and a 7805 circuit.  It made
no difference if the 5V power was applied at the Raspberry Pi's header pins or
at the pads under the mini-USB power input connector.

Putting the SNI-1/3.5 ground loop noise isolator (pac-audio.com) inline between
the Raspberry Pi and the radio eliminates the noise and makes the sound usable
(tested with the UBEC).


## Raspberry Pi PHAT DAC Audio

The PHAT DAC produces mostly acceptable sound when connected directly to the
TEA0675 output pads.  Some crackling and background noise can be heard, which
is mostly noticeable during silence.  Surprisingly, the noise was less when
the Raspberry Pi was powered by the UBEC than the 7805.  Putting the SNI-1/3.5
inline eliminates the noise.
