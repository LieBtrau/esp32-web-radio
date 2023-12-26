#pragma once

#include "MenuSystem.h"
#include <Adafruit_SSD1305.h>

class OLED_Renderer : public MenuComponentRenderer
{

public:
    OLED_Renderer(Adafruit_SSD1305 &display) : display(display) {}
    bool init();
    void render(Menu const &menu) const;
    void render_menu_item(MenuItem const &menu_item) const;
    void render_back_menu_item(BackMenuItem const &menu_item) const;
    void render_numeric_menu_item(NumericMenuItem const &menu_item) const;
    void render_menu(Menu const &menu) const;
private:
    const int MENU_ROWS = 5;
    const int MENU_COLS = 21;
    Adafruit_SSD1305 &display;
};
