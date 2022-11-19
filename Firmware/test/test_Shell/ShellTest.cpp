#include <Arduino.h>
#include <unity.h>
#include "Shell.h"
#include "../mocks/CdcDummy.h"
#include <string>

extern "C" {
int main();
void setUp(void);
void setup();
void loop();
}

CdcDummy serial;
DeviceState _device{
    .Visualization = VisualizationSate::Startup,
    .ButtonState = Button::State::Idle,
};

void SetColor(const Color &color) {
    // _shell.PrintResponse("Setting Color");
    // _pixelRing.SetEffect(macIdleAll, &color);
}
void SetScene(VisualizationSate scene) {
    // _shell.PrintResponse("Setting Effect");
    // const Scene &s = Scenes.at(scene);
    // _pixelRing.SetEffect(s.Effect, &s.color);
}

Shell _shell(serial, _device, SetColor, SetScene);


void SendString(const std::string &send) {
  for (size_t i = 0; i < send.size(); i++) {
    _shell.ConsumeSymbol(send[i]);
  }
  _shell.ConsumeSymbol('\n');
}

void SendStringWithEcho(const std::string &send) {
  static std::string prompt("$ >");
  for (size_t i = 0; i < send.size(); i++) {
    _shell.ConsumeSymbol(send[i]);
  }
  _shell.ConsumeSymbol('\n');
  TEST_ASSERT_EQUAL_STRING(send.c_str(), serial.ReadOutputBuffer().substr(prompt.size()).c_str());
}

void CheckHelloMessage(void) {
  _shell.PrintWelcome();
  TEST_ASSERT_EQUAL_STRING("** Magic Button Shell **", serial.ReadOutputBuffer().c_str());
  TEST_ASSERT_EQUAL_STRING("* Get Help by type ?-Symbol", serial.ReadOutputBuffer().c_str());
  TEST_ASSERT_EQUAL(0, serial.OutQueue.size());
}

void ReadingVersion(void) {
  SendStringWithEcho("Get Version:");
  auto out =  serial.ReadOutputBuffer();
  TEST_ASSERT_EQUAL_STRING("<TestButton T0.5", out.c_str());
}

void setup() {
    pinMode(PC13, OUTPUT);
    SerialUSB.begin();

    while (!SerialUSB) { delay(1); }
}

void loop() {
    UNITY_BEGIN();

    RUN_TEST(CheckHelloMessage);
    RUN_TEST(ReadingVersion);

    UNITY_END(); // stop unit testing

    while (1) {
    }
}

void setUp(void) {}
void tearDown(void) {}
