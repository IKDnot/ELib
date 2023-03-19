#include "EJ_ToFUnit.h"

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
class EJ_ToFUnit
--------------*/

/* static menber */
const char* EJ_ToFUnit::_classname = "EJ_ToFUnit";

/* private method */
EJ_ToFUnit::EJ_ToFUnit(uint8_t address)
:   _address(address),
    _error(true),
    VL53L0X()
{
    if (!VL53L0X::init()) {
        ERRORLOG();
        return;
    }
    VL53L0X::setAddress(_address);
    VL53L0X::setTimeout(500);
    VL53L0X::startContinuous(0);
    _error = false;
}

/* public method */
EJ_ToFUnit::~EJ_ToFUnit()
{
    VL53L0X::stopContinuous();
}

uint16_t EJ_ToFUnit::read()
{
    return VL53L0X::readRangeContinuousMillimeters();
}

/*--------------
class EJ_ToFUnit
--------------*/

/* static member */
EJ_ToFUnit_Manager* EJ_ToFUnit_Manager::_singleton = NULL;
const char* EJ_ToFUnit_Manager::_classname = "EJ_ToFUnit_Manager";

/* private method */
EJ_ToFUnit_Manager::EJ_ToFUnit_Manager(size_t maxInstanceSize)
:   _maxInstanceSize(maxInstanceSize),
    _instanceList(NULL)
{
    _instanceList = new EJ_ToFUnit*[_maxInstanceSize];
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
EJ_ToFUnit_Manager* EJ_ToFUnit_Manager::getInstance()
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
EJ_ToFUnit_Manager::~EJ_ToFUnit_Manager()
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
bool EJ_ToFUnit_Manager::configure(size_t maxInstanceSize)
{
    if (_singleton == NULL) {
        _singleton = new EJ_ToFUnit_Manager(maxInstanceSize);
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

EJ_ToFUnit* EJ_ToFUnit_Manager::createToFUnit(ToFUnitDef tof)
{
    return EJ_ToFUnit_Manager::createToFUnit(tof.id, tof.address);
}

EJ_ToFUnit* EJ_ToFUnit_Manager::createToFUnit(uint8_t id, uint8_t address)
{
    EJ_ToFUnit_Manager *manager = EJ_ToFUnit_Manager::getInstance();
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
        EJ_ToFUnit *instance = new EJ_ToFUnit(address);
        if (instance == NULL || instance->_error) {
            /*
            ERRORLOG
                内容：インスタンス生成に失敗した
            */
            ERRORLOG();
            return NULL;
        }
        manager->_instanceList[id] = instance;
    }

    return manager->_instanceList[id];
}

EJ_ToFUnit* EJ_ToFUnit_Manager::getToFUnit(uint8_t id)
{
    EJ_ToFUnit_Manager *manager = EJ_ToFUnit_Manager::getInstance();
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
