#include "OLED_Renderer.h"

/**
 * @brief 
 * e.g. "Zia Quizon · Jungee Marcelo · Jungee Marcelo - Bakit Hindi Ka Crush Ng Crush Mo?"
 * @param channel 
 * @param artist 
 * @param song_title 
 */
void OLED_Renderer::render_song(const String &channel, const String &artist, const String &song_title)
{
    render_channel(channel);
    print(artist.c_str());
    display.println();
    display.println();
    print(song_title.c_str());

    display.display();
}

/**
 * @brief Print a string to the OLED display
 * Split in multiple lines if string is too long
 *
 * Parsing is done using C string functions
 *
 * @param str   String to print
 */
void OLED_Renderer::print(const char *str) const
{
    char sentence[128];
    char *pch;
    int current_line_length = 0;

    if (strlen(str) > 128)
    {
        ESP_LOGE(TAG, "String too long");
        return;
    }
    strcpy(sentence, str);

    pch = strtok(sentence, " ,.-");
    while (pch != NULL)
    {
        display.print(pch);
        display.print(' ');
        current_line_length += strlen(pch) + 1;
        pch = strtok(NULL, " ,.-");
        if (pch != NULL && current_line_length + strlen(pch) > MENU_COLS)
        {
            display.println();
            current_line_length = 0;
        }
    }
    display.display();
}