#include "Button.h"
#include "CountDown.h"
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include "Processor.h"
#include "Shell.h"
#include "TimerInterrupt_Generic.h"
#include "DeviceState.h"
#include "DeviceScenes.h"

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

DeviceState state = DeviceState::Startup;
Adafruit_NeoPixel _strip(PixelCount, LedOutputPin, NEO_GRBW + NEO_KHZ800);

void SetColor(const Color &c) { pixelRing.SetEffect(macIdleAll, &c); }
void SetScene(DeviceState s) {
    const Scene & scn = Scenes.at(s);
    pixelRing.SetEffect(scn.Effect, &scn.color);
}
void cyclicInterruptRoutine() {
    ledTick.Tick();
    buttonTick.Tick();
}

Shell shell(SerialUSB, SetColor, SetScene);

void setup() {
    pinMode(PC13, OUTPUT);
    pinMode(LedOutputPin, OUTPUT);

    SerialUSB.begin();
    SerialUSB.println("Setup is finished");

    if (!ITimer.attachInterruptInterval(HwTimerInterval_us, cyclicInterruptRoutine))
        SerialUSB.println(F("Can't set ITimer. Select another freq. or timer"));

    pixelRing.SetEffect(macStartIdleAll, &CWhite);
}

int readButtonPin(void) { return digitalRead(ButtonPin); }

void setDebugPin(int value) { digitalWrite(DebugPin, value); }

void loop() {
    static const Color *pColorOverride = nullptr;
    if (SerialUSB == true) {
        if (state == DeviceState::Startup) {
            shell.PrintWelcome();
            state = DeviceState::Connected;
            const Scene & scn = Scenes.at(state);
            pixelRing.SetEffect(scn.Effect, &scn.color);
        }
    } else {
        // if (state != DeviceState::Startup) {
            state = DeviceState::Startup;
            pixelRing.SetEffect(macStartIdleAll, &CWhite);
        // }
    }

    if (SerialUSB) {
        if (SerialUSB.available()) {
            auto c = SerialUSB.read();
            SerialUSB.print((char)c);
            shell.ConsumeSymbol(c);
        }
    }

    if (buttonTick.GetVolatileFlag()) {
        switch (Btn.Eval()) {
        case Button::State::Pressed:
            SerialUSB.println("Btn:L->H");
            pColorOverride = &CRed;
            break;

        case Button::State::Released:
            SerialUSB.println("Btn:H->L");
            pColorOverride = nullptr;
            break;

        default: break;
        }
    }

    if (ledTick.GetVolatileFlag()) {
        setDebugPin(HIGH);
        auto colors = pixelRing.Tick();
        if (pColorOverride != nullptr) {
            auto c = pColorOverride->GetColor();
            for (size_t i = 0; i < PixelCount; i++) {
                _strip.setPixelColor(i, c.red >> 2, c.green >> 2, c.blue >> 2, 0);
            }
        } else {
            for (size_t i = 0; i < PixelCount; i++) {
                auto c = colors[i].GetColor();
                _strip.setPixelColor(i, c.red >> 2, c.green >> 2, c.blue >> 2, 0);
            }
        }
        _strip.show();
        setDebugPin(LOW);
    }
}
