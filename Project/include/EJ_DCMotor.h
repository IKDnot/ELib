#ifndef EJDCMOTOR
#define EJDCMOTOR
#include <Arduino.h>

typedef struct
{
    uint8_t pin1;
    uint8_t pin2;
    uint8_t id;
} MotorDef;

/*-------------------------------------------------------------------------------------------------*/

class EJ_DCMotor
{
private:
    EJ_DCMotor(uint8_t pin1 = 0, uint8_t pin2 = 0);
    friend class EJ_DCMotor_Manager;

private:
    void write(uint8_t bit1, uint8_t bit2);

public:
    ~EJ_DCMotor();

public:
    void forward();
    void reverse();
    void stop();

private:
    static const char* _classname;;
    uint8_t _pin1;
    uint8_t _pin2;
};

/*-------------------------------------------------------------------------------------------------*/

class EJ_DCMotor_Manager
{
private:
    EJ_DCMotor_Manager(size_t maxInstanceSize);

public:
    static EJ_DCMotor_Manager* getInstance(size_t maxInstanceSize);
    static EJ_DCMotor_Manager* getInstance();

public:
    ~EJ_DCMotor_Manager();

public:
    EJ_DCMotor *createMotor(MotorDef motor);
    EJ_DCMotor *createMotor(uint8_t pin1, uint8_t pin2, uint8_t id);
    EJ_DCMotor *getMotor(MotorDef motor);
    EJ_DCMotor *getMotor(uint8_t id);

private:
    static const char* _classname;
    static EJ_DCMotor_Manager *_singleton;
    const size_t _maxInstanceSize;
    size_t _numOfInstanceSize;
    EJ_DCMotor **_instanceList;
};

#endif // EJDCMOTOR