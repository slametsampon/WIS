#include "oDeDu.h"

ODeDu::ODeDu(String id) : _id(id) {}

void ODeDu::setConfig(config configData)
{
    _config = configData;
}

void ODeDu::execute(unsigned long samplingTime)
{
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
                _oprStatus = WAIT;
            }
            else
            {
                if ((millis() - _prevOnDelay) > (_config.onDelay * 60 * 1000))
                {
                    _oprStatus = ACTIVE;
                    _prevOnDuration = millis();
                }
                else
                {
                    if ((millis() - _prevOnDuration) > (_config.onDuration * 60 * 1000))
                    {
                        _oprStatus = IDLE;
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
                _oprStatus = WAIT;
            }
            else
            {
                if ((millis() - _prevOnDelay) > (_config.onDelay * 60 * 1000))
                {
                    _oprStatus = ACTIVE;
                    _prevOnDuration = millis();
                }
                else
                {
                    if ((millis() - _prevOnDuration) > (_config.onDuration * 60 * 1000))
                    {
                        _prevOnDelay = millis();
                        _oprStatus = WAIT;
                    }
                }
            }
            break;

        case MANUAL_CON: //Manual Continuous
            _oprStatus = ACTIVE;
            break;

        default:
            break;
        }

        if (_oprStatus == ACTIVE)
        {
            //send command to activate irrigation valve
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

    doc["idNode"] = 15;
    doc["mode"] = 1;
    doc["cyclic"] = 0;
    doc["onDelay"] = 1440;
    doc["onDuration"] = 180;

    serializeJson(doc, strConfig);

    return strConfig;
}

String ODeDu::getStatus()
{
    /*
        {
        "status": 15,
        "onDelay": 1440,
        "onDuration": 180
        }
    */
    String strStatus;
    StaticJsonDocument<48> doc;

    doc["status"] = 15;
    doc["onDelay"] = 1440;
    doc["onDuration"] = 180;

    serializeJson(doc, strStatus);
    return strStatus;
}

int ODeDu::_operationLogic()
{
    int mode = _config.mode;
    int cyclic = _config.cyclic;
    int oprMode = IDLE;

    switch (mode)
    {
    case MANUAL:
        oprMode = MANUAL_CON;
        if (cyclic == ONE_SHOOT)
            oprMode = MANUAL_ONE;
        else if (cyclic == CYCLIC)
            oprMode = MANUAL_CYC;
        /* code */
        break;

    case AUTO:
        /* code */
        break;

    default:
        break;
    }
    return oprMode;
}