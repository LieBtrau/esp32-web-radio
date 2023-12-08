#pragma once

// I2S, pin defs from https://github.com/pschatzmann/arduino-audiokit/blob/main/src/audio_board/ai_thinker_es8388_5.h
#define PIN_I2S_AUDIO_KIT_MCLK 0
#define PIN_I2S_AUDIO_KIT_BCK 27
#define PIN_I2S_AUDIO_KIT_WS 25
#define PIN_I2S_AUDIO_KIT_DATA_OUT 26
#define PIN_I2S_AUDIO_KIT_DATA_IN 35

// I2C, pin defs from https://github.com/pschatzmann/arduino-audiokit/blob/main/src/audio_board/ai_thinker_es8388_5.h
#define I2C_MASTER_NUM I2C_NUM_0 /*!< I2C port number for master dev */
#define I2C_MASTER_SCL_IO 32
#define I2C_MASTER_SDA_IO 33

#define PIN_PA_ENABLE 21
#define PIN_KEY1 36
#define PIN_KEY2 13
#define PIN_KEY3 19
#define PIN_KEY4 23
#define PIN_KEY5 18
#define PIN_KEY6 5
#define PIN_IO22 22