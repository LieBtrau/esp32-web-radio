#include "ChannelMenu.h"

static const char *TAG = "ChannelMenu";

static void on_item_selected(MenuComponent *p_menu_component);
static void (*onChannelSelected)(const char*) = nullptr;

ChannelMenu::ChannelMenu(MenuComponentRenderer *my_renderer, RotaryEncoder *my_rotary_encoder, void (*callback)(const char*)) : _ms(*my_renderer), channelKnob(my_rotary_encoder)
{
    onChannelSelected = callback;
}

void ChannelMenu::addMenuItem(const char *name)
{
    _menuItems = (MenuItem **)realloc(_menuItems, (_num_menuItems + 1) * sizeof(MenuItem *));
    if (_menuItems == NULL)
    {
        ESP_LOGE(TAG, "Unable to allocate memory for menu item");
        return;
    }

    //MenuItem doesn't copy the name, so we need to keep it in memory
    _menuNames = (char **)realloc(_menuNames, (_num_menuItems + 1) * sizeof(char *));
    if (_menuNames == NULL)
    {
        ESP_LOGE(TAG, "Unable to allocate memory for menu name");
        return;
    }
    _menuNames[_num_menuItems] = (char *)malloc(strlen(name) + 1);
    if (_menuNames[_num_menuItems] == NULL)
    {
        ESP_LOGE(TAG, "Unable to allocate memory for menu name");
        return;
    }
    strcpy(_menuNames[_num_menuItems], name);
    _menuItems[_num_menuItems] = new MenuItem(_menuNames[_num_menuItems], &on_item_selected);
    _ms.get_root_menu().add_item(_menuItems[_num_menuItems]);
}

ChannelMenu::~ChannelMenu()
{
    for (size_t i = 0; i < _num_menuItems; i++)
    {
        free(_menuNames[i]);
        delete _menuItems[i];
    }
    free(_menuNames);
    free(_menuItems);
}

/**
 * @brief Handle rotary encoder events
 * 
 * @return true Rotary encoder event occurred
 * @return false no rotary encoder event occurred
 */
bool ChannelMenu::loop()
{
    switch (channelKnob->rotary_encoder_update())
    {
    case RotaryEncoder::TURN_DOWN:
        _ms.prev();
        break;
    case RotaryEncoder::TURN_UP:
        _ms.next();
        break;
    case RotaryEncoder::BUTTON_FELL:
        _ms.select();
        break;
    default:
        return false;
    }
    _ms.display();
    return true;
}

void on_item_selected(MenuComponent *p_menu_component)
{
    ESP_LOGI(TAG, "Selected menu item: %s", p_menu_component->get_name());
    if(onChannelSelected != nullptr)
    {
        onChannelSelected(p_menu_component->get_name());
    }
}
