#include "EJ_EncoderMotor.h"

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

/*-------------------
class EJ_EncoderMotor
-------------------*/

/* static member */
const char* EJ_EncoderMotor::_classname = "EJ_EncoderMotor";

/* private method */
EJ_EncoderMotor::EJ_EncoderMotor(uint8_t pin1, uint8_t pin2, uint8_t enc1, uint8_t enc2, int8_t en)
:   _enc1(enc1),
    _enc2(enc2),
    _target(0),
    EJ_DCMotor(pin1, pin2, en),
    Encoder(_enc1, _enc2)
{}

/* public method */
EJ_EncoderMotor::~EJ_EncoderMotor()
{}

void EJ_EncoderMotor::move(long relativePosition) {
    _target = Encoder::read() + relativePosition;
    if (relativePosition > 0) {
        EJ_DCMotor::forward();
    } else {
        EJ_DCMotor::reverse();
    }
    while (!isTargetReached()) {
        delay(1);
    }
    EJ_DCMotor::stop();
}

void EJ_EncoderMotor::move(long relativePosition, int16_t duty) {
    if (!EJ_DCMotor::_enablePWM) return;
    if (relativePosition * duty < 0) {
        /*
        ERRORLOG
            内容：目標方向とduty比で表現する進行方向が不一致
        */
        ERRORLOG();
        return;
    }
    _target = Encoder::read() + relativePosition;
    EJ_DCMotor::setPWM(duty);
    while (!isTargetReached()) {
        delay(1);
    }
    EJ_DCMotor::stop();
}

void EJ_EncoderMotor::moveTo(long absolutePosition, int16_t duty) {
    move(absolutePosition - Encoder::read(), duty);
}


void EJ_EncoderMotor::setTarget(long targetPosition) {
    _target = targetPosition;
}

bool EJ_EncoderMotor::isTargetReached() {
    return Encoder::read() == _target;
}

/* ↑のコードでしきい値がオーバーシュートする場合のアイデア (Draft)
bool EJ_EncoderMotor::isTargetReached() {
    int currentVal = Encoder::read();
    if (currentVal >= _target - TOLERANCE && currentVal <= _target + TOLERANCE) {
        if (_reachedCount == 0) {
            _reachedTime = millis();
        }
        _reachedCount++;
        if (_reachedCount >= REACHED_COUNT_THRESHOLD && millis() - _reachedTime >= REACHED_TIME_THRESHOLD) {
            return true;
        }
    } else {
        _reachedCount = 0;
    }
    return false;
}
*/

/*---------------------------
class EJ_EncoderMotor_Manager 
---------------------------*/

/* static member */
EJ_EncoderMotor_Manager* EJ_EncoderMotor_Manager::_singleton = NULL;
const char* EJ_EncoderMotor_Manager::_classname = "EJ_EncoderMotor_Manager";

/* private method */
EJ_EncoderMotor_Manager::EJ_EncoderMotor_Manager(size_t maxInstanceSize)
:   _maxInstanceSize(maxInstanceSize),
    _instanceList(NULL)
{
    _instanceList = new EJ_EncoderMotor*[_maxInstanceSize];
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
EJ_EncoderMotor_Manager* EJ_EncoderMotor_Manager::getInstance()
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
EJ_EncoderMotor_Manager::~EJ_EncoderMotor_Manager()
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
bool EJ_EncoderMotor_Manager::configure(size_t maxInstanceSize)
{
    if (_singleton == NULL) {
        _singleton = new EJ_EncoderMotor_Manager(maxInstanceSize);
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

EJ_EncoderMotor* EJ_EncoderMotor_Manager::createEncoderMotor(EncoderMotorDef motor)
{
    return EJ_EncoderMotor_Manager::createEncoderMotor(motor.pin1, motor.pin2, motor.enc1, motor.enc2, motor.en, motor.id);
}

EJ_EncoderMotor* EJ_EncoderMotor_Manager::createEncoderMotor(uint8_t pin1, uint8_t pin2, uint8_t enc1, uint8_t enc2, int8_t en, uint8_t id)
{
    EJ_EncoderMotor_Manager *manager = EJ_EncoderMotor_Manager::getInstance();
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
        EJ_EncoderMotor *instance = new EJ_EncoderMotor(pin1, pin2, enc1, enc2, en);
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

EJ_EncoderMotor* EJ_EncoderMotor_Manager::getEncoderMotor(EncoderMotorDef motor)
{
    EJ_EncoderMotor* instance = EJ_EncoderMotor_Manager::getEncoderMotor(motor.id);
    if (instance == NULL) {
        /*
        ERROLOG
            内容：インスタンス取得に失敗した
        */
        ERRORLOG();
        return NULL;
    }
    if (instance->_enc1 != motor.enc1 || instance->_enc2 != motor.enc2) {
        /*
        ERROLOG
            内容：指定されたidが指すインスタンスと、確保済みの同じidのインスタンスが一致しない
        */
        ERRORLOG();
        return NULL;
    }
    return instance;
}

EJ_EncoderMotor* EJ_EncoderMotor_Manager::getEncoderMotor(uint8_t id)
{
    EJ_EncoderMotor_Manager *manager = EJ_EncoderMotor_Manager::getInstance();
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
