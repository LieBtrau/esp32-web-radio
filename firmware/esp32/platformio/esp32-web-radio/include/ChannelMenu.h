#pragma once
#include <MenuSystem.h>
#include "RotaryEncoder.h"

class ChannelMenu
{
public:
    ChannelMenu(MenuComponentRenderer* my_renderer, RotaryEncoder* my_rotary_encoder, void (*callback)(const char*));
    ~ChannelMenu();
    void addMenuItem(const char *name);
    bool loop();

private:
    MenuSystem _ms;
    MenuItem **_menuItems;
    char **_menuNames;
    size_t _num_menuItems = 0;
    RotaryEncoder* channelKnob;
};