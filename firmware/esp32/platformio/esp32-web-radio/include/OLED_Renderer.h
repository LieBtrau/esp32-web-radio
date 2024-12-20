#pragma once

#include "MenuSystem.h"
#include <Adafruit_SSD1305.h>
#include "AsyncDelay.h"

class OLED_Renderer : public MenuComponentRenderer
{

public:
    OLED_Renderer(Adafruit_SSD1305 &display) : display(display) {}
    bool init(bool displayInitialized);
    void clear() const;
    void render(Menu const &menu) const;
    void render_menu_item(MenuItem const &menu_item) const;
    void render_back_menu_item(BackMenuItem const &menu_item) const;
    void render_numeric_menu_item(NumericMenuItem const &menu_item) const;
    void render_menu(Menu const &menu) const;
    void render_volume(uint8_t volume, uint8_t maxVolume, String &channelName);
    void render_song(const String &channel, const String &artist, const String &song_title);
    void screenSaver();

private:
    void render_channel(const String &channel);
    const int MENU_ROWS = 5;
    const int MENU_COLS = 21;
    void print(const char *str) const;
    Adafruit_SSD1305 &display;
    bool _displayInitialized = false;
};
