#include "EJ_PhotoInterrupter.h"

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

/*-----------------------
class EJ_PhotoInterrupter
-----------------------*/

/* static member */
const char* EJ_PhotoInterrupter::_classname = "EJ_PhotoInterrupter";

/* private method */
EJ_PhotoInterrupter::EJ_PhotoInterrupter(uint8_t pin)
:   _pin(pin)
{
    pinMode(_pin, INPUT);
}

/* public method */
bool EJ_PhotoInterrupter::isInterrupted()
{
    int value = digitalRead(_pin);
    return (value == HIGH) ? true : false;
}

EJ_PhotoInterrupter::~EJ_PhotoInterrupter()
{}

/*-------------------------------
class EJ_PhotoInterrupter_Manager 
-------------------------------*/

/* static member */
EJ_PhotoInterrupter_Manager* EJ_PhotoInterrupter_Manager::_singleton = NULL;
const char* EJ_PhotoInterrupter_Manager::_classname = "EJ_PhotoInterrupter_Manager";

/* private method */
EJ_PhotoInterrupter_Manager::EJ_PhotoInterrupter_Manager(size_t maxInstanceSize)
:   _maxInstanceSize(maxInstanceSize),
    _instanceList(NULL)
{
    _instanceList = new EJ_PhotoInterrupter*[_maxInstanceSize];
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
EJ_PhotoInterrupter_Manager* EJ_PhotoInterrupter_Manager::getInstance()
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
EJ_PhotoInterrupter_Manager::~EJ_PhotoInterrupter_Manager()
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
bool EJ_PhotoInterrupter_Manager::configure(size_t maxInstanceSize)
{
    if (_singleton == NULL) {
        _singleton = new EJ_PhotoInterrupter_Manager(maxInstanceSize);
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

EJ_PhotoInterrupter* EJ_PhotoInterrupter_Manager::createPhotoInterrupter(PhotoInterrupterDef PhotoInterrupter)
{
    return EJ_PhotoInterrupter_Manager::createPhotoInterrupter(PhotoInterrupter.pin, PhotoInterrupter.id);
}

EJ_PhotoInterrupter* EJ_PhotoInterrupter_Manager::createPhotoInterrupter(uint8_t pin, uint8_t id)
{
    EJ_PhotoInterrupter_Manager *manager = EJ_PhotoInterrupter_Manager::getInstance();
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
        EJ_PhotoInterrupter *instance = new EJ_PhotoInterrupter(pin);
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

EJ_PhotoInterrupter* EJ_PhotoInterrupter_Manager::getPhotoInterrupter(PhotoInterrupterDef PhotoInterrupter)
{
    EJ_PhotoInterrupter* instance = EJ_PhotoInterrupter_Manager::getPhotoInterrupter(PhotoInterrupter.id);
    if (instance == NULL) {
        /*
        ERROLOG
            内容：インスタンス取得に失敗した
        */
        ERRORLOG();
        return NULL;
    }
    if (instance->_pin != PhotoInterrupter.pin) {
        /*
        ERROLOG
            内容：指定されたidが指すインスタンスと、確保済みの同じidのインスタンスが一致しない
        */
        ERRORLOG();
        return NULL;
    }
    return instance;
}

EJ_PhotoInterrupter* EJ_PhotoInterrupter_Manager::getPhotoInterrupter(uint8_t id)
{
    EJ_PhotoInterrupter_Manager *manager = EJ_PhotoInterrupter_Manager::getInstance();
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
