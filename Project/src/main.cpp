#include <Arduino.h>
#include <M5Core2.h>
#include "EJ_DCMotor.h"

const size_t MAX_MOTOR_NUM = 4;
const uint8_t M1_P1 = 13;
const uint8_t M1_P2 = 14;

typedef enum {
  MOTOR1
} MotorId;

EJ_DCMotor_Manager* motorManager = NULL;
EJ_DCMotor* motor = NULL;
EJ_DCMotor* motorAlias = NULL;

void setup() {
  M5.begin();
  motorManager = EJ_DCMotor_Manager::getInstance(MAX_MOTOR_NUM);
  motor = motorManager->createMotor(M1_P1, M1_P2, MOTOR1);
  motorAlias = motorManager->getMotor(MOTOR1);
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