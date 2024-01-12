#pragma once


// Using an enum instead of #define allows the compiler to catch typos and avoids defining the same pin twice.
typedef enum pin_def_t
{
    // Don't use pin 12 for I2C, it's a strapping pin (MTDI), which should be pulled down to set the voltage of VDD_SDIO to 3.3V.
    // https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
    PIN_SCL = 14,
    PIN_SDA = 13,
    
    // I2S
    I2S_DOUT = 23,
    I2S_LRC = 22,
    I2S_BCLK = 21,

    // Encoder 1
    PIN_ENC1_S1 = 33,
    PIN_ENC1_S2 = 34,
    PIN_ENC1_KEY = 32,

    // Encoder 2
    PIN_ENC2_S1 = 36,
    PIN_ENC2_S2 = 39,
    PIN_ENC2_KEY = 35,

    // News button
    PIN_NEWS_BTN = 18
} pin_def_t;