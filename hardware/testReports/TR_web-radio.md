# Equipment

| Equipment    | Description          | Serial Number | Date of last calibration |
|--------------|----------------------|---------------|--------------------------|
| Multimeter   | Mastech M92          | 20011206033   | -                        |
| Oscilloscope | Siglent SDS2354X HD  | SDS2HBAQ6R0257| TBD                      |
| Power Supply | Siglent SPD3303C     | SPD3EEEC6R0513| TBD                      |

# Test Incident Reports
| Test | Incident                                 | Proposed Solution                                      | Status |
|------|------------------------------------------|--------------------------------------------------------|--------|
| TIR1 | ESP32-WROVER-E-N16R8 fails to flash       | Use WROVER module with 4MB flash instead of 16MB       | Open   |
| TIR2 | J1 is not labeled                         | Add polarity of J1 to silkscreen                       | Open   |
| TIR3 | UART.TX doesn't work above 115200baud     | Remove C20                                             | Open   |
| TIR4 | Noise on 5V line, caused by current peaks | Power for U3.7&8: Add a zero-ohm link from 3V3 and a 0ohm resistor to 5V | Open |
| TIR5 | OLED can not be initialized               | Change I²C-pull-ups R25 & R26 from 10K to 1K, better use 3.3k | Open   |
| TIR6 | Rising edge on PROG./RST is too fast      | Change C26 from 100nF to 1µF              | Open   |
| TIR7 | Rotary encoder doesn't fit                | Change footprint : move the two slots further away from the center, 0.2mm? | Open   |
| TIR8 | UVLO of U1 is not used                   | Replace R1 by 10K+3.3K.  Add 10K from U1.4 to GND | Open   |  
| TIR9 | Pushing a button is not always recognized.           | Replace R34, R35, R36, R37 and R40 by 1K<br/>Replace C31, C32, C33, C34 and C36 by 100nF | Open   |
| TIR10| FFC doesn't go straight into the connector | Shift J2 1mm to the left and lock position in the layout. | Open   |
| TIR11| News button is hard to see | Replace SW3 by a button with a white cap | Open   |
| TIR12 | No termination on high speed lines | Add series termination on SPI/I²C lines | Open   |
| TIR13 | SW1 needs to be pushed for several seconds until the radio turns on. | Improve the soft power latch circuit | Open   |

# Test Results
## PBA assembly remarks
| Action | Expected Result | Observed result | Status |
|--------|-----------------|-----------------|--------|
| Silkscreen on PCB | Polarities are labeled | * J1 is not labeled<br/>* during debug I accidentally put power on the audio output. | TIR2 |
| rotary encoder | easy fit | A hard push is needed to fit these | TIR7 |

## Power & reset
| Soft-start of power supply | ms-delay | 1V/ms rising edge<br/>It's important to have reverse current protection (with a series diode.  Q4 doesn't provide reverse current protection).  So that a dropping input voltage, due to high source resistance, doesn't discharge the elcos.  Otherwise a non-monotonous rising edge might occur. | ✅ |
| 5V on C9, without ESP Prog connected | 0V | 0V | ✅ |
| Noise on 5V line |  | 100mVpp | TIR4 |
| 5V on C9, with ESP Prog connected | 5V | 5.01V | ✅ |
| 3.3V on C13, with ESP Prog connected | 3V3 | 3.30V | ✅ |
| Noise on 3.3V on C13 |  | 10mVpp | ✅ <br/>Good news, the ESP32 filtering works perfectly! |
| Noise on ESP32 power C24 | | 12mVpp | ✅ |
| VLED on C6, with ESP Prog connected | 13V | 12.89V | ✅ (ignoring tolerances for now)|
| Reset rising edge | VDD must be stable 50µs before RST signal comes high  | Reset rises too fast. | TIR6 |

## Other
| Action | Expected Result | Observed result | Status |
|--------|-----------------|-----------------|--------|
| Uploading code to ESP32 | flashing works | * flashing ESP32-WROVER-E-N16R8 using default upload speed (460800baud) fails <br/>* Code upload at 115200 works | TIR1 |
| Checking UART.RX | switches between 3V3 and 0.5V | follows cathode of D8 | ✅ |
| Checking UART.TX | switches between 5V and 0V | * switches between 0.5V and 3V3<br/>* ESP-Prog UART is always 3V3, no matter the output voltage setting of the ESP-Prog<br/>* The ESP32-WROVER has an internal 499ohm resistor on its TX-line, so don't choose the pull-ups a too small value because you'll end up above the Vil of the ESP-Prog. | TIR3 |
| Checking audio output | playing sound | sound plays, but noise is just as loud as the music | TIR4 |
| OLED | shows text | Can not be initialized because SDA=0 (it is 1.5VDC) | TIR5 |
| Button debouncing | no bouncing | When switch is pushed, voltage on IO-line is 0.73V which is too high<br/>The 10K series resistors in the debouncing circuit are too large.  They form a resistor divider with the pull-up resistor inside the MCU. | TIR9 |

# Test Result Details

## TIR4 : Finding out where 5V noise comes from
The noise starts after the ESP32 has booted.

* Pulling /ENABLE low by soldering a wire to GND to avoid ground loops caused by the ESP-Prog.  No effect.
* Disabling U1 has no effect on the noise.  That was to be expected, because it's always powered.
* Measuring noise on C8 : 55mVpp.  So the noise is not coming from the lab power supply.  It's our circuit that's causing the noise.
* Measuring noise on C13 (3V3) : 10mVpp.  So the noise is not coming from the 3V3 line either.
* I accidentally put power on the audio output.  That may have broken the audio amplifier.
* WS and BCLK are switching even when no sound is playing.  SD remains low when no sound is playing. 
* Diode check reveals that U3.9 is short-circuited to +5V.
* To prevent this from happening again, a [transmission gate](https://github.com/LieBtrau/electronics-design-data/tree/master/circuits/transmission_gate) circuit can be added in series with L3 or L4.  This will prevent the audio amplifier from being powered when the ESP32 is not powered.

* Wifi current peaks : 130mA, 5ms wide, every 103ms.
  * The impedance of the power supply is too high.  Currently it's almost 1ohm.  We don't control this impedance.  If a cheap USB-cable is used, this impedance can get even worse.
  * These slow high current peaks can not be filtered out by increasing the capacitors.  At least 10000uF would be needed to filter out these peaks.  This is not feasible.
  * L1 has an Rdc of 730mOhm at 1A, that's too high to be put in series with the power supply of the ESP32.  
* Golden rule : don't power IC's from an unregulated power supply.  The cheapest fix is to power U3 (MAX98357) from 3V3 instead of from 5V.

## TIR5 : OLED can not be initialized
### Chinese devboard with OLED.
The OLED screen works fine on the chinese devkit.  It has an RC on the reset line.  It takes about 50ms for the reset line to reach 3V3.  The 3V3 of the NodeMCU32S falls down to 1.5V during bootup of the board.  The 3V3 of the NodeMCU32s isn't great.  It suffers hard (drops down to 1.68V) due to current spikes of WiFi.  Still, the OLED works here.

Measuring power on sequence : VLED first, then +3V3 and PROG./RST as the last.  This is wrong.  VLED should be the last to power up.  The OLED-screen doesn't seem to care.


### ESP32-web radio
Measuring +3V3, PROG./RST and VLED shows the following power-up sequence: VLED first, then +3V3 and PROG./RST as the last.  This is wrong.  VLED should be the last to power up.

U1 is a boost converter, which has not output disconnect.  The minimum output voltage is the input voltage minus a diode drop.  A PMOS in series with the output or input could be used to prevent the OLED from being powered when the ESP32 is not powered.

1. Pull-up resistors on I²C are 10K, which is too high.  These (R25 & R26) have been replaced by 1K.
2. IO15 is a strapping pin.  It should be pulled high.  It's currently pulled low by R17.  It would be better to choose another GPIO-pin for /CS.

Process of replacing circuitry on the Chinese devkit with the circuitry of the ESP32-web radio:
* Use VLED instead of booster U1 to power the OLED : ✅
* Replace R6 914K by 1M : ✅
* Internal reset of the OLED-screen by PROG./RST : ✅
* VCOMH too much capacitive load?  SSD1309 datasheet recommends 2.2µF.  Solder 10µF/50V in parallel to C5 on the chinese devkit.  ✅

The problem can not be fixed on this unit of ESP32-web radio.  Let's take the second unit and do the following modifications:
* cut the trace /CS loose from the ESP32.  It's a strapping pin.
* Replace R25 and R26 by 1K.
result : ✅


