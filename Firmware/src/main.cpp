#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

unsigned PixelCount = 16u;
int LedOutputPin = PB8;
int ButtonPin = PB9;
int DebugPin = PB12;

Adafruit_NeoPixel _strip(PixelCount, LedOutputPin, NEO_GRBW + NEO_KHZ800);
struct RgbColor {
    u_int8_t R;
    u_int8_t G;
    u_int8_t B;
    u_int8_t W;
};
RgbColor color = {8, 0, 0, 0};

void setup() {
    pinMode(PC13, OUTPUT);
    pinMode(LedOutputPin, OUTPUT);

    SerialUSB.begin();
    SerialUSB.println("Setup is finished");
}

int readButtonPin() { return digitalRead(ButtonPin); }

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

    setDebugPin(HIGH);
    if (!(digitalRead(ButtonPin) != 0)) {
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

    delay(500);
}