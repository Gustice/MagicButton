#include "Shell.h"
#include "../mocks/CdcDummy.h"
#include <Arduino.h>
#include <string>
#include <unity.h>

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

bool SetColorTriggered = false;
bool SetSceneTriggered = false;
Color color;
VisualizationSate scene;

void SetColor(const Color &c) {
    SetColorTriggered = true;
    color = c;
}
void SetScene(VisualizationSate s) {
    SetSceneTriggered = true;
    scene = s;
}

Shell _shell(serial, _device, SetColor, SetScene);

void SendString(const std::string &send) {
    serial.OutBuffer = ""; // Kill Prompt "$ > "
    for (size_t i = 0; i < send.size(); i++) {
        _shell.ConsumeSymbol(send[i]);
    }
    _shell.ConsumeSymbol('\n');
}

void SendStringWithEcho(const std::string &send) {
    static std::string prompt("$ >");
    serial.OutBuffer = ""; // Kill Prompt "$ > "
    for (size_t i = 0; i < send.size(); i++) {
        _shell.ConsumeSymbol(send[i]);
    }
    _shell.ConsumeSymbol('\n');
    TEST_ASSERT_EQUAL_STRING(send.c_str(), serial.ReadOutputBuffer().c_str());
}

void CheckHelloMessage(void) {
    _shell.PrintWelcome();
    TEST_ASSERT_EQUAL_STRING("** Magic Button Shell **", serial.ReadOutputBuffer().c_str());
    TEST_ASSERT_EQUAL_STRING("* Get Help by type ?-Symbol", serial.ReadOutputBuffer().c_str());
    TEST_ASSERT_EQUAL(0, serial.OutQueue.size());
}

void ReadingVersion(void) {
    SendStringWithEcho("Get Version:");
    auto out = serial.ReadOutputBuffer();
    TEST_ASSERT_EQUAL_STRING("<TestButton T0.5", out.c_str());
}

void WriteShortCuts(void) {
    SetColorTriggered = SetSceneTriggered = false;
    // Arbitrary Color
    SendStringWithEcho("#r");
    TEST_ASSERT_TRUE(SetColorTriggered);
    TEST_ASSERT_FALSE(SetSceneTriggered);
    TEST_ASSERT_EQUAL_STRING("<Shortcut detected", serial.ReadOutputBuffer().c_str());
    SetColorTriggered = SetSceneTriggered = false;

    // Arbitrary Scene
    SendStringWithEcho("#0");
    TEST_ASSERT_FALSE(SetColorTriggered);
    TEST_ASSERT_TRUE(SetSceneTriggered);
    TEST_ASSERT_EQUAL_STRING("<Shortcut detected", serial.ReadOutputBuffer().c_str());
    SetColorTriggered = SetSceneTriggered = false;

    // Invalid Shortcut
    SendStringWithEcho("#@");
    TEST_ASSERT_FALSE(SetColorTriggered);
    TEST_ASSERT_FALSE(SetSceneTriggered);
    TEST_ASSERT_EQUAL_STRING("<Shortcut detected", serial.ReadOutputBuffer().c_str());
    TEST_ASSERT_EQUAL_STRING("!Unknown shortcut", serial.ReadOutputBuffer().c_str());

    TEST_ASSERT_EQUAL(0, serial.OutQueue.size());
}

void WriteColorOutput(void) { 
  color = Color();
  SetColorTriggered = false;

  auto c1 = color.GetColor();
  TEST_ASSERT_TRUE(c1.red == 0 && c1.green == 0 && c1.blue == 0 && c1.white == 0);


  SendStringWithEcho("Set Color:A1,B2,C3,D4");
  c1 = color.GetColor();
  TEST_ASSERT_TRUE(SetColorTriggered);
  TEST_ASSERT_TRUE(c1.red == 0xA1);
  TEST_ASSERT_TRUE(c1.green == 0xB2);
  TEST_ASSERT_TRUE(c1.blue == 0xC3);
  TEST_ASSERT_TRUE(c1.white == 0xD4);

  SetColorTriggered = false;
  color = Color();
  SendStringWithEcho("Set Color:A1");
  TEST_ASSERT_FALSE(SetColorTriggered);
  TEST_ASSERT_EQUAL_STRING("!Four comma-separated color-values expected. Input discared!", serial.ReadOutputBuffer().c_str());

  TEST_ASSERT_EQUAL(0, serial.OutQueue.size()); 
}

void WriteEffectOutput(void) {
  scene = VisualizationSate::Idle;
  SetSceneTriggered = false;

  SendStringWithEcho("Set Effect:Good");
  TEST_ASSERT_TRUE(SetSceneTriggered);
  TEST_ASSERT_EQUAL(VisualizationSate::Good, scene);

  SetSceneTriggered = false;
  SendStringWithEcho("Set Effect:###");
  TEST_ASSERT_FALSE(SetSceneTriggered);
  TEST_ASSERT_EQUAL_STRING("!Unknown Effect", serial.ReadOutputBuffer().c_str());
  
  TEST_ASSERT_EQUAL(0, serial.OutQueue.size()); 
}

void ReadButtonStatus(void) {
  _device.ButtonState = Button::State::Holding;
  _device.Visualization = VisualizationSate::Processing;

  SendStringWithEcho("Get Status:");
  TEST_ASSERT_EQUAL_STRING("<State:Btn=Holding;Effect=Processing;", serial.ReadOutputBuffer().c_str());
  TEST_ASSERT_EQUAL(0, serial.OutQueue.size()); 
}

void ReadVersionSilently(void) {
  SendString("*Get Version:");
  auto out = serial.ReadOutputBuffer();
  TEST_ASSERT_EQUAL_STRING("<TestButton T0.5", out.c_str());
}
void ReadButtonSilently(void) {
  _device.ButtonState = Button::State::Holding;
  _device.Visualization = VisualizationSate::Processing;

  SendString("*Get Status:");
  TEST_ASSERT_EQUAL_STRING("<State:Btn=Holding;Effect=Processing;", serial.ReadOutputBuffer().c_str());
  TEST_ASSERT_EQUAL(0, serial.OutQueue.size()); 
}

void RepeatTestsWithoutEcho(void) {
    SendString("Get Version:");
    TEST_ASSERT_EQUAL_STRING("<TestButton T0.5", serial.ReadOutputBuffer().c_str());

  color = Color();
  scene = VisualizationSate::Idle;

  SetColorTriggered = false;
  SetSceneTriggered = false;

  SendString("Set Color:A1,B2,C3,D4");
  auto c1 = color.GetColor();
  TEST_ASSERT_TRUE(SetColorTriggered);
  TEST_ASSERT_TRUE(c1.red == 0xA1);
  TEST_ASSERT_TRUE(c1.green == 0xB2);
  TEST_ASSERT_TRUE(c1.blue == 0xC3);
  TEST_ASSERT_TRUE(c1.white == 0xD4);

  SendString("Set Effect:Good");
  TEST_ASSERT_TRUE(SetSceneTriggered);
  TEST_ASSERT_EQUAL(VisualizationSate::Good, scene);

  _device.ButtonState = Button::State::Holding;
  _device.Visualization = VisualizationSate::Processing;
  SendString("Get Status:");
  TEST_ASSERT_EQUAL_STRING("<State:Btn=Holding;Effect=Processing;", serial.ReadOutputBuffer().c_str());

  SendString("*Get Version:");
  auto out = serial.ReadOutputBuffer();
  TEST_ASSERT_EQUAL_STRING("<TestButton T0.5", out.c_str());

  TEST_ASSERT_EQUAL(0, serial.OutQueue.size());
}


/**
 * @brief Test-Setup
 */
void setup() {
    pinMode(PC13, OUTPUT);
    SerialUSB.begin();

    while (!SerialUSB) {
        delay(1);
    }
}

void loop() {
    UNITY_BEGIN();

    RUN_TEST(CheckHelloMessage);
    RUN_TEST(ReadingVersion);
    RUN_TEST(WriteShortCuts);
    RUN_TEST(WriteColorOutput);
    RUN_TEST(WriteEffectOutput);
    RUN_TEST(ReadButtonStatus);

    RUN_TEST(ReadVersionSilently);
    RUN_TEST(ReadButtonSilently);

    SendString("*Set Echo:off");
    TEST_ASSERT_EQUAL(0, serial.OutQueue.size()); 

    RUN_TEST(RepeatTestsWithoutEcho);

    UNITY_END(); // stop unit testing

    while (1) {
    }
}

void setUp(void) {}
void tearDown(void) {}
