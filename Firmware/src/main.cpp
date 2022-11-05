#include "Button.h"
#include "ClountDown.h"
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#define STM32F1
#include "TimerInterrupt_Generic.h"
#include "Processor.h"

using namespace Effect;

unsigned PixelCount = 16u;
int LedOutputPin = PB8;
int ButtonPin = PB9;
int DebugPin = PB12;
const unsigned HwTimerInterval_us(1 * 1000);

int readButtonPin(void);
STM32Timer ITimer(TIM1);
static Button Btn(readButtonPin, HIGH);
static CntDown ledTick(40);
static CntDown buttonTick(20);


Adafruit_NeoPixel _strip(PixelCount, LedOutputPin, NEO_GRBW + NEO_KHZ800);
struct RgbColor {
    u_int8_t R;
    u_int8_t G;
    u_int8_t B;
    u_int8_t W;
};
RgbColor color = {8, 0, 0, 0};

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
    static unsigned idx;

    if (SerialUSB) {
        if (SerialUSB.available()) {
            auto c = SerialUSB.read();
            SerialUSB.println(c);
            if (c == 'r')
                color = {8, 0, 0, 0};
            else if (c == 'g')
                color = {0, 8, 0, 0};
            else if (c == 'b')
                color = {0, 0, 8, 0};
            else if (c == 'w')
                color = {0, 0, 0, 8};
        }
    }

    if (buttonTick.GetVolatileFlag()) {
        switch (Btn.Eval()) {
        case Button::State::Pressed:
            //       Light.ChangeScheme(eLedEffects::Led_Process);
            //   DevIn.u1_buzz = 1;
            //   SER_SendInput(0x01);
            break;

        case Button::State::Holding:
            //   DevIn.u1_buzz = 1;
            break;

        case Button::State::Released:
            //       Light.ChangeScheme(eLedEffects::Led_Stable);
            //   DevIn.u1_buzz = 0;
            //   SER_SendInput(0x00);
            break;

        case Button::State::Idle:
            //   DevIn.u1_buzz = 0;
            break;
        }
    }

    if (ledTick.GetVolatileFlag()) {
        setDebugPin(HIGH);
        if (Btn.GetState() != Button::State::Holding) {
            for (unsigned i = 0; i < PixelCount; i++) {
                if (i == idx) {
                    _strip.setPixelColor(i, color.R, color.G, color.B, color.W);
                } else {
                    _strip.setPixelColor(i, 0, 0, 0, 0);
                }
            }
        } else {
            for (unsigned i = 0; i < PixelCount; i++) {
                _strip.setPixelColor(i, 8, 8, 8, 0);
            }
        }

        if (++idx >= PixelCount)
            idx = 0;

        _strip.show();
        setDebugPin(LOW);
    }
}
