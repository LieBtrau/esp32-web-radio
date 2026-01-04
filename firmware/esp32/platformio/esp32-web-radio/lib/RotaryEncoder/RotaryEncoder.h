#pragma once

#include <ESP32Encoder.h>
#include <Bounce2.h>

class RotaryEncoder
{
public:
    typedef enum
    {
        TURN_DOWN,
        NO_TURN,
        TURN_UP,
        BUTTON_FELL
    } ROTARY_ENCODER_STATE;
    RotaryEncoder(const int encPin1, const int encPin2, const int switchPin = -1);
    ~RotaryEncoder();
    bool init();
    ROTARY_ENCODER_STATE rotary_encoder_update();
private:
    static const int MIN_VALID_PULSE = 4;
    ESP32Encoder* _myEncoder = nullptr;
    int _pulse_ctr = 0;
    long _oldPosition = 0;
    Bounce _momentarySwitch;
    int _switchPin;
    int _encPin1;
    int _encPin2;
};
