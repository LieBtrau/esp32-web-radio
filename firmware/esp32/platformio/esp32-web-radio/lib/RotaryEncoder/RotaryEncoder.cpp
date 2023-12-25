#include "RotaryEncoder.h"

RotaryEncoder::RotaryEncoder(Encoder *aRotaryEncoder, int const switchPin) : _myEncoder(aRotaryEncoder), _switchPin(switchPin)
{
}

RotaryEncoder::~RotaryEncoder()
{
}

bool RotaryEncoder::init()
{
    if(_switchPin < 0)
    {
        return true;
    }
    pinMode(_switchPin, INPUT_PULLUP);
    _momentarySwitch.attach(_switchPin);
    _momentarySwitch.interval(5);
    return true;
}

/**
 * Filter rotary encoder to get a usable up/down signal out of it.
 * Four states per notch in the 24steps rotary encoder
 *
 */
RotaryEncoder::ROTARY_ENCODER_STATE RotaryEncoder::rotary_encoder_update()
{
    if(_switchPin >= 0)
    {
        _momentarySwitch.update();
        if(_momentarySwitch.fell())
        {
            return BUTTON_FELL;
        }
    }
    // 2 state-step needed before updating value.  This avoids updating value in internotch states.
    long newPosition = _myEncoder->read() >> 1; // ignore the last bit of the state

    if (newPosition - _oldPosition > 1)
    {
        _oldPosition = newPosition;
        return TURN_UP;
    }
    if (_oldPosition - newPosition > 1)
    {
        _oldPosition = newPosition;
        return TURN_DOWN;
    }
    return NO_TURN;
}