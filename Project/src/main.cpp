#include <Arduino.h>
#include <M5Core2.h>
#include <math.h>
#include "Elib.h"
#include "PinAssign.h"
#ifdef M5_DEBUG
#define ERRORLOG() M5.Lcd.printf("[ERROR] Class:%s, Line:%d\n", _classname, __LINE__)
#else
#define ERRORLOG() ((void)0)
#endif

const char* _classname = "main";

const int M5_SHUTDOWN_DELAY = 5000; // ms

const uint8_t SCREEN_X = 0;
const uint8_t SCREEN_Y = 20;
const uint8_t LINE_HEIGHT = 20;

/* GPIO */
EJ_EncoderMotor* motor = NULL;

void setup() {
  { /* 共通初期化 */
    M5.begin();
    M5.Lcd.clear();
    M5.Lcd.setCursor(SCREEN_X, SCREEN_Y);
    M5.Lcd.setTextSize(2);
  }

  {
    EJ_EncoderMotor_Manager::configure(1);
    motor = EJ_EncoderMotor_Manager::createEncoderMotor(25, 26, 35, 36, -1, 0);
  }
}

void loop() {
  motor->forward();
  delay(2000);
  motor->reverse();
  delay(2000);
  M5.lcd.println(motor->read());
}