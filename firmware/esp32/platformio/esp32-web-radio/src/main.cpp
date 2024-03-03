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
#include "Bounce2.h"

static const uint8_t SSD1305_ADDR = 0x3C;
static const char *STREAMS_FILE = "/streams.json";
static const char *NEWS_STREAM = "VRT NWS";
static const int DEFAULT_SCREEN_TIMEOUT = 240000;
static const int VOLUME_SCREEN_TIMEOUT = 2000;
static const int CHANNEL_SCREEN_TIMEOUT = 5000;

void showstreamtitle(const String &artist, const String &song_title); // non-static, must be visible to Music.cpp
static void onChannelSelected(const String &name);

static Music musicPlayer;
static WiFiMulti wifiMulti;
static StreamDB streamDB;
static RotaryEncoder volumeKnob(new Encoder(PIN_ENC1_S1, PIN_ENC1_S2), PIN_ENC1_KEY);
static RotaryEncoder channelKnob(new Encoder(PIN_ENC2_S1, PIN_ENC2_S2), PIN_ENC2_KEY);
static Adafruit_SSD1305 display(128, 64, &Wire, -1);
static OLED_Renderer renderer(display);
static ChannelMenu channelMenu(&renderer, &channelKnob, onChannelSelected);
static AsyncDelay screenTimeout;
static bool personDetected = true; // to be replaced by PIR sensor
static String last_artist = "", last_song_title = "";
static Bounce newsButton = Bounce();

enum class ScreenActions
{
    SLEEP,
    SHOW_VOLUME,
    SHOW_CHANNEL,
    SHOW_SONG,
    SHOW_NEWS
};

static ScreenActions screen_state = ScreenActions::SLEEP;

enum class MusicActions
{
    NONE,
    PLAY_NEWS,
    PLAY_CHANNEL,
    DEC_VOLUME,
    INC_VOLUME,
};

static MusicActions music_state = MusicActions::NONE;

void show(ScreenActions action)
{
    String selectedChannel;

    switch (action)
    {
    case ScreenActions::SLEEP:
        renderer.clear();
        break;
    case ScreenActions::SHOW_VOLUME:
        screenTimeout.start(VOLUME_SCREEN_TIMEOUT, AsyncDelay::MILLIS);
        if (streamDB.getCurrentStream(selectedChannel))
        {
            renderer.render_volume(musicPlayer.getVolume(), musicPlayer.getMaxValue(), selectedChannel);
        }
        else
        {
            ESP_LOGE(, "No current stream");
            renderer.clear();
        }
        break;
    case ScreenActions::SHOW_CHANNEL:
        screenTimeout.start(CHANNEL_SCREEN_TIMEOUT, AsyncDelay::MILLIS);
        // Menu rendered automatically by channelMenu
        break;
    case ScreenActions::SHOW_SONG:
        screenTimeout.start(DEFAULT_SCREEN_TIMEOUT, AsyncDelay::MILLIS);
        if (streamDB.getCurrentStream(selectedChannel))
        {
            renderer.render_song(selectedChannel, last_artist, last_song_title);
        }
        else
        {
            ESP_LOGE(, "No current stream");
            renderer.clear();
        }
        break;
    case ScreenActions::SHOW_NEWS:
        screenTimeout.start(DEFAULT_SCREEN_TIMEOUT, AsyncDelay::MILLIS);
        renderer.render_song(NEWS_STREAM, "", "");
        break;
    }
    screen_state = action;
}

void music(MusicActions action)
{
    String selectedChannel = "";
    String url = "";
    uint8_t volume = 0;

    switch (action)
    {
    case MusicActions::NONE:
        break;
    case MusicActions::PLAY_NEWS:
        streamDB.setCurrentStream(NEWS_STREAM);
        streamDB.getVolume(NEWS_STREAM, volume);
        streamDB.getStream(NEWS_STREAM, url);
        musicPlayer.startStream(url.c_str(), volume);
        break;
    case MusicActions::PLAY_CHANNEL:
        if (streamDB.getCurrentStream(selectedChannel))
        {
            ESP_LOGI(, "Selected channel: %s", selectedChannel.c_str());
            streamDB.getVolume(selectedChannel, volume);
            streamDB.getStream(selectedChannel, url);
        }
        musicPlayer.startStream(url.c_str(), volume);
        break;
    case MusicActions::DEC_VOLUME:
        musicPlayer.decreaseVolume();
        streamDB.setVolumeCurrentChannel(musicPlayer.getVolume());
        break;
    case MusicActions::INC_VOLUME:
        musicPlayer.increaseVolume();
        streamDB.setVolumeCurrentChannel(musicPlayer.getVolume());
        break;
    }

    music_state = action;
}


void setup()
{
    ESP_LOGI(, "\r\nBuild %s, utc: %lu\r\n", COMMIT_HASH, CURRENT_TIME);
    Wire.setPins(PIN_SDA, PIN_SCL);

    volumeKnob.init();
    channelKnob.init();

    if (!SPIFFS.begin())
    {
        ESP_LOGE(, "Cannot mount SPIFFS volume...be sure to upload Filesystem Image before uploading the sketch");
        while (1)
            ;
    }
    streamDB.open(STREAMS_FILE);
    for (int i = 0; i < streamDB.size(); i++)
    {
        String name;
        if (streamDB.getName(i, name) && name.compareTo(NEWS_STREAM) != 0)
        // Don't add VRT NWS to the menu, it's a news channel, which only lasts for a few minutes
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
        ESP_LOGI(,"WiFi status: %d\r\n", (int)stat);
        delay(100);
    }

    if (!musicPlayer.init(I2S_BCLK, I2S_LRC, I2S_DOUT))
    {
        ESP_LOGE(, "Error initializing music player");
    }
    // musicPlayer.playSpeech("Hallo Marison, leuk dat je weer naar me wil luisteren. Hihi", "nl"); // Google TTS

    if (!renderer.init(display.begin(SSD1305_ADDR, false)))
    {
        ESP_LOGE(, "Unable to initialize OLED");
    }

    String selectedChannel;
    if (streamDB.getCurrentStream(selectedChannel))
    {
        ESP_LOGI(, "Resuming stream: %s", selectedChannel);
        onChannelSelected(selectedChannel.c_str());
    }

    screenTimeout.start(DEFAULT_SCREEN_TIMEOUT, AsyncDelay::MILLIS);

    newsButton.attach(PIN_NEWS_BTN, INPUT_PULLUP);
    newsButton.interval(5);
}

void loop()
{

    musicPlayer.update(); // play audio
    if (channelMenu.loop())
    {
        // refresh timeout as long as the user is selecting a channel
        show(ScreenActions::SHOW_CHANNEL);
    }

    newsButton.update();
    if (newsButton.fell() && music_state != MusicActions::PLAY_NEWS)
    {
        music(MusicActions::PLAY_NEWS);
        show(ScreenActions::SHOW_NEWS);
    }
    if (music_state == MusicActions::PLAY_NEWS && !musicPlayer.isPlaying())
    {
        ESP_LOGI(, "News finished");
        streamDB.restoreLastStream();
        last_artist = "";
        last_song_title = "";
        music(MusicActions::PLAY_CHANNEL);
        show(ScreenActions::SHOW_SONG);
    }

    switch (volumeKnob.rotary_encoder_update())
    {
    case RotaryEncoder::TURN_DOWN:
        music(MusicActions::DEC_VOLUME);
        show(ScreenActions::SHOW_VOLUME);
        break;
    case RotaryEncoder::TURN_UP:
        music(MusicActions::INC_VOLUME);
        show(ScreenActions::SHOW_VOLUME);
        break;
    case RotaryEncoder::BUTTON_FELL:
        show(ScreenActions::SLEEP);
        musicPlayer.stopStream();
        musicPlayer.playSpeech("Goodbye", "en"); // Google TTS
        for (int i = 0; i < 10; i++)
        {
            if ((streamDB.save(STREAMS_FILE)))
            {
                ESP_LOGI(, "Stream database saved");
                break;
            }
        }
        while (musicPlayer.isPlaying())
        {
            musicPlayer.update();
        }
        ESP_LOGI(, "Going to sleep");
        delay(1000);
        esp_deep_sleep_start();
        break;
    default:
        break;
    }

    if (screenTimeout.isExpired())
    {
        ESP_LOGI(, "Screen timeout");
        screenTimeout.repeat();
        switch (screen_state)
        {
        case ScreenActions::SLEEP:
            break;
        case ScreenActions::SHOW_VOLUME:
        case ScreenActions::SHOW_CHANNEL:
            show(ScreenActions::SHOW_SONG);
            break;
        case ScreenActions::SHOW_SONG:
            break;
        case ScreenActions::SHOW_NEWS:
            break;
        }
    }
}

/**
 * @brief Callback function for channel selection (push rotary encoder of the channel knob)
 *
 * @param name
 */
static void onChannelSelected(const String &name)
{
    ESP_LOGI(, "Setting channel: %s", name.c_str());
    streamDB.setCurrentStream(name);
    last_artist = "";
    last_song_title = "";
    music(MusicActions::PLAY_CHANNEL);
}

/**
 * @brief Callback function for showing the song title and artist
 *
 * @param artist
 * @param song_title
 */
void showstreamtitle(const String &artist, const String &song_title)
{
    last_artist = artist;
    last_song_title = song_title;
    show(ScreenActions::SHOW_SONG);
}
