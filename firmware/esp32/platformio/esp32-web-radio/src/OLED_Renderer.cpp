#include "OLED_Renderer.h"

static const char *TAG = "OLED_Renderer";

bool OLED_Renderer::init()
{
    display.display();
    delay(1000);
    display.clearDisplay(); // clears the screen and buffer
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(WHITE, BLACK); // 'normal' text
    display.cp437(true);
    display.setTextWrap(true); // Doesn't work, so we'll do it manually
    display.display();

    return true;
}

void OLED_Renderer::clear() const
{
    display.clearDisplay();
    display.display();
}

void OLED_Renderer::render(Menu const &menu) const
{
    int active_item = 0;

    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    // Get active menu item
    for (int i = 0; i < menu.get_num_components(); i++)
    {
        MenuComponent const *cp_m_comp = menu.get_menu_component(i);
        if (cp_m_comp->is_current())
        {
            active_item = i;
            break;
        }
    }
    int start, end;
    if (active_item < (MENU_ROWS >> 1))
    {
        // Handling start of the list
        start = 0;
        end = MENU_ROWS - 1;
    }
    else if (active_item > menu.get_num_components() - 1 - (MENU_ROWS >> 1))
    {
        // Handling end of the list
        start = menu.get_num_components() - MENU_ROWS;
        end = menu.get_num_components() - 1;
    }
    else
    {
        // Handling middle of the list
        start = active_item - (MENU_ROWS >> 1);
        end = active_item + (MENU_ROWS >> 1);
    }

    for (int i = start; i <= end; i++)
    {
        MenuComponent const *cp_m_comp = menu.get_menu_component(i);

        if (cp_m_comp->is_current())
        {
            display.setTextColor(BLACK, WHITE); // 'inverted' text
        }
        else
        {
            display.setTextColor(WHITE, BLACK); // 'normal' text
        }
        // Printing
        cp_m_comp->render(*this);
    }
    display.display();
}

void OLED_Renderer::render_menu_item(MenuItem const &menu_item) const
{
    display.print(menu_item.get_name());

    // Padding for inverted text
    int len = MENU_COLS - strlen(menu_item.get_name());
    if (len > 0)
    {
        for (int i = 0; i < len - 1; i++)
        {
            display.print(" ");
        }
    }

    display.println();
}

void OLED_Renderer::render_back_menu_item(BackMenuItem const &menu_item) const
{
}

void OLED_Renderer::render_numeric_menu_item(NumericMenuItem const &menu_item) const
{
}

void OLED_Renderer::render_menu(Menu const &menu) const
{
}

void OLED_Renderer::screenSaver()
{
    display.clearDisplay();
    display.display();
}

/**
 * @brief Prepare the OLED display to render a channel, but does not display it, so you can add more info
 *
 * @param channel
 */
void OLED_Renderer::render_channel(const String &channel)
{
    display.clearDisplay(); // clears the screen and buffer
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(WHITE, BLACK); // 'normal' text
    display.println(channel);
    display.drawFastHLine(0, 10, 128, WHITE);
    display.println();
}

void OLED_Renderer::render_volume(uint8_t volume, uint8_t maxVolume, String &channelName)
{
    render_channel(channelName);

    display.println("Volume: ");
    for (int i = 0; i < maxVolume; i++)
    {
        if (i <= volume)
        {
            display.print('\xDB');
        }
        else
        {
            display.print('\xB0');
        }
    }
    display.display();
}