#include "Arduino.h"
#include "WiFi.h"
#include "Audio.h"
// #include "FS.h"
#include "ES8388.h"
#include "wifi_credentials.h"

// I2S, pin defs from https://github.com/pschatzmann/arduino-audiokit/blob/main/src/audio_board/ai_thinker_es8388_5.h
#define PIN_I2S_AUDIO_KIT_MCLK 0
#define PIN_I2S_AUDIO_KIT_BCK 27
#define PIN_I2S_AUDIO_KIT_WS 25
#define PIN_I2S_AUDIO_KIT_DATA_OUT 26
#define PIN_I2S_AUDIO_KIT_DATA_IN 35

static Audio audio;
static ES8388 dac;

// I2C, pin defs from https://github.com/pschatzmann/arduino-audiokit/blob/main/src/audio_board/ai_thinker_es8388_5.h
#define I2C_MASTER_NUM I2C_NUM_0 /*!< I2C port number for master dev */
#define I2C_MASTER_SCL_IO 32
#define I2C_MASTER_SDA_IO 33

void setup()
{
  Serial.begin(115200);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  while (!dac.begin(I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO))
  {
    Serial.println("dac verbindinding mislukt");
    delay(1000);
  }
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
    delay(1500);
  audio.setPinout(PIN_I2S_AUDIO_KIT_BCK, PIN_I2S_AUDIO_KIT_WS, PIN_I2S_AUDIO_KIT_DATA_OUT);
  audio.setVolume(21);                                                                                // default 0...21
                                                                                                      //  or alternative
                                                                                                      //  audio.setVolumeSteps(64); // max 255
                                                                                                      //  audio.setVolume(63);
                                                                                                      //
                                                                                                      //  *** radio streams ***
  audio.connecttohost("http://stream.antennethueringen.de/live/aac-64/stream.antennethueringen.de/"); // aac
  //  audio.connecttohost("http://mcrscast.mcr.iol.pt/cidadefm");                                         // mp3
  //  audio.connecttohost("http://www.wdr.de/wdrlive/media/einslive.m3u");                                // m3u
  //  audio.connecttohost("https://stream.srg-ssr.ch/rsp/aacp_48.asx");                                   // asx
  //  audio.connecttohost("http://tuner.classical102.com/listen.pls");                                    // pls
  //  audio.connecttohost("http://stream.radioparadise.com/flac");                                        // flac
  //  audio.connecttohost("http://stream.sing-sing-bis.org:8000/singsingFlac");                           // flac (ogg)
  //  audio.connecttohost("http://s1.knixx.fm:5347/dein_webradio_vbr.opus");                              // opus (ogg)
  //  audio.connecttohost("http://stream2.dancewave.online:8080/dance.ogg");                              // vorbis (ogg)
  //  audio.connecttohost("http://26373.live.streamtheworld.com:3690/XHQQ_FMAAC/HLSTS/playlist.m3u8");    // HLS
  //  audio.connecttohost("http://eldoradolive02.akamaized.net/hls/live/2043453/eldorado/master.m3u8");   // HLS (ts)
  //  *** web files ***
  //  audio.connecttohost("https://github.com/schreibfaul1/ESP32-audioI2S/raw/master/additional_info/Testfiles/Pink-Panther.wav");        // wav
  //  audio.connecttohost("https://github.com/schreibfaul1/ESP32-audioI2S/raw/master/additional_info/Testfiles/Santiano-Wellerman.flac"); // flac
  //  audio.connecttohost("https://github.com/schreibfaul1/ESP32-audioI2S/raw/master/additional_info/Testfiles/Olsen-Banden.mp3");        // mp3
  //  audio.connecttohost("https://github.com/schreibfaul1/ESP32-audioI2S/raw/master/additional_info/Testfiles/Miss-Marple.m4a");         // m4a (aac)
  //  audio.connecttohost("https://github.com/schreibfaul1/ESP32-audioI2S/raw/master/additional_info/Testfiles/Collide.ogg");             // vorbis
  //  audio.connecttohost("https://github.com/schreibfaul1/ESP32-audioI2S/raw/master/additional_info/Testfiles/sample.opus");             // opus
  //  *** local files ***
  //  audio.connecttoFS(SD, "/test.wav");     // SD
  //  audio.connecttoFS(SD_MMC, "/test.wav"); // SD_MMC
  //  audio.connecttoFS(SPIFFS, "/test.wav"); // SPIFFS

  //  audio.connecttospeech("Wenn die Hunde schlafen, kann der Wolf gut Schafe stehlen.", "de"); // Google TTS
}

void loop()
{
  audio.loop();
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
  Serial.print("eof_speech  ");
  Serial.println(info);
}
