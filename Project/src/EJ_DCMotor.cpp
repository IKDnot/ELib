#include "EJ_DCMotor.h"

/*-------------------
class EJ_DCMotor_Impl 
-------------------*/

/* private method */
void EJ_DCMotor::write(uint8_t bit1, uint8_t bit2)
{
    digitalWrite(_pin1, bit1);
    digitalWrite(_pin2, bit2);
}

/* protected method */
EJ_DCMotor::EJ_DCMotor(uint8_t pin1, uint8_t pin2)
:   _pin1(pin1),
    _pin2(pin2)
{
    pinMode(_pin1, OUTPUT);
    pinMode(_pin2, OUTPUT);
    digitalWrite(_pin1, LOW);
    digitalWrite(_pin2, LOW);
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

/*---------------
class EJ_DCMotor 
---------------*/

/* static member */
EJ_DCMotor_Manager* EJ_DCMotor_Manager::_singleton = NULL;

/* private method */
EJ_DCMotor_Manager::EJ_DCMotor_Manager(size_t maxInstanceSize)
:   _maxInstanceSize(maxInstanceSize),
    _numOfInstanceSize(0),
    _instanceList(NULL)
{
    _instanceList = new EJ_DCMotor*[_maxInstanceSize];
    if (_instanceList == NULL) {
        /* ERRORLOG */
        return;
    }
    for (size_t i = 0; i < _maxInstanceSize; i++) {
        _instanceList[i] = NULL;
    }
}

/* static public method */
EJ_DCMotor_Manager* EJ_DCMotor_Manager::getInstance(size_t maxInstanceSize)
{
    if (_singleton == NULL) {
        _singleton = new EJ_DCMotor_Manager(maxInstanceSize);
        if (_singleton == NULL) {
            /* ERROLOG */
            return NULL;
        }
    }
    return _singleton;
}

EJ_DCMotor_Manager* EJ_DCMotor_Manager::getInstance()
{
    if (_singleton == NULL) {
        /* ERROLOG */
        return NULL;
    }

    return _singleton;
}

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

EJ_DCMotor* EJ_DCMotor_Manager::createMotor(uint8_t pin1, uint8_t pin2, uint8_t id)
{
    if (id > _maxInstanceSize) {
        /* ERRORLOG */
        return NULL;
    }
    if (_instanceList[id] == NULL) {
        EJ_DCMotor *instance = new EJ_DCMotor(pin1, pin2);
        if (instance == NULL) {
            /* ERRORLOG */
            return NULL;
        }
        _instanceList[id] = instance;
    }

    return _instanceList[id];
}

EJ_DCMotor* EJ_DCMotor_Manager::getMotor(uint8_t id)
{
    if (id >= _maxInstanceSize) {
        /* ERRORLOG */
        return NULL;
    } 
    if (_instanceList[id] == NULL) {
        /* ERRORLOG */
        return NULL;
    }
    return _instanceList[id];
}
