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
#include <WiFiMulti.h>
#include "StreamDB.h"
#include "RotaryEncoder.h"
#include "pins.h"
#include "Music.h"
#include <Adafruit_SSD1305.h>
#include "OLED_Renderer.h"
#include "ChannelMenu.h"

static const char *TAG = "main";
static const uint8_t SSD1305_ADDR = 0x3C;
static const char *STREAMS_FILE = "/streams.json";

static Music musicPlayer;

static void onChannelSelected(const char *name);

static WiFiMulti wifiMulti;
static StreamDB streamDB;
static RotaryEncoder volumeKnob(new Encoder(PIN_ENC1_S1, PIN_ENC1_S2), PIN_ENC1_KEY);
static RotaryEncoder channelKnob(new Encoder(PIN_ENC2_S1, PIN_ENC2_S2), PIN_ENC2_KEY);
static Adafruit_SSD1305 display(128, 64, &Wire, -1);
static OLED_Renderer renderer(display);
static ChannelMenu channelMenu(&renderer, &channelKnob, onChannelSelected);

static char *selectedChannel = nullptr;

void setup()
{
    ESP_LOGI(TAG, "\r\nBuild %s, utc: %lu\r\n", COMMIT_HASH, CURRENT_TIME);
    Wire.setPins(PIN_SDA, PIN_SCL);

    volumeKnob.init();
    channelKnob.init();

    if (!SPIFFS.begin())
    {
        ESP_LOGE(TAG, "Cannot mount SPIFFS volume...be sure to upload Filesystem Image before uploading the sketch");
        while (1)
            ;
    }
    streamDB.open(STREAMS_FILE);
    for (int i = 0; i < streamDB.size(); i++)
    {
        String name;
        if (streamDB.getName(i, name))
        {
            channelMenu.addMenuItem(name.c_str());
        }
    }

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

    if (!musicPlayer.init(I2S_BCLK, I2S_LRC, I2S_DOUT))
    {
        ESP_LOGE(TAG, "Error initializing music player");
    }
    // musicPlayer.playSpeech("Hallo Marison, leuk dat je weer naar me wil luisteren. Hihi", "nl"); // Google TTS

    if (!display.begin(SSD1305_ADDR, false))
    {
        ESP_LOGE(TAG, "Unable to initialize OLED");
        return;
    }

    renderer.init();
}

void loop()
{
    musicPlayer.update();
    switch (volumeKnob.rotary_encoder_update())
    {
    case RotaryEncoder::TURN_DOWN:
        musicPlayer.decreaseVolume();
        renderer.render_volume(musicPlayer.getVolume(), musicPlayer.getMaxValue());
        streamDB.setVolume(selectedChannel, musicPlayer.getVolume());
        break;
    case RotaryEncoder::TURN_UP:
        musicPlayer.increaseVolume();
        renderer.render_volume(musicPlayer.getVolume(), musicPlayer.getMaxValue());
        streamDB.setVolume(selectedChannel, musicPlayer.getVolume());
        break;
    case RotaryEncoder::BUTTON_FELL:
        musicPlayer.stopStream();
        musicPlayer.playSpeech("Goodbye", "en"); // Google TTS
        if (!streamDB.save(STREAMS_FILE))
        {
            ESP_LOGE(TAG, "Failed to save stream database");
        }
        break;
    default:
        break;
    }
    
    renderer.screenSaver(channelMenu.loop());
}

static void onChannelSelected(const char *name)
{
    String url;
    uint8_t volume;

    ESP_LOGI(TAG, "Selected channel: %s", name);
    if (streamDB.getVolume(name, volume))
    {
        ESP_LOGI(TAG, "Setting volume to: %d", volume);
        musicPlayer.setVolume(volume);
    }
    if (streamDB.getStream(name, url))
    {
        ESP_LOGI(TAG, "Playing stream: %s", url.c_str());
        musicPlayer.startStream(url.c_str());
        selectedChannel = (char *)name;
    }
    else
    {
        ESP_LOGE(TAG, "Stream not found");
    }
}

void audio_showstreamtitle(const char *info)
{
    renderer.render_song(info);
}