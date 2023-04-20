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
EJ_PhotoInterrupter* photo = NULL;

void setup() {
  { /* 共通初期化 */
    M5.begin();
    M5.Lcd.clear();
    M5.Lcd.setCursor(SCREEN_X, SCREEN_Y);
    M5.Lcd.setTextSize(2);
  }

  EJ_PhotoInterrupter_Manager::configure(1);
  photo = EJ_PhotoInterrupter_Manager::createPhotoInterrupter(19,0);
}

void loop() {
  if(photo->isInterrupted())
  {
    M5.Lcd.println("interrupted");
  }
  delay(10);
}