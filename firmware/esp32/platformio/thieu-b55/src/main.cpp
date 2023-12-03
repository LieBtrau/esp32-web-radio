/**
 * @file main.cpp
 *
 * @author Christoph Tack (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-12-02
 *
 * @copyright Copyright (c) 2023
 *
 * AI-Thinker ESP32-A1S Audio Kit:
 *  DAC1 connected to PA, DAC2 connected to headphone jack
 *
 */
#include "Arduino.h"
#include "WiFi.h"
#include "Audio.h"
#include "ES8388.h"
#include "wifi_credentials.h"
#include <Bounce2.h>
#include "RemoteMonitor.h"
#include <WiFiMulti.h>
#include "StreamDB.h"

static const char *TAG = "main";

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

const char *hostName = "esp32-web-radio";

static Audio audio;
static ES8388 dac(I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO);
static RemoteMonitor remoteMonitor(hostName);
static Command set_led;
static void set_led_callback(cmd *c);
static WiFiMulti wifiMulti;
static StreamDB streamDB;
static int streamIndex = 0;

// Instantiate a Bounce object
Bounce debouncer1 = Bounce();

void setup()
{
  Serial.begin(115200);
  ESP_LOGI(TAG, "\r\nBuild %s, utc: %lu\r\n", COMMIT_HASH, CURRENT_TIME);

  // Setup the first button with an internal pull-up :
  pinMode(PIN_KEY2, INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  debouncer1.attach(PIN_KEY2);
  debouncer1.interval(5); // interval in ms

  if (!SPIFFS.begin())
  {
    ESP_LOGE(TAG, "Cannot mount SPIFFS volume...be sure to upload Filesystem Image before uploading the sketch");
    while (1)
      ;
  }
  streamDB.open("/streams.json");

  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  for (int i = 0; i < (sizeof(wifiNetworks) / sizeof(wifiNetworks[0])); i++)
  {
    wifiMulti.addAP(wifiNetworks[i][0], wifiNetworks[i][1]);
  }

  uint8_t stat = WL_DISCONNECTED;
  while (stat != WL_CONNECTED)
  {
    stat = wifiMulti.run();
    Serial.printf("WiFi status: %d\r\n", (int)stat);
    delay(100);
  }

  remoteMonitor.start(HUSARNET_JOINCODE);
  set_led = remoteMonitor.addCommand("set_led", set_led_callback);
  set_led.addPosArg("state");
  while (!dac.init())
  {
    ESP_LOGE(TAG, "Error initializing ES8388 chip");
    delay(1000);
  }
  dac.setOutputVolume(ES8388::OutSel::OUTALL, 5);
  pinMode(PIN_PA_ENABLE, OUTPUT);
  digitalWrite(PIN_PA_ENABLE, HIGH);

  audio.setPinout(PIN_I2S_AUDIO_KIT_BCK, PIN_I2S_AUDIO_KIT_WS, PIN_I2S_AUDIO_KIT_DATA_OUT);
  audio.connecttospeech("Hallo Marison, leuk dat je weer naar me wil luisteren. Hihi", "nl"); // Google TTS
}

void loop()
{
  audio.loop();
  debouncer1.update();
  if (debouncer1.fell())
  {
    Serial.println("Button 1 pressed!");

       char name[64];
       char url[64];

    if (streamDB.getStream(streamIndex, name, url))
    {
      ESP_LOGI(TAG, "Playing stream %s", name);
      audio.connecttohost(url);
      streamIndex++;
      if (streamIndex >= streamDB.size())
      {
        streamIndex = 0;
      }
    }
  }
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

void set_led_callback(cmd *c)
{
  Command cmd(c);

  String state = cmd.getArg("state").getValue();
  Serial.println(state);
}