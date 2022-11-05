#pragma once

class Button {
  public:
    enum State {
        Idle,
        Pressed,
        Holding,
        Released,
    };

    typedef int (*GetPinCb)(void);
    Button(GetPinCb fpGetPin, int onCondition) : _fpGetPinCb(fpGetPin), _onCondition(onCondition) {
        int value = _fpGetPinCb() == _onCondition;
        _oldValue = (value & 0x01);
        _state = State::Idle;
    }

    State GetState(void) { return _state; }

    State Eval() {
        int value = _fpGetPinCb() == _onCondition;
        int values = ((_oldValue & 1) << 1) | (value & 1);
        _oldValue = value;

        switch (values) {
        default:
        case 0x00: // Idle / OFF
            _state = Idle;
            break;

        case 0x01: // Press-Event
            _state = Pressed;
            break;

        case 0x03: // Hold
            _state = Holding;
            break;

        case 0x02: // Release-Event
            _state = Released;
            break;
        }
        return (_state);
    }

  private:
    const GetPinCb _fpGetPinCb;
    int _onCondition;
    int _oldValue;
    State _state;
};
