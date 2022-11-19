// #include "Shell.h"
// #include <Arduino.h>

// // #include "mocks/CdcDummy.h"
// #include <unity.h>

// void setUp(void) {}
// void tearDown(void) {}

// // CdcDummy serial;
// DeviceState _device{
//     .Visualization = VisualizationSate::Startup,
//     .ButtonState = Button::State::Idle,
// };

// void SetColor(const Color &color) {
//     // _shell.PrintResponse("Setting Color");
//     // _pixelRing.SetEffect(macIdleAll, &color);
// }
// void SetScene(VisualizationSate scene) {
//     // _shell.PrintResponse("Setting Effect");
//     // const Scene &s = Scenes.at(scene);
//     // _pixelRing.SetEffect(s.Effect, &s.color);
// }

// // Shell _shell(serial, _device, SetColor, SetScene);

// void setup() {
//     SerialUSB.begin();
// }

// void test_dummy(void)
// {
//     // _shell.PrintWelcome();
//     // _shell.SendButtonEvent(Button::State::Pressed);
//     TEST_ASSERT_EQUAL(1, 1);
// }

// void loop() {
//     UNITY_BEGIN();
//     RUN_TEST(test_dummy);
//     UNITY_END(); // stop unit testing

//     while (1)
//     {
//         // if (SerialUSB.peek() == Shell::EscapeChar)
//         //     _shell.ConsumeSymbol(SerialUSB.read());
//     }
// }

#include <Arduino.h>
#include <unity.h>

extern "C" {
int main();
void setUp(void);
void setup();
void loop();
}

void setUp(void) {}

void tearDown(void) {}

void test_led_builtin_pin_number(void) {}

void test_led_state_high(void) { TEST_ASSERT_EQUAL(1, 1); }

void test_led_state_low(void) { TEST_ASSERT_EQUAL(1, 1); }

void setup() {
    pinMode(PC13, OUTPUT);
    SerialUSB.begin();

    while (!SerialUSB) { delay(1); }
}

void loop() {
    UNITY_BEGIN();
    RUN_TEST(test_led_builtin_pin_number);
    for (unsigned int i = 0; i < 5; i++) {
        RUN_TEST(test_led_state_high);
        delay(10);
        RUN_TEST(test_led_state_low);
        delay(10);
    }
    UNITY_END(); // stop unit testing

    while (1) {
    }
}
