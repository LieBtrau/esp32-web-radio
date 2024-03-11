#pragma once


// Using an enum instead of #define allows the compiler to catch typos and avoids defining the same pin twice.
typedef enum pin_def_t
{
    // Don't use pin 12 for I2C, it's a strapping pin (MTDI), which should be pulled down to set the voltage of VDD_SDIO to 3.3V.
    // https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
    PIN_SCL = 14,
    PIN_SDA = 13,
    
    // I2S
    I2S_DOUT = 5,
    I2S_LRC = 2,
    I2S_BCLK = 4,

    // Encoder for volume
    PIN_ENCVOL_S1 = 21,
    PIN_ENCVOL_S2 = 19,
    PIN_ENCVOL_KEY = 18,

    // Encoder for channel
    PIN_ENCCH_S1 = 34,
    PIN_ENCCH_S2 = 39,
    PIN_ENCCH_KEY = 36,

    // News button
    PIN_NEWS_BTN = 33
} pin_def_t;