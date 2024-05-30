#pragma once

#include "Audio.h"
#ifdef ESP32_AUDIO_KIT
#include "ES8388.h"
#endif

class Music
{
public:
    Music(const int i2c_sda = -1, const int i2c_scl = -1, const int pin_PA_enable = -1);
    ~Music();

    bool init(const uint8_t i2s_bck, const uint8_t i2s_ws, const uint8_t i2s_data_out);
    bool playSpeech(const char *text, const char *lang, bool isBlocking = false);
    bool startStream(const char *url, const uint8_t volume, bool stopCurrentAudio = true);
    bool stopStream();
    void update();
    bool isPlaying();
    bool increaseVolume();
    bool decreaseVolume();
    bool setVolume(const uint8_t volume);
    uint8_t getVolume();
    uint8_t getMaxValue() const;

private:
    uint8_t _volume = 6;
    Audio _audio;
    const static int MIN_VOLUME = 0;
#ifdef ESP32_AUDIO_KIT
    ES8388 _dac;
    int _i2c_sda = -1, _i2c_scl = -1, _pin_PA_enable = -1;
#endif
};