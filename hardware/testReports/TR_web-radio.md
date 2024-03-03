# Equipment

| Equipment | Description | Serial Number | Date of last calibration |
|-----------|-------------|---------------|--------------------------|
| Multimeter | Mastech M92 | 20011206033 | - |
| 

# Test Incident Reports

| Test | Incident | Proposed Solution | Status |
|------|----------|-------------------|--------|
| TIR1 | ESP32-WROVER-E-N16R8 fails to flash | Use WROVER module with 4MB flash instead of 16MB | Open |
| TIR2 | J1 is not labeled | Add polarity of J1 to silkscreen | Open |
| TIR3 | UART.TX doesn't work above 115200baud | remove C20 | Open |
| TIR4 | Noise on audio output |  | Open |
| TIR5 | Lots of noise on 5V line |  | Open |

# Test Results
| Action | Expected Result | Observed result | Status |
|--------|-----------------|-----------------|--------|
| Silkscreen on PCB | Polarities are labeled | * J1 is not labeled<br/>* during debug I accidentally put power on the audio output. | TIR2 |
| 5V on C9, without ESP Prog connected | 0V | 0V | ✅ |
| noise on 5V line |  | 500mVpp | TIR5 |
| 5V on C9, with ESP Prog connected | 5V | 5.01V | ✅ |
| 3.3V on C13, with ESP Prog connected | 3V3 | 3.30V | ✅ |
| Noise on 3.3V on C13 |  | 10mVpp | ✅ <br/>Good news, the ESP32 filtering works perfectly! |
| Noise on ESP32 power C24 | | 12mVpp | ✅ |
| VLED on C6, with ESP Prog connected | 13V | 12.89V | ✅ (ignoring tolerances for now)|
| Uploading code to ESP32 | flashing works | * flashing ESP32-WROVER-E-N16R8 using default upload speed (460800baud) fails <br/>* Code upload at 115200 works | TIR1 |
| Checking UART.RX | switches between 3V3 and 0.5V | follows cathode of D8 | ✅ |
| Checking UART.TX | switches between 5V and 0V | * switches between 0.5V and 3V3<br/>* ESP-Prog UART is always 3V3, no matter the output voltage setting of the ESP-Prog<br/>* The ESP32-WROVER has an internal 499ohm resistor on its TX-line, so don't choose the pull-ups a too small value because you'll end up above the Vil of the ESP-Prog. | TIR3 |
| Checking audio output | playing sound | sound plays, but noise is just as loud as the music | TIR4 |

# Test Result Details

## Finding out where 5V noise comes from
The noise starts after the ESP32 has booted.

* Pulling /ENABLE low by soldering a wire to GND to avoid ground loops caused by the ESP-Prog.  No effect.
* Disabling U1 has no effect on the noise.  That was to be expected, because it's always powered.
* Measuring noise on C8 : 55mVpp.  So the noise is not coming from the lab power supply.  It's our circuit that's causing the noise.
* Measuring noise on C13 (3V3) : 10mVpp.  So the noise is not coming from the 3V3 line either.
* I accidentally put power on the audio output.  That may have broken the audio amplifier.
* WS and BCLK are switching even when no sound is playing.  SD remains low when no sound is playing. 
* Diode check reveals that U3.9 is short-circuited to +5V.