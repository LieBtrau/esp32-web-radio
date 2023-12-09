// Simple I2C test for ebay 128x64 oled.

#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

static const char *TAG = "main";
TwoWire i2c = TwoWire(1);

// Define proper RST_PIN if required.
#define RST_PIN -1

SSD1306AsciiWire oled(i2c);
//------------------------------------------------------------------------------
void setup() {
  ESP_LOGI(TAG, "\r\nBuild %s, utc: %lu\r\n", COMMIT_HASH, CURRENT_TIME);
  i2c.begin(18, 23, 100000);
  ESP_LOGI(TAG, "SSD1305 OLED test");

#if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
#endif // RST_PIN >= 0

  //oled.setFont(Adafruit5x7);
  //oled.setFont(Wendy3x5);
  oled.setFont(TimesNewRoman13);

  uint32_t m = micros();
  oled.clear();
  oled.println("Hello world!");
  oled.println("A long line may be truncated");
  oled.println();
  oled.set2X();
  oled.println("2X demo");
  oled.set1X();
  oled.print("\nmicros: ");
  oled.print(micros() - m);
}
//------------------------------------------------------------------------------
void loop() {}
