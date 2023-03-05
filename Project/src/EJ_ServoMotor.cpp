#include "EJ_ServoMotor.h"
#include "M5Core2.h"

/*-------------------
class EJ_ServoMotor
-------------------*/

/* static menber */
const char* EJ_ServoMotor::_classname = "EJ_ServoMotor";

/* private method */
EJ_ServoMotor::EJ_ServoMotor(uint8_t pin, int min, int max)
:   _pin(pin),
    _min_(min),
    _max_(max),
    Servo()
{
    Servo::attach(_pin, _min_, _max_);
}

/* public method */
EJ_ServoMotor::~EJ_ServoMotor()
{
    Servo::detach();
}

void EJ_ServoMotor::write(int angle)
{
    if (angle < 0 || 180 < angle) {
        /* 
        ERRORLOG 
            内容: 無効な角度が指定された
        */
        M5.Lcd.printf("[ERROR] Class:%s, Line:%d\n", _classname, __LINE__);
        return;
    }
    Servo::write(angle);
}

void EJ_ServoMotor::writeMicroseconds(int us)
{
    Servo::writeMicroseconds(us);
}

int EJ_ServoMotor::read()
{
    return Servo::read();
}

/*-----------------
class EJ_ServoMotor
-----------------*/

/* static member */
EJ_ServoMotor_Manager* EJ_ServoMotor_Manager::_singleton = NULL;
const char* EJ_ServoMotor_Manager::_classname = "EJ_ServoMotor_Manager";

/* private method */
EJ_ServoMotor_Manager::EJ_ServoMotor_Manager(size_t maxInstanceSize)
:   _maxInstanceSize(maxInstanceSize),
    _numOfInstanceSize(0),
    _instanceList(NULL)
{
    _instanceList = new EJ_ServoMotor*[_maxInstanceSize];
    if (_instanceList == NULL) {
        /* 
        ERRORLOG 
            内容: メモリ確保に失敗した
        */
        M5.Lcd.printf("[ERROR] Class:%s, Line:%d\n", _classname, __LINE__);
        return;
    }
    for (size_t i = 0; i < _maxInstanceSize; i++) {
        _instanceList[i] = NULL;
    }
}

/* static public method */
EJ_ServoMotor_Manager* EJ_ServoMotor_Manager::getInstance(size_t maxInstanceSize)
{
    if (_singleton == NULL) {
        _singleton = new EJ_ServoMotor_Manager(maxInstanceSize);
        if (_singleton == NULL) {
            /*
            ERROLOG
                内容：メモリ確保に失敗した 
            */
            M5.Lcd.printf("[ERROR] Class:%s, Line:%d\n", _classname, __LINE__);
            return NULL;
        }
    }
    return _singleton;
}

EJ_ServoMotor_Manager* EJ_ServoMotor_Manager::getInstance()
{
    if (_singleton == NULL) {
        /*
        ERROLOG
            内容：シングルトンがまだ生成されていない
        */
        M5.Lcd.printf("[ERROR] Class:%s, Line:%d\n", _classname, __LINE__);
        return NULL;
    }

    return _singleton;
}

/* public method */
EJ_ServoMotor_Manager::~EJ_ServoMotor_Manager()
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

EJ_ServoMotor* EJ_ServoMotor_Manager::createServo(ServoDef servo)
{
    return createServo(servo.pin, servo.id);
}

EJ_ServoMotor* EJ_ServoMotor_Manager::createServo(uint8_t pin, uint8_t id, int min, int max)
{
    if (id > _maxInstanceSize) {
        /*
        ERRORLOG
            内容：最大インスタンス数を超えるidが指定された
        */
        M5.Lcd.printf("[ERROR] Class:%s, Line:%d\n", _classname, __LINE__);
        return NULL;
    }
    if (_instanceList[id] == NULL) {
        EJ_ServoMotor *instance = new EJ_ServoMotor(pin, min, max);
        if (instance == NULL) {
            /*
            ERRORLOG
                内容：メモリ確保に失敗した
            */
            M5.Lcd.printf("[ERROR] Class:%s, Line:%d\n", _classname, __LINE__);
            return NULL;
        }
        _instanceList[id] = instance;
    }

    return _instanceList[id];
}

EJ_ServoMotor* EJ_ServoMotor_Manager::getServo(ServoDef servo)
{
    EJ_ServoMotor* instance = getServo(servo.id);
    if (instance == NULL) {
        /*
        ERROLOG
            内容：インスタンス取得に失敗した
        */
        M5.Lcd.printf("[ERROR] Class:%s, Line:%d\n", _classname, __LINE__);
        return NULL;
    }
    if (instance->_pin != servo.pin) {
        /*
        ERROLOG
            内容：指定されたidが指すインスタンスと、確保済みの同じidのインスタンスが一致しない
        */
        M5.Lcd.printf("[ERROR] Class:%s, Line:%d\n", _classname, __LINE__);
        return NULL;
    }
    return instance;
}

EJ_ServoMotor* EJ_ServoMotor_Manager::getServo(uint8_t id)
{
    if (id >= _maxInstanceSize) {
        /*
        ERRORLOG
            内容：最大インスタンス数を超えるidが指定された
        */
        M5.Lcd.printf("[ERROR] Class:%s, Line:%d\n", _classname, __LINE__);
        return NULL;
    } 
    if (_instanceList[id] == NULL) {
        /*
        ERRORLOG
            内容：指定されたidのインスタンスが存在しない
        */
        M5.Lcd.printf("[ERROR] Class:%s, Line:%d\n", _classname, __LINE__);
        return NULL;
    }
    return _instanceList[id];
}
