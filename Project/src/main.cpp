#include <Arduino.h>
#include <M5Core2.h>
#include "EJ_DCMotor.h"
#include "EJ_PinAssign.h"

const size_t MAX_MOTOR_NUM = 4;

EJ_DCMotor_Manager* motorManager = NULL;
EJ_DCMotor* motor = NULL;
EJ_DCMotor* motor4 = NULL;
EJ_DCMotor* motorAlias = NULL;

void setup() {
  M5.begin();
  motorManager = EJ_DCMotor_Manager::getInstance(MAX_MOTOR_NUM);
  motor = motorManager->createMotor(MOTOR1.pin1, MOTOR1.pin2, MOTOR1.id);
  motorAlias = motorManager->getMotor(MOTOR1.id);
  motor4 = motorManager->createMotor(1,3,5); // Error 出力テスト用
}

void loop() {
  motor->forward();
  M5.Lcd.println("forward");
  
  delay(1000);
  
  motor->reverse();
  M5.Lcd.println("reverse");

  delay(1000);
  
  motorAlias->stop();
  M5.Lcd.println("stop");

  delay(3000);
}