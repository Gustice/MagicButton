#include "Button.h"
#include "ClountDown.h"
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#define STM32F1
#include "Processor.h"
#include "TimerInterrupt_Generic.h"

using namespace Effect;

#define PIXEL_COUNT 16
int LedOutputPin = PB8;
int ButtonPin = PB9;
int DebugPin = PB12;
const unsigned HwTimerInterval_us(1 * 1000);

int readButtonPin(void);
STM32Timer ITimer(TIM1);
static Button Btn(readButtonPin, HIGH);
static CntDown ledTick(40 /* ms*/);
static CntDown buttonTick(20 /* ms*/);

unsigned PixelCount = PIXEL_COUNT;
MultiProcessor<PIXEL_COUNT> pixelRing(8);

enum DeviceState {
    Startup = 0,
    Connected,
    Processing,
    Good,
    Bad,
};
struct Scene
{
    DeviceState Code;
    const Macro & Effect;
    const Color & color;
};
DeviceState state = DeviceState::Startup;
const Scene Scenes[] {
    {DeviceState::Startup , macStartIdleAll, CWhite},
    {DeviceState::Connected , macIdleAll, CCyan},
    {DeviceState::Processing , macStdRotate, CYellow},
    {DeviceState::Good , macStdPulseAll, CGreen},
    {DeviceState::Bad , macNervousPulseAll, CRed},
};
Adafruit_NeoPixel _strip(PixelCount, LedOutputPin, NEO_GRBW + NEO_KHZ800);

void cyclicInterruptRoutine() {
    ledTick.Tick();
    buttonTick.Tick();
}

void setup() {
    pinMode(PC13, OUTPUT);
    pinMode(LedOutputPin, OUTPUT);

    SerialUSB.begin();
    SerialUSB.println("Setup is finished");

    if (!ITimer.attachInterruptInterval(HwTimerInterval_us, cyclicInterruptRoutine))
        SerialUSB.println(F("Can't set ITimer. Select another freq. or timer"));
}

int readButtonPin(void) { return digitalRead(ButtonPin); }

void setDebugPin(int value) { digitalWrite(DebugPin, value); }

void loop() {
    static const Color * pColorOverride = nullptr;
    if (SerialUSB == true) {
        if (state == DeviceState::Startup) {
            state = DeviceState::Connected;
            pixelRing.SetEffect(Scenes[state].Effect, &Scenes[state].color);
        }
    } else {
        state = DeviceState::Startup;
        pixelRing.SetEffect(Scenes[state].Effect, &Scenes[state].color);
    }

    if (SerialUSB) {

        if (SerialUSB.available()) {
            auto c = SerialUSB.read();
            SerialUSB.println(c);
            if (c == 'r')
                pixelRing.SetEffect(Scenes[DeviceState::Startup].Effect, &CRed);
            else if (c == 'g')
                pixelRing.SetEffect(Scenes[DeviceState::Startup].Effect, &CGreen);
            else if (c == 'b')
                pixelRing.SetEffect(Scenes[DeviceState::Startup].Effect, &CBlue);
            else if (c == 'w')
                pixelRing.SetEffect(Scenes[DeviceState::Startup].Effect, &CWhite);
            else if (c == '1')
                pixelRing.SetEffect(Scenes[DeviceState::Processing].Effect, &Scenes[DeviceState::Processing].color);
            else if (c == '2')
                pixelRing.SetEffect(Scenes[DeviceState::Good].Effect, &Scenes[DeviceState::Good].color);
            else if (c == '3')
                pixelRing.SetEffect(Scenes[DeviceState::Bad].Effect, &Scenes[DeviceState::Bad].color);
        }
    }

    if (buttonTick.GetVolatileFlag()) {
        switch (Btn.Eval()) {
        case Button::State::Pressed:
            //       Light.ChangeScheme(eLedEffects::Led_Process);
            SerialUSB.println("Btn:L->H");
            pColorOverride = &CRed;
            break;

        case Button::State::Holding:
            //   DevIn.u1_buzz = 1;
            break;

        case Button::State::Released:
            //       Light.ChangeScheme(eLedEffects::Led_Stable);
            SerialUSB.println("Btn:H->L");
            pColorOverride = nullptr;
            break;

        case Button::State::Idle:
            //   DevIn.u1_buzz = 0;
            break;
        }
    }

    if (ledTick.GetVolatileFlag()) {
        setDebugPin(HIGH);
        auto colors = pixelRing.Tick();
        if (pColorOverride != nullptr) {
            auto c = pColorOverride->GetColor();
            for (size_t i = 0; i < PixelCount; i++) {
                    _strip.setPixelColor(i, c.red >> 2 , c.green  >> 2, c.blue  >> 2, 0);
                }
        }
        else{
            for (size_t i = 0; i < PixelCount; i++) {
                auto c = colors[i].GetColor();
                _strip.setPixelColor(i, c.red >> 2 , c.green  >> 2, c.blue  >> 2, 0);
            }
        }
        _strip.show();
        setDebugPin(LOW);
    }
}
