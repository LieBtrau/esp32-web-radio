#include <Arduino.h>
#include "RotaryEncoder.h"

const uint8_t PIN_S1 = 36;
const uint8_t PIN_S2 = 39;

static const char *TAG = "main";
static Encoder encoder1(PIN_S1, PIN_S2);
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