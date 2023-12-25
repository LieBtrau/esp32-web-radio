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
#include "wifi_credentials.h"

#include "RemoteMonitor.h"
#include "Webserver.h"
#include <WiFiMulti.h>
#include "StreamDB.h"
#include "RotaryEncoder.h"
#include "pins.h"
#include "Music.h"
#include "SPIFFS.h"

static const char *TAG = "main";
static const char *hostName = "esp32-web-radio";

static Music musicPlayer;
// static RemoteMonitor remoteMonitor(hostName);
// static Command set_led;
static Webserver remoteMonitor(hostName);
static void set_led_callback(cmd *c);
static WiFiMulti wifiMulti;
static StreamDB streamDB;
static int streamIndex = 0;
static RotaryEncoder volumeKnob(new Encoder(PIN_ENC1_S1, PIN_ENC1_S2), PIN_ENC1_KEY);
static RotaryEncoder channelKnob(new Encoder(PIN_ENC2_S1, PIN_ENC2_S2), PIN_ENC2_KEY);

void setup()
{
    Serial.begin(115200);
    ESP_LOGI(TAG, "\r\nBuild %s, utc: %lu\r\n", COMMIT_HASH, CURRENT_TIME);

    volumeKnob.init();
    channelKnob.init();

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
    // set_led = remoteMonitor.addCommand("set_led", set_led_callback);
    // set_led.addPosArg("state");

    if (!musicPlayer.init(I2S_BCLK, I2S_LRC, I2S_DOUT))
    {
        ESP_LOGE(TAG, "Error initializing music player");
    }

    musicPlayer.playSpeech("Hallo Marison, leuk dat je weer naar me wil luisteren. Hihi", "nl"); // Google TTS
}

void loop()
{
    String name, url;

    musicPlayer.update();
    switch (volumeKnob.rotary_encoder_update())
    {
    case RotaryEncoder::TURN_DOWN:
        musicPlayer.decreaseVolume();
        break;
    case RotaryEncoder::TURN_UP:
        musicPlayer.increaseVolume();
        break;
    case RotaryEncoder::BUTTON_FELL:
        ESP_LOGI(TAG, "Button 1 pressed!");
        break;
    default:
        break;
    }
    switch (channelKnob.rotary_encoder_update())
    {
    case RotaryEncoder::TURN_DOWN:
        streamIndex = streamIndex == 0 ? streamDB.size() - 1 : streamIndex - 1;
        ESP_LOGI(TAG, "Channel knob turned down: %d", streamIndex);
        break;
    case RotaryEncoder::TURN_UP:
        streamIndex = (streamIndex + 1) % streamDB.size();
        ESP_LOGI(TAG, "Channel knob turned up: %d", streamIndex);
        break;
    case RotaryEncoder::BUTTON_FELL:
        if (streamDB.getStream(streamIndex, name, url))
        {
            ESP_LOGI(TAG, "Playing stream: \"%s\"", name.c_str());
            musicPlayer.startStream(url.c_str());
        }
        break;
    default:
        break;
    }
}

void set_led_callback(cmd *c)
{
    Command cmd(c);

    String state = cmd.getArg("state").getValue();
    Serial.println(state);
}