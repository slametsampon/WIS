#include "oDeDu.h"

ODeDu::ODeDu(String id) : _id(id) {}

void ODeDu::init(int pin)
{
    _irrigationValve = pin;
    pinMode(_irrigationValve, OUTPUT);

    //set default value
    _config.idNode = 9;
    _config.mode = AUTO;
    _config.cyclic = ONE_SHOOT;
    _config.onDelay = 90;    //minute
    _config.onDuration = 35; //minute
}

void ODeDu::setConfig(config configData)
{
    _config = configData;
}

void ODeDu::execute(unsigned long samplingTime)
{
    int oprStatus = IDLE;
    if ((millis() - _prevMilli) > samplingTime)
    {
        _prevMilli = millis();

        //do process here
        int oprMode = this->_operationLogic(); //select operation mode

        switch (oprMode)
        {
        case MANUAL_ONE: //Manual One Shoot
            if (_firstRun)
            {
                _firstRun = false;
                _prevOnDelay = millis();
                oprStatus = WAIT;
            }
            else
            {
                if ((millis() - _prevOnDelay) > (_config.onDelay * 60 * 1000))
                {
                    _oprStatus.onDelay = (millis() - _prevOnDelay) / (1000 * 60);
                    _oprStatus.onDelay = _config.onDelay - _oprStatus.onDelay;

                    oprStatus = ACTIVE;
                    _prevOnDuration = millis();
                }
                else
                {
                    if ((millis() - _prevOnDuration) > (_config.onDuration * 60 * 1000))
                    {
                        _oprStatus.onDuration = (millis() - _prevOnDuration) / (1000 * 60);
                        _oprStatus.onDuration = _config.onDuration - _oprStatus.onDuration;

                        oprStatus = IDLE;
                        _firstRun = true;
                    }
                }
            }
            break;

        case MANUAL_CYC: //Manual Cyclic
            if (_firstRun)
            {
                _firstRun = false;
                _prevOnDelay = millis();
                oprStatus = WAIT;
            }
            else
            {
                if ((millis() - _prevOnDelay) > (_config.onDelay * 60 * 1000))
                {
                    _oprStatus.onDelay = (millis() - _prevOnDelay) / (1000 * 60);
                    _oprStatus.onDelay = _config.onDelay - _oprStatus.onDelay;

                    oprStatus = ACTIVE;
                    _prevOnDuration = millis();
                }
                else
                {
                    if ((millis() - _prevOnDuration) > (_config.onDuration * 60 * 1000))
                    {
                        _oprStatus.onDuration = (millis() - _prevOnDuration) / (1000 * 60);
                        _oprStatus.onDuration = _config.onDuration - _oprStatus.onDuration;

                        _prevOnDelay = millis();
                        oprStatus = WAIT;
                    }
                }
            }
            break;

        case MANUAL_CON: //Manual Continuous
            oprStatus = ACTIVE;
            break;

        case AUTO: //Auto
            if (_firstRun)
            {
                _firstRun = false;
                _prevOnDelay = millis();
                oprStatus = WAIT;
            }
            else
            {
                if ((millis() - _prevOnDelay) > (_config.onDelay * 60 * 1000))
                {
                    _oprStatus.onDelay = (millis() - _prevOnDelay) / (1000 * 60);
                    _oprStatus.onDelay = _config.onDelay - _oprStatus.onDelay;

                    oprStatus = ACTIVE;
                    _prevOnDuration = millis();
                }
                else
                {
                    if ((millis() - _prevOnDuration) > (_config.onDuration * 60 * 1000))
                    {
                        _oprStatus.onDuration = (millis() - _prevOnDuration) / (1000 * 60);
                        _oprStatus.onDuration = _config.onDuration - _oprStatus.onDuration;

                        oprStatus = IDLE;
                        _firstRun = true;
                    }
                }
            }
            break;

        default:
            break;
        }

        _oprStatus.status = oprStatus;

        if (oprStatus == ACTIVE)
        {
            //send command to activate irrigation valve
            digitalWrite(_irrigationValve, HIGH);
        }
        else
        {
            //send command to de-activate irrigation valve
            digitalWrite(_irrigationValve, LOW);
        }
    }
}

String ODeDu::getConfig()
{
    /*
        {
        "idNode": 15,
        "mode": 1,
        "cyclic": 0,
        "onDelay": 1440,
        "onDuration": 180
        }
    */
    String strConfig;
    StaticJsonDocument<96> doc;

    doc["idNode"] = _config.idNode;
    doc["mode"] = _config.mode;
    doc["cyclic"] = _config.cyclic;
    doc["onDelay"] = _config.onDelay;
    doc["onDuration"] = _config.onDuration;

    serializeJson(doc, strConfig);

    return strConfig;
}

String ODeDu::getStatus()
{
    /*
        {
        "mode": 1,
        "status": 0,
        "onDelay": 1440,
        "onDuration": 180
        }
    */
    String strStatus;
    StaticJsonDocument<48> doc;

    doc["mode"] = _oprStatus.mode;
    doc["status"] = _oprStatus.status;
    doc["onDelay"] = _oprStatus.onDelay;
    doc["onDuration"] = _oprStatus.onDuration;

    serializeJson(doc, strStatus);
    return strStatus;
}

int ODeDu::_operationLogic()
{
    int mode = _config.mode;
    int cyclic = _config.cyclic;
    unsigned long onDelay = _config.onDelay;
    int oprMode = IDLE;

    switch (mode)
    {
    case MANUAL:
        if (cyclic == ONE_SHOOT)
        {
            oprMode = MANUAL_ONE;
        }
        else if (cyclic == CYCLIC)
        {
            oprMode = MANUAL_CYC;
        }

        //manual continuous
        if (onDelay <= 0)
        {
            oprMode = MANUAL_CON;
        }

        break;

    case AUTO:
        oprMode = AUTO;
        break;

    default:
        break;
    }
    _oprStatus.mode = oprMode;
    return oprMode;
}