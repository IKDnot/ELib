#include <Arduino.h>
#include <M5Core2.h>
#include "Elib.h"
#include "PinAssign.h"
#ifdef M5_DEBUG
#define ERRORLOG() M5.Lcd.printf("[ERROR] Class:%s, Line:%d\n", _classname, __LINE__)
#else
#define ERRORLOG() ((void)0)
#endif

const char* _classname = "main";

const size_t MAX_MOTOR_NUM = 4;
const size_t MAX_SERVO_SIZE = 1;
const size_t MAX_TOFUNIT_SIZE = 3;

EJ_DCMotor* motor = NULL;
EJ_DCMotor* motorAlias = NULL;
EJ_DCMotor* invalidMotor = NULL;

EJ_ServoMotor* servo = NULL;
EJ_ServoMotor* servoAlias = NULL;
EJ_ServoMotor* invalidServo = NULL;

EJ_ToFUnit* tof = NULL;

void setupMotor() {
  if (!EJ_DCMotor_Manager::configure(MAX_MOTOR_NUM)) {
    ERRORLOG();
    M5.shutdown(5);
  }

  motor = EJ_DCMotor_Manager::createMotor(MOTOR1);
  if (motor == NULL) {
    /*
    ERRORLOG
        内容：メモリ確保に失敗した
    */
    ERRORLOG();
    M5.shutdown(5);
  }

  motorAlias = EJ_DCMotor_Manager::getMotor(MOTOR1.id);
  if (motorAlias == NULL) {
    /*
    ERRORLOG
        内容：メモリ確保に失敗した
    */
    ERRORLOG();
    M5.shutdown(5);
  }
  
  invalidMotor = EJ_DCMotor_Manager::createMotor(1,3,5); // Error 出力テスト用
  if (invalidMotor == NULL) {
    /*
    ERRORLOG
        内容：メモリ確保に失敗した
    */
    ERRORLOG();
    delay(2000);
  }
}

void setupServo() {
  if (!EJ_ServoMotor_Manager::configure(MAX_SERVO_SIZE)) {
    ERRORLOG();
    M5.shutdown(5);
  }

  servo = EJ_ServoMotor_Manager::createServo(SERVO1);
  if (servo == NULL) {
    /*
    ERRORLOG
        内容：メモリ確保に失敗した
    */
    ERRORLOG();
    M5.shutdown(5);
  }

  servoAlias = EJ_ServoMotor_Manager::getServo(SERVO1.id);
  if (servoAlias == NULL) {
    /*
    ERRORLOG
        内容：メモリ確保に失敗した
    */
    ERRORLOG();
    M5.shutdown(5);
  }

  invalidServo = EJ_ServoMotor_Manager::createServo(1, 50); // Error 出力テスト用
  if (invalidServo == NULL) {
    /*
    ERRORLOG
        内容：メモリ確保に失敗した
    */
    ERRORLOG();
    delay(2000);
  }
}

void setupToFUnit() {
  if (!EJ_ToFUnit_Manager::configure(MAX_TOFUNIT_SIZE)) {
    ERRORLOG();
    M5.shutdown(5);
  }

  tof = EJ_ToFUnit_Manager::createToFUnit(0);
  if (tof == NULL) {
    /*
    ERRORLOG
        内容：メモリ確保に失敗した
    */
    ERRORLOG();
    M5.shutdown(5);
  }

  M5.Lcd.printf("ADDRESS:%#x\n", tof->getAddress());
}

void setup() {
  M5.begin();
  Wire.begin(32, 33);
  setupToFUnit();
}

void loop() {
  uint16_t millis = tof->read();
  M5.Lcd.printf("%dmm\n", millis);
  delay(1000);
}