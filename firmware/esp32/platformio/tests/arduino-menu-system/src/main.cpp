/*
 * current_menu.ino - Example code using the menu system library.
 *
 * This example shows all items in the current menu.
 *
 * Copyright (c) 2015 arduino-menusystem
 * Licensed under the MIT license (see LICENSE)
 */

#include <MenuSystem.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1305.h>

static const char *TAG = "main";
TwoWire i2c = TwoWire(1);
const int OLED_RESET = 5;
Adafruit_SSD1305 display(128, 64, &i2c, OLED_RESET);
const char* strings[] = {"Hello", "World", "!", "I", "am", "a", "SSD1305", "display", "driver", "for", "ESP32"};

// renderer
int active_item = 0;

class MyRenderer : public MenuComponentRenderer
{

public:
  void render(Menu const &menu) const
  {
    Serial.println("");
    //Get active menu item
    for (int i = 0; i < menu.get_num_components(); i++)
    {
      MenuComponent const *cp_m_comp = menu.get_menu_component(i);
      if(cp_m_comp->is_current())
      {
        active_item = i;
        break;
      }
    }
    ESP_LOGI(TAG, "Active item: %d\r\n", active_item);
    int start, end;
    if(active_item < 2)
    {
      start = 0;
      end = 4;
    }
    else if(active_item > menu.get_num_components() - 3)
    {
      start = menu.get_num_components() - 5;
      end = menu.get_num_components() - 1;
    }
    else
    {
      start = active_item - 2;
      end = active_item + 2;
    }

    for (int i = start; i <= end; i++)
    {
      MenuComponent const *cp_m_comp = menu.get_menu_component(i);
      cp_m_comp->render(*this);

      if (cp_m_comp->is_current())
        Serial.print("<<< ");
      Serial.println("");
    }
  }

  void render_menu_item(MenuItem const &menu_item) const
  {
    Serial.print(menu_item.get_name());
  }

  void render_back_menu_item(BackMenuItem const &menu_item) const
  {
    Serial.print(menu_item.get_name());
  }

  void render_numeric_menu_item(NumericMenuItem const &menu_item) const
  {
    Serial.print(menu_item.get_name());
  }

  void render_menu(Menu const &menu) const
  {
    Serial.print(menu.get_name());
  }
};
MyRenderer my_renderer;

// forward declarations

void on_item_selected(MenuComponent *p_menu_component);

// Menu variables

MenuSystem ms(my_renderer);
MenuItem* mm_mi[sizeof(strings)/sizeof(strings[0])];

// Menu callback function

void on_item_selected(MenuComponent *p_menu_component)
{
  ESP_LOGI(TAG, "Item %d Selected", active_item);
}


void setup()
{
  Serial.begin(115200);
  ESP_LOGI(TAG, "\r\nBuild %s, utc: %lu\r\n", COMMIT_HASH, CURRENT_TIME);
  i2c.begin(18, 23, 100000);
  Serial.println("SSD1305 OLED test");

  for(int i=0;i<sizeof(strings)/sizeof(strings[0]);i++){
    mm_mi[i] = new MenuItem(strings[i], &on_item_selected);
    ms.get_root_menu().add_item(mm_mi[i]);
  }

  if (!display.begin(0x3C))
  {
    Serial.println("Unable to initialize OLED");
    while (1)
      yield();
  }

  // init done
  display.display(); // show splashscreen
  delay(1000);
  display.clearDisplay(); // clears the screen and buffer
}

void loop()
{
  ms.display();

  // Simulate using the menu by walking over the entire structure.
  ms.select();
  ms.next();

  delay(2000);
}
