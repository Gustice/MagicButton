#include "Shell.h"
#include "../mocks/CdcDummyRaw.h"
#include <Arduino.h>
#include <string>
#include <unity.h>

extern "C" {
int main();
void setUp(void);
void setup();
void loop();
}

CdcDummyRaw serial;
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
    _device.Visualization = s;
}

Shell _shell(serial, _device, SetColor, SetScene);

void SendString(const std::vector<uint8_t> &send) {
    for (size_t i = 0; i < send.size(); i++) {
        _shell.ConsumeSymbol(send[i]);
    }
}

void ReadingVersion(void) {
    SendString({0x01, 0, 0});
    uint8_t expected[7] {0x01, 0, 4, 'T', '0', '.', '5'};
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected, serial.ReadOutputBuffer(), serial.RawBufferLength);

    TEST_ASSERT_EQUAL(0, serial.OutQueue.size()); 
}

void WriteColorOutput(void) { 
  color = Color();
  SetColorTriggered = false;

  auto c1 = color.GetColor();
  TEST_ASSERT_TRUE(c1.red == 0 && c1.green == 0 && c1.blue == 0 && c1.white == 0);

  SendString({0x02, 0, 4, 0xA1, 0xB2, 0xC3, 0xD4});
  c1 = color.GetColor();
  TEST_ASSERT_TRUE(SetColorTriggered);
  TEST_ASSERT_TRUE(c1.red == 0xA1);
  TEST_ASSERT_TRUE(c1.green == 0xB2);
  TEST_ASSERT_TRUE(c1.blue == 0xC3);
  TEST_ASSERT_TRUE(c1.white == 0xD4);

  uint8_t expected[7] {0x02, 1, 4, 0xA1, 0xB2, 0xC3, 0xD4};
  TEST_ASSERT_EQUAL_INT8_ARRAY(expected, serial.ReadOutputBuffer(), serial.RawBufferLength);

  TEST_ASSERT_EQUAL(0, serial.OutQueue.size()); 
}

void WriteEffectOutput(void) {
  scene = VisualizationSate::Idle;
  SetSceneTriggered = false;

  SendString({0x03, 0, 1, 0x04});
  TEST_ASSERT_TRUE(SetSceneTriggered);
  TEST_ASSERT_EQUAL(VisualizationSate::Good, scene);
  
  uint8_t expected[4] {0x03, 1, 1, 0x04};
  TEST_ASSERT_EQUAL_INT8_ARRAY(expected, serial.ReadOutputBuffer(), serial.RawBufferLength);

  TEST_ASSERT_EQUAL(0, serial.OutQueue.size()); 
}

void ReadButtonStatus(void) {
  _device.ButtonState = Button::State::Holding;
  _device.Visualization = VisualizationSate::Processing;

  SendString({0x04, 0, 0});
  uint8_t expected[7] {0x04, 1, 2, 2, 3};
  TEST_ASSERT_EQUAL_INT8_ARRAY(expected, serial.ReadOutputBuffer(), serial.RawBufferLength);

  TEST_ASSERT_EQUAL(0, serial.OutQueue.size()); 
}

void SendButtonEvent(void) {
  _shell.SendButtonEvent(Button::State::Pressed);
  uint8_t expected[7] {0x81, 0x40, 1, 'R'};
  TEST_ASSERT_EQUAL_INT8_ARRAY(expected, serial.ReadOutputBuffer(), serial.RawBufferLength);
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

    // Switch to Raw
    _shell.ConsumeSymbol(_shell.EscapeChar);
    uint8_t expected[7] {0x00, 0, 4, 'M', 'B', 'T', 'N'};
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected, serial.ReadOutputBuffer(), serial.RawBufferLength);

    RUN_TEST(ReadingVersion);
    RUN_TEST(WriteColorOutput);
    RUN_TEST(WriteEffectOutput);
    RUN_TEST(ReadButtonStatus);
    
    RUN_TEST(SendButtonEvent);
    
    UNITY_END(); // stop unit testing

    while (1) {
    }
}

void setUp(void) {}
void tearDown(void) {}
