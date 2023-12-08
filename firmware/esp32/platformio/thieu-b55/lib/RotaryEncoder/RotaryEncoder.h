#pragma once

#include <Encoder.h>    // paulstoffregen/Encoder @ ^1.4.2

class RotaryEncoder
{
public:
    typedef enum
    {
        TURN_DOWN,
        NO_TURN,
        TURN_UP
    } ROTARY_ENCODER_STATE;
    RotaryEncoder(Encoder* aRotaryEncoder);
    ~RotaryEncoder();
    ROTARY_ENCODER_STATE rotary_encoder_update();
private:
    static const int MIN_VALID_PULSE = 4;
    Encoder* _myEncoder = nullptr;
    int _pulse_ctr = 0;
    long _oldPosition = 0;
};
