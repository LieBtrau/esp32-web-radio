#include "Music.h"

static const char *TAG = "Music";

Music::Music(const int i2c_sda, const int i2c_scl, const int pin_PA_enable)
#ifdef ESP32_AUDIO_KIT
    : _i2c_sda(i2c_sda), _i2c_scl(i2c_scl), _pin_PA_enable(pin_PA_enable), _dac(ES8388(_i2c_sda, _i2c_scl))
#endif
{
}

Music::~Music()
{
    // free allocated heap memory
}

bool Music::init(const uint8_t i2s_bck, const uint8_t i2s_ws, const uint8_t i2s_data_out)
{
    if (!_audio.setPinout(i2s_bck, i2s_ws, i2s_data_out))
    {
        ESP_LOGE(TAG, "Error setting pinout");
        return false;
    }
#ifdef ESP32_AUDIO_KIT
    if (!dac.init())
    {
        ESP_LOGE(TAG, "Error initializing ES8388 chip");
        return false;
    }
    _dac.setOutputVolume(ES8388::OutSel::OUTALL, _volume);
    if (_pin_PA_enable != -1)
    {
        pinMode(_pin_PA_enable, OUTPUT);
        digitalWrite(_pin_PA_enable, HIGH);
    }
#endif
    return true;
}

bool Music::playSpeech(const char *text, const char *lang, bool isBlocking)
{
    if (_audio.isRunning())
    {
        ESP_LOGW(TAG, "Already playing");
        return false;
    }
    if (!isBlocking)
    {
        return _audio.connecttospeech(text, lang);
    }
    else
    {
        if (!_audio.connecttospeech(text, lang))
        {
            return false;
        }
        while (_audio.isRunning())
        {
            _audio.loop();
            delay(1);
        }
        return true;
    }
}

bool Music::startStream(const char *url, bool stopCurrentAudio)
{
    if (_audio.isRunning() && !stopCurrentAudio)
    {
        ESP_LOGW(TAG, "Already playing");
        return false;
    }
    return _audio.connecttohost(url);
}

bool Music::stopStream()
{
    if (!_audio.isRunning())
    {
        ESP_LOGW(TAG, "Not playing");
        return false;
    }
    return _audio.stopSong();
}

void Music::update()
{
    _audio.loop();
}

bool Music::increaseVolume()
{
#ifdef ESP32_AUDIO_KIT
    uint8_t new_volume = _dac.getOutputVolume() + 1;
    _dac.setOutputVolume(ES8388::OutSel::OUTALL, new_volume);
    return new_volume == _dac.getOutputVolume();
#else
    uint8_t new_volume = _audio.getVolume() + 1;
    _audio.setVolume(new_volume);
    return new_volume == _audio.getVolume();
#endif
}

bool Music::decreaseVolume()
{
#ifdef ESP32_AUDIO_KIT
    uint8_t new_volume = _dac.getOutputVolume() > 0 ? dac.getOutputVolume() - 1 : 0;
    _dac.setOutputVolume(ES8388::OutSel::OUTALL, new_volume);
    return new_volume == _dac.getOutputVolume();
#else
    uint8_t new_volume = _audio.getVolume() > 0 ? _audio.getVolume() - 1 : 0;
    _audio.setVolume(new_volume);
    return new_volume == _audio.getVolume();
#endif
}

// optional
void audio_info(const char *info)
{
    Serial.print("info        ");
    Serial.println(info);
}
void audio_id3data(const char *info)
{ // id3 metadata
    Serial.print("id3data     ");
    Serial.println(info);
}
void audio_eof_mp3(const char *info)
{ // end of file
    Serial.print("eof_mp3     ");
    Serial.println(info);
}
void audio_showstation(const char *info)
{
    Serial.print("station     ");
    Serial.println(info);
}
void audio_showstreamtitle(const char *info)
{
    Serial.print("streamtitle ");
    Serial.println(info);
}
void audio_bitrate(const char *info)
{
    Serial.print("bitrate     ");
    Serial.println(info);
}
void audio_commercial(const char *info)
{ // duration in sec
    Serial.print("commercial  ");
    Serial.println(info);
}
void audio_icyurl(const char *info)
{ // homepage
    Serial.print("icyurl      ");
    Serial.println(info);
}
void audio_lasthost(const char *info)
{ // stream URL played
    Serial.print("lasthost    ");
    Serial.println(info);
}
void audio_eof_speech(const char *info)
{
    ESP_LOGI(TAG, "End of speech: %s", info);
}