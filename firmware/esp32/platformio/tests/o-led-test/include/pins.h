#include "stdint.h"

// Don't use pin 12 for I2C, it's a strapping pin (MTDI), which should be pulled down to set the voltage of VDD_SDIO to 3.3V.
const int PIN_SDA = 13;
const int PIN_SCL = 9;