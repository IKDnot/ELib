#include <Arduino.h>
#include <M5Core2.h>
#include "EJ_DCMotor.h"
#include "EJ_ServoMotor.h"
#include "EJ_PinAssign.h"

const char* _classname = "main";

const size_t MAX_MOTOR_NUM = 4;
const size_t MAX_SERVO_SIZE = 1;

EJ_DCMotor_Manager* motorManager = NULL;
EJ_DCMotor* motor = NULL;
EJ_DCMotor* motorAlias = NULL;
EJ_DCMotor* invalidMotor = NULL;

EJ_ServoMotor_Manager* servoManager = NULL;
EJ_ServoMotor* servo = NULL;
EJ_ServoMotor* servoAlias = NULL;
EJ_ServoMotor* invalidServo = NULL;

void setup() {
  M5.begin();
  
  /* DCモータテスト */
  motorManager = EJ_DCMotor_Manager::getInstance(MAX_MOTOR_NUM);
  motor = motorManager->createMotor(MOTOR1);
  motorAlias = motorManager->getMotor(MOTOR1.id);
  invalidMotor = motorManager->createMotor(1,3,5); // Error 出力テスト用

  /* Servoモータテスト */
  servoManager = EJ_ServoMotor_Manager::getInstance(MAX_SERVO_SIZE);
  if (servoManager == NULL) {
    /*
    ERRORLOG
        内容：メモリ確保に失敗した
    */
    M5.Lcd.printf("[ERROR] Class:%s, Line:%d\n", _classname, __LINE__);
    delay(2000);
  }

  servo = servoManager->createServo(SERVO1);
  if (servo == NULL) {
    /*
    ERRORLOG
        内容：メモリ確保に失敗した
    */
    M5.Lcd.printf("[ERROR] Class:%s, Line:%d\n", _classname, __LINE__);
    delay(2000);
  }

  servoAlias = servoManager->getServo(SERVO1.id);
  if (servoAlias == NULL) {
    /*
    ERRORLOG
        内容：メモリ確保に失敗した
    */
    M5.Lcd.printf("[ERROR] Class:%s, Line:%d\n", _classname, __LINE__);
    delay(2000);
  }

  invalidServo = servoManager->createServo(1, 50); // Error 出力テスト用
  if (invalidServo == NULL) {
  /*
  ERRORLOG
      内容：メモリ確保に失敗した
  */
  M5.Lcd.printf("[ERROR] Class:%s, Line:%d\n", _classname, __LINE__);
  delay(2000);
  }
}

void loop() {
  static int count = 0;
  M5.Lcd.println(count);

  motor->forward();
  M5.Lcd.println("forward");
  
  delay(1000);
  
  motor->reverse();
  M5.Lcd.println("reverse");

  delay(1000);
  
  motorAlias->stop();
  M5.Lcd.println("stop");

  delay(1000);

  /* servoとservoAliasが同じインスタンスを指していればOK*/
  for (int i=1;i<19;i++) {
      if (i%2 == 0){
        servo->write(i*10);
        M5.Lcd.printf("servo->write(%d)\n",i*10);
      } else {
        servoAlias->write(i*10);
        M5.Lcd.printf("servoAlias->write(%d)\n",i*10);
      }
      delay(1000);
  }
  
  delay(1000);
  count++;
}