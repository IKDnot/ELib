#include "EJ_I2CHub.h"
#include "M5Core2.h"
#ifdef M5_DEBUG
#define ERRORLOG() M5.Lcd.printf("[ERROR] Class:%s, Line:%d\n", _classname, __LINE__)
#else
#define ERRORLOG() ((void)0)
#endif

/*--------------
class EJ_I2CHub
--------------*/

/* static menber */
const char* EJ_I2CHub::_classname = "EJ_I2CHub";

/* private method */
EJ_I2CHub::EJ_I2CHub(uint8_t address)
:   _address(address),
    ClosedCube::Wired::TCA9548A(_address)
{}

/* public method */
EJ_I2CHub::~EJ_I2CHub()
{}

/*---------------------
class EJ_I2CHub_Manager
---------------------*/

/* static member */
EJ_I2CHub_Manager* EJ_I2CHub_Manager::_singleton = NULL;
const char* EJ_I2CHub_Manager::_classname = "EJ_I2CHub_Manager";

/* private method */
EJ_I2CHub_Manager::EJ_I2CHub_Manager(size_t maxInstanceSize)
:   _maxInstanceSize(maxInstanceSize),
    _instanceList(NULL)
{
    _instanceList = new EJ_I2CHub*[_maxInstanceSize];
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
EJ_I2CHub_Manager* EJ_I2CHub_Manager::getInstance()
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
EJ_I2CHub_Manager::~EJ_I2CHub_Manager()
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
bool EJ_I2CHub_Manager::configure(size_t maxInstanceSize)
{
    if (_singleton == NULL) {
        _singleton = new EJ_I2CHub_Manager(maxInstanceSize);
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

EJ_I2CHub* EJ_I2CHub_Manager::createI2CHub(uint8_t id, uint8_t address)
{
    EJ_I2CHub_Manager *manager = EJ_I2CHub_Manager::getInstance();
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
        EJ_I2CHub *instance = new EJ_I2CHub(address);
        if (instance == NULL) {
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

EJ_I2CHub* EJ_I2CHub_Manager::getI2CHub(uint8_t id)
{
    EJ_I2CHub_Manager *manager = EJ_I2CHub_Manager::getInstance();
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
