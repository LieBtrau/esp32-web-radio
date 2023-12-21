#include <Arduino.h>
#include "RotaryEncoder.h"

#define PIN_KEY3 19
#define PIN_IO22 22

static const char *TAG = "main";
static Encoder encoder1(PIN_KEY3, PIN_IO22);
static RotaryEncoder encoder(&encoder1);

void setup()
{
  ESP_LOGI(TAG, "\r\nBuild %s, utc: %lu\r\n", COMMIT_HASH, CURRENT_TIME);
}

int lastpos = 0;
void loop()
{
  switch (encoder.rotary_encoder_update())
  {
  case RotaryEncoder::TURN_DOWN:
    lastpos--;
    ESP_LOGI(TAG, "lastpos: %d", lastpos);
    break;
  case RotaryEncoder::TURN_UP:
    lastpos++;
    ESP_LOGI(TAG, "lastpos: %d", lastpos);
    break;
  default:
    break;
  }
}