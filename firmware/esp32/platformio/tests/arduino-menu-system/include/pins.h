#include "stdint.h"

// Don't use pin 12 for I2C, it's a strapping pin (MTDI), which should be pulled down to set the voltage of VDD_SDIO to 3.3V.
// https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
const int PIN_SDA = 23;
const int PIN_SCL = 22;