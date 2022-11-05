#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

unsigned PixelCount = 16u;
int LedOutputPin = PB8;
int ButtonPin = PB9;

Adafruit_NeoPixel _strip(PixelCount, LedOutputPin, NEO_RGBW + NEO_KHZ800);

void setup() {
    pinMode(PC13, OUTPUT);
    pinMode(LedOutputPin, OUTPUT);
}

void loop() {
    static unsigned idx;

    if (!(digitalRead(ButtonPin) != 0)) {
        for (unsigned i = 0; i < PixelCount; i++) {
            if (i == idx) {
                _strip.setPixelColor(i, 8, 0, 0, 0);
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
    delay(500);
}