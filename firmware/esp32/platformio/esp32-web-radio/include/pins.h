#pragma once

typedef enum pin_def_t
{
    // I2S
    I2S_DOUT = 25,
    I2S_LRC = 26,
    I2S_BCLK = 27,

    // Encoder 1
    PIN_ENC1_S1 = 36,
    PIN_ENC1_S2 = 39,
    PIN_ENC1_KEY = 34,

    // Encoder 2
    PIN_ENC2_S1 = 33,
    PIN_ENC2_S2 = 32,
    PIN_ENC2_KEY = 35
} pin_def_t;