#include "EJ_Encoder.h"
#include <math.h>

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
class EJ_Encoder
--------------*/

/* static member */
const char* EJ_Encoder::_classname = "EJ_Encoder";
EJ_Encoder* EJ_Encoder::_instance = NULL;

/* public method */
EJ_Encoder::EJ_Encoder(uint8_t a, uint8_t b)
:   _a(a),
    _b(b),
    _count(0),
    _lastEncoded(0)
{
    pinMode(_a, INPUT);
    pinMode(_b, INPUT);
    digitalWrite(_a, HIGH);
    digitalWrite(_b, HIGH);
    _instance = this;
    attachInterrupt(digitalPinToInterrupt(_a), onInterrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(_b), onInterrupt, CHANGE);
}

/* public method */
EJ_Encoder::~EJ_Encoder()
{}

void EJ_Encoder::onInterrupt() 
{
    if (_instance != NULL) {
        int MSB = digitalRead(_instance->_a);
        int LSB = digitalRead(_instance->_b);
        int encoded = (MSB << 1) | LSB;
        int sum = (_instance->_lastEncoded << 2) | encoded;
        if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
        _instance->_count += (sum == 0b1101 || sum == 0b0100) ? -1 : 1;
        }
        _instance->_lastEncoded = encoded;
    }
}

float EJ_Encoder::getAngle()
{
    if (_instance != NULL) {
        return _instance->_count * 360.0 / 1024.0;
    }
}

long long EJ_Encoder::getCount()
{
    if (_instance != NULL) {
        return _instance->_count;
    }
}

void EJ_Encoder::reset()
{
    if (_instance != NULL) {
        _instance->_count = 0;
    }
}

/*----------------------
class EJ_Encoder_Manager 
----------------------*/

/* static member */
EJ_Encoder_Manager* EJ_Encoder_Manager::_singleton = NULL;
const char* EJ_Encoder_Manager::_classname = "EJ_Encoder_Manager";

/* private method */
EJ_Encoder_Manager::EJ_Encoder_Manager(size_t maxInstanceSize)
:   _maxInstanceSize(maxInstanceSize),
    _instanceList(NULL)
{
    _instanceList = new EJ_Encoder*[_maxInstanceSize];
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
EJ_Encoder_Manager* EJ_Encoder_Manager::getInstance()
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
EJ_Encoder_Manager::~EJ_Encoder_Manager()
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
bool EJ_Encoder_Manager::configure(size_t maxInstanceSize)
{
    if (_singleton == NULL) {
        _singleton = new EJ_Encoder_Manager(maxInstanceSize);
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

EJ_Encoder* EJ_Encoder_Manager::createMotor(MotorDef motor)
{
    return EJ_Encoder_Manager::createMotor(motor.pin1, motor.pin2, motor.en, motor.id);
}

EJ_Encoder* EJ_Encoder_Manager::createMotor(uint8_t pin1, uint8_t pin2, int8_t en, uint8_t id)
{
    EJ_Encoder_Manager *manager = EJ_Encoder_Manager::getInstance();
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
        EJ_Encoder *instance = new EJ_Encoder(pin1, pin2, en);
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

EJ_Encoder* EJ_Encoder_Manager::getMotor(MotorDef motor)
{
    EJ_Encoder* instance = EJ_Encoder_Manager::getMotor(motor.id);
    if (instance == NULL) {
        /*
        ERROLOG
            内容：インスタンス取得に失敗した
        */
        ERRORLOG();
        return NULL;
    }
    if (instance->_pin1 != motor.pin1 || instance->_pin2 != motor.pin2 || instance->_en != motor.en) {
        /*
        ERROLOG
            内容：指定されたidが指すインスタンスと、確保済みの同じidのインスタンスが一致しない
        */
        ERRORLOG();
        return NULL;
    }
    return instance;
}

EJ_Encoder* EJ_Encoder_Manager::getMotor(uint8_t id)
{
    EJ_Encoder_Manager *manager = EJ_Encoder_Manager::getInstance();
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
