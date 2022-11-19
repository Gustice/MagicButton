#include "Button.h"
#include "CountDown.h"
#include "DeviceScenes.h"
#include "DeviceState.h"
#include "Processor.h"
#include "Shell.h"
#include "TimerInterrupt_Generic.h"
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

using namespace Effect;

/* Constants */
constexpr int LedOutputPin = PB8;
constexpr int ButtonPin = PB9;
constexpr int DebugPin = PB12;
constexpr unsigned HwTimerInterval_us(1 * 1000);
constexpr unsigned PixelCount = 16;

/* Forward declarations */
int readButtonPin(void);
void SetColor(const Color &color);
void SetScene(VisualizationSate scene);
void setDebugPin(int value);
void cyclicInterruptRoutine(void);

STM32Timer _iTimer(TIM1);
static Button _button(readButtonPin, HIGH);
static CountDown _ledTick(40 /* ms*/);
static CountDown _buttonTick(20 /* ms*/);
MultiProcessor<PixelCount> _pixelRing(8);
Adafruit_NeoPixel _strip(PixelCount, LedOutputPin, NEO_GRBW + NEO_KHZ800);
DeviceState _device{
    .Visualization = VisualizationSate::Startup,
    .ButtonState = Button::State::Idle,
};
Shell _shell(SerialUSB, _device, SetColor, SetScene);

void setup() {
    pinMode(PC13, OUTPUT);
    pinMode(LedOutputPin, OUTPUT);

    SerialUSB.begin();
    _shell.PrintEvent("Setup is finished");

    if (!_iTimer.attachInterruptInterval(HwTimerInterval_us, cyclicInterruptRoutine))
        _shell.PrintError("Can't set ITimer. Select another freq. or timer");

    _pixelRing.SetEffect(macStartIdleAll, &CWhite);
}

void loop() {
    static const Color *pColorOverride = nullptr;
    if (SerialUSB == true) {
        if (_device.Visualization == VisualizationSate::Startup) {
            _shell.PrintWelcome();
            _device.Visualization = VisualizationSate::Connected;
            const Scene &scn = Scenes.at(_device.Visualization);
            _pixelRing.SetEffect(scn.Effect, &scn.color);
        }
    } else {
        // if (state != DeviceState::Startup) { // @todo USB not working if this is uncommented
        _device.Visualization = VisualizationSate::Startup;
        _pixelRing.SetEffect(macStartIdleAll, &CWhite);
        // }

        if (SerialUSB.available()) {
            if (SerialUSB.peek() == Shell::EscapeChar)
                _shell.ConsumeSymbol(SerialUSB.read());
                // @todo shall we discard incoming characters in this case?
        }
    }

    if (SerialUSB) {
        if (SerialUSB.available()) {
            _shell.ConsumeSymbol(SerialUSB.read());
        }
    }

    if (_buttonTick.GetVolatileFlag()) {
        _device.ButtonState = _button.Eval();
        switch (_device.ButtonState) {
        case Button::State::Pressed:
            _shell.SendButtonEvent(Button::State::Pressed);
            pColorOverride = &CRed;
            break;

        case Button::State::Released:
            _shell.SendButtonEvent(Button::State::Released);
            pColorOverride = nullptr;
            break;

        default:
            break;
        }
    }

    if (_ledTick.GetVolatileFlag()) {
        setDebugPin(HIGH);
        const Color *colors = _pixelRing.Tick();
        if (pColorOverride != nullptr) {
            Color::Rgbw_t c = pColorOverride->GetColor();
            for (size_t i = 0; i < PixelCount; i++) {
                _strip.setPixelColor(i, c.red >> 2, c.green >> 2, c.blue >> 2, 0);
            }
        } else {
            for (size_t i = 0; i < PixelCount; i++) {
                Color::Rgbw_t c = colors[i].GetColor();
                _strip.setPixelColor(i, c.red >> 2, c.green >> 2, c.blue >> 2, 0);
            }
        }
        _strip.show();
        setDebugPin(LOW);
    }
}

void SetColor(const Color &color) {
    _shell.PrintResponse("Setting Color");
    _pixelRing.SetEffect(macIdleAll, &color);
}
void SetScene(VisualizationSate scene) {
    _shell.PrintResponse("Setting Effect");
    _device.Visualization = scene;
    const Scene &s = Scenes.at(scene);
    _pixelRing.SetEffect(s.Effect, &s.color);
}
void cyclicInterruptRoutine(void) {
    _ledTick.Tick();
    _buttonTick.Tick();
    _shell.Tick();
}

int readButtonPin(void) { return digitalRead(ButtonPin); }
void setDebugPin(int value) { digitalWrite(DebugPin, value); }
