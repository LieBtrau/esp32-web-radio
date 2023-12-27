#include "OLED_Renderer.h"

void OLED_Renderer::render_song(const char *info)
{
    const char *SPLIT = " - ";
    char artist[40], song_title[40];
    char *splitstart = strstr(info, SPLIT);
    ESP_LOGI(TAG, "Stream info: %s", info);

    display.clearDisplay(); // clears the screen and buffer
    display.setCursor(0, 0);

    if (splitstart != NULL)
    {
        // Get artist
        int artist_length = splitstart - info;
        strncpy(artist, info, min(40, artist_length));
        artist[artist_length] = '\0';
        ESP_LOGI(TAG, "Artist : %s.\r\n", artist);

        print(artist);
        display.println();
        display.println();

        // Get song title
        int song_length = strlen(info) - strlen(artist) - strlen(SPLIT);
        if (song_length > 0)
        {
            strncpy(song_title, splitstart + strlen(SPLIT), min(40, song_length));
            song_title[song_length] = '\0';
            ESP_LOGI(TAG, "Song : %s.\r\n", song_title);
            print(song_title);
        }
    }
    else
    {
        print(info);
    }
    display.display();
    screenTimeout.restart();
}

/**
 * @brief Print a string to the OLED display
 * Split in multiple lines if string is too long
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
        if (current_line_length + strlen(pch) > MENU_COLS)
        {
            display.println();
            current_line_length = 0;
        }
        display.print(pch);
        display.print(' ');
        current_line_length += strlen(pch) + 1;
        pch = strtok(NULL, " ,.-");
    }
    display.display();
}