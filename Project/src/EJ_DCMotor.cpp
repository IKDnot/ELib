#include "EJ_DCMotor.h"

#ifdef M5CORE2
#include <M5Core2.h>
#elif M5STICKCPLUS
#include <M5StickCPlus.h>
#else
#undef M5_DEBUG
#endif

#ifdef M5_DEBUG
#define ERRORLOG() M5.Lcd.printf("[ERROR] Class:%s, Line:%d\n", _classname, __LINE__)
#else
#define ERRORLOG() ((void)0)
#endif

/*--------------
class EJ_DCMotor
--------------*/

/* static menber */
const char* EJ_DCMotor::_classname = "EJ_DCMotor";

/* private method */
EJ_DCMotor::EJ_DCMotor(uint8_t pin1, uint8_t pin2)
:   _pin1(pin1),
    _pin2(pin2)
{
    pinMode(_pin1, OUTPUT);
    pinMode(_pin2, OUTPUT);
    digitalWrite(_pin1, LOW);
    digitalWrite(_pin2, LOW);
}

void EJ_DCMotor::write(uint8_t bit1, uint8_t bit2)
{
    digitalWrite(_pin1, bit1);
    digitalWrite(_pin2, bit2);
}

/* public method */
EJ_DCMotor::~EJ_DCMotor()
{}

void EJ_DCMotor::forward()
{
    write(HIGH, LOW);
}

void EJ_DCMotor::reverse()
{
    write(LOW, HIGH);
}

void EJ_DCMotor::stop()
{
    write(LOW, LOW);
}

/*----------------------
class EJ_DCMotor_Manager 
----------------------*/

/* static member */
EJ_DCMotor_Manager* EJ_DCMotor_Manager::_singleton = NULL;
const char* EJ_DCMotor_Manager::_classname = "EJ_DCMotor_Manager";

/* private method */
EJ_DCMotor_Manager::EJ_DCMotor_Manager(size_t maxInstanceSize)
:   _maxInstanceSize(maxInstanceSize),
    _instanceList(NULL)
{
    _instanceList = new EJ_DCMotor*[_maxInstanceSize];
    if (_instanceList == NULL) {
        /* 
        ERRORLOG 
            内容: メモリ確保に失敗した
        */
        ERRORLOG();
        return;
    }
    for (size_t i = 0; i < _maxInstanceSize; i++) {
        _instanceList[i] = NULL;
    }
}

/* static private method */
EJ_DCMotor_Manager* EJ_DCMotor_Manager::getInstance()
{
    if (_singleton == NULL) {
        /*
        ERROLOG
            内容：シングルトンがまだ生成されていない
        */
        ERRORLOG();
        return NULL;
    }

    return _singleton;
}

/* public method */
EJ_DCMotor_Manager::~EJ_DCMotor_Manager()
{
    if (_instanceList != NULL) {
        for (size_t i = 0; i < _maxInstanceSize; i++) {
            if (_instanceList[i] != NULL) {
                delete _instanceList[i];
                _instanceList[i] = NULL;
            }
        }
    }
}

/* static public method */
bool EJ_DCMotor_Manager::configure(size_t maxInstanceSize)
{
    if (_singleton == NULL) {
        _singleton = new EJ_DCMotor_Manager(maxInstanceSize);
        if (_singleton == NULL) {
            /*
            ERROLOG
                内容：メモリ確保に失敗した 
            */
            ERRORLOG();
            return false;
        }
    }
    return true;
}

EJ_DCMotor* EJ_DCMotor_Manager::createMotor(MotorDef motor)
{
    return EJ_DCMotor_Manager::createMotor(motor.pin1, motor.pin2, motor.id);
}

EJ_DCMotor* EJ_DCMotor_Manager::createMotor(uint8_t pin1, uint8_t pin2, uint8_t id)
{
    EJ_DCMotor_Manager *manager = EJ_DCMotor_Manager::getInstance();
    if (manager == NULL) {
        /*
        ERRORLOG
            内容：マネージャクラスのインスタンス取得に失敗した
        */
        ERRORLOG();
        return NULL;
    }
    if (id > manager->_maxInstanceSize) {
        /*
        ERRORLOG
            内容：最大インスタンス数を超えるidが指定された
        */
        ERRORLOG();
        return NULL;
    }
    if (manager->_instanceList[id] == NULL) {
        EJ_DCMotor *instance = new EJ_DCMotor(pin1, pin2);
        if (instance == NULL) {
            /*
            ERRORLOG
                内容：メモリ確保に失敗した
            */
            ERRORLOG();
            return NULL;
        }
        manager->_instanceList[id] = instance;
    }

    return manager->_instanceList[id];
}

EJ_DCMotor* EJ_DCMotor_Manager::getMotor(MotorDef motor)
{
    EJ_DCMotor* instance = EJ_DCMotor_Manager::getMotor(motor.id);
    if (instance == NULL) {
        /*
        ERROLOG
            内容：インスタンス取得に失敗した
        */
        ERRORLOG();
        return NULL;
    }
    if (instance->_pin1 != motor.pin1 || instance->_pin2 != motor.pin2) {
        /*
        ERROLOG
            内容：指定されたidが指すインスタンスと、確保済みの同じidのインスタンスが一致しない
        */
        ERRORLOG();
        return NULL;
    }
    return instance;
}

EJ_DCMotor* EJ_DCMotor_Manager::getMotor(uint8_t id)
{
    EJ_DCMotor_Manager *manager = EJ_DCMotor_Manager::getInstance();
    if (manager == NULL) {
        /*
        ERRORLOG
            内容：マネージャクラスのインスタンス取得に失敗した
        */
        ERRORLOG();
        return NULL;
    }

    if (id >= manager->_maxInstanceSize) {
        /*
        ERRORLOG
            内容：最大インスタンス数を超えるidが指定された
        */
        ERRORLOG();
        return NULL;
    } 
    if (manager->_instanceList[id] == NULL) {
        /*
        ERRORLOG
            内容：指定されたidのインスタンスが存在しない
        */
        ERRORLOG();
        return NULL;
    }
    return manager->_instanceList[id];
}
