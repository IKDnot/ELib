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

const size_t MAX_MOTOR_NUM = 4;
const size_t MAX_SERVO_SIZE = 1;
const size_t MAX_TOFUNIT_SIZE = 4;
const size_t MAX_I2CHUBUNIT_SIZE = 1;

const int M5_SHUTDOWN_DELAY = 5000; // ms

const uint8_t SCREEN_X = 0;
const uint8_t SCREEN_Y = 20;
const uint8_t LINE_HEIGHT = 20;

/* GPIO */
EJ_DCMotor* motor = NULL;
EJ_DCMotor* motorAlias = NULL;
EJ_DCMotor* invalidMotor = NULL;
EJ_ServoMotor* servo = NULL;
EJ_ServoMotor* servoAlias = NULL;
EJ_ServoMotor* invalidServo = NULL;

/* I2C Unit */
EJ_ToFUnit* tof[MAX_TOFUNIT_SIZE] = {NULL};
EJ_I2CHub* hub = NULL;

MPU6886 imu;

void setupMotor() {
  if (!EJ_DCMotor_Manager::configure(MAX_MOTOR_NUM)) {
    ERRORLOG();
    delay(M5_SHUTDOWN_DELAY);
    M5.shutdown();
  }

  motor = EJ_DCMotor_Manager::createMotor(MOTOR1);
  if (motor == NULL) {
    /*
    ERRORLOG
        内容：メモリ確保に失敗した
    */
    ERRORLOG();
    delay(M5_SHUTDOWN_DELAY);
    M5.shutdown();
  }

  motorAlias = EJ_DCMotor_Manager::getMotor(MOTOR1.id);
  if (motorAlias == NULL) {
    /*
    ERRORLOG
        内容：メモリ確保に失敗した
    */
    ERRORLOG();
    delay(M5_SHUTDOWN_DELAY);
    M5.shutdown();
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
    delay(M5_SHUTDOWN_DELAY);
    M5.shutdown();
  }

  servo = EJ_ServoMotor_Manager::createServo(SERVO1);
  if (servo == NULL) {
    /*
    ERRORLOG
        内容：メモリ確保に失敗した
    */
    ERRORLOG();
    delay(M5_SHUTDOWN_DELAY);
    M5.shutdown();
  }

  servoAlias = EJ_ServoMotor_Manager::getServo(SERVO1.id);
  if (servoAlias == NULL) {
    /*
    ERRORLOG
        内容：メモリ確保に失敗した
    */
    ERRORLOG();
    delay(M5_SHUTDOWN_DELAY);
    M5.shutdown();
  }

}

void setupI2CHubUnit()
{
  if (!EJ_I2CHub_Manager::configure(MAX_I2CHUBUNIT_SIZE)) {
    ERRORLOG();
    delay(M5_SHUTDOWN_DELAY);
    M5.shutdown();
  }

  hub = EJ_I2CHub_Manager::createI2CHub(0);
  if (hub == NULL) {
    /*
    ERRORLOG
        内容：メモリ確保に失敗した
    */
    ERRORLOG();
    delay(M5_SHUTDOWN_DELAY);
    M5.shutdown();
  }
}

void setupToFUnit() {
  if (!EJ_ToFUnit_Manager::configure(MAX_TOFUNIT_SIZE)) {
    ERRORLOG();
    delay(M5_SHUTDOWN_DELAY);
    M5.shutdown();
  }

  for (uint8_t id=0; id<MAX_TOFUNIT_SIZE; id++) {
    uint8_t returnCode = hub->selectChannel(id);
    if (returnCode != 0x00) M5.shutdown(2); /* setup時点で切り替え失敗する時は再起動しか手がない?  */
    tof[id] = EJ_ToFUnit_Manager::createToFUnit(id);
    if (tof[id] == NULL) {
      /*
      ERRORLOG
          内容：メモリ確保に失敗した
      */
      ERRORLOG();
      M5.Lcd.println(id);
      delay(M5_SHUTDOWN_DELAY);
      M5.shutdown();
    }
  }
}

void setup() {
  { /* 共通初期化 */
    M5.begin();
    M5.Lcd.clear();
    M5.Lcd.setCursor(SCREEN_X, SCREEN_Y);
    M5.Lcd.setTextSize(2);
  }

  { /* ToF初期化 */
    Wire.begin(32, 33);
    setupI2CHubUnit();
    setupToFUnit();
  }

  { /* サーボ初期化 */
    setupServo();
  }

  {
    imu.Init();
  }
}

void loop() {
  { /* ToF テスト*/
    static uint16_t distance[MAX_TOFUNIT_SIZE] = {0};
    for (uint8_t id=0; id<MAX_TOFUNIT_SIZE; id++) {
      hub->selectChannel(id);
      distance[id] = tof[id]->read();
    }
    for (uint8_t id=0; id<MAX_TOFUNIT_SIZE; id++) {
      M5.Lcd.setCursor(SCREEN_X, SCREEN_Y + LINE_HEIGHT * id);
      M5.Lcd.printf("CH%d: %dmm    ", id, distance[id]);
    }
    delay(10);
  }

  float pitch, roll, yaw;
  { /* IMU テスト*/
    imu.getAhrsData(&pitch, &roll, &yaw);
    M5.Lcd.setCursor(SCREEN_X, SCREEN_Y + LINE_HEIGHT * 4);
    M5.Lcd.printf("Pitch: %f\nRoll: %f\nYaw: %f", pitch, roll, yaw);
  }

  { /* サーボテスト */
    int angle = abs(int(pitch));
    servo->write(angle);
  }
}