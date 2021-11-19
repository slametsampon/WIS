
/*********
 * WIS - Wireless Irrigation System :
 * 
 * Operation Mode :
 * Auto - (No Delay)
 * Manual - cyclic (On Delay, On Duration)
 * Manual - continuous (On DElay = 0, On Duration)
 * Manual - Off/Idle (On Duration = 0)
 * 
 * data model :
 * config = {
 *  "idNode" : 9,
 *  "mode" : 1, (Manual = 0, Auto = 1)
 *  "cyclic" : 0, (One Shoot = 0, Cyclic = 1)
 *  "onDelay" : xxxx,
 *  "onDuration" : yyyy
 * }
 * status = {
 *  "status" : 0, (Idle =0, 
 *                Wait = 1, 
 *                Active = 2,
 *                Manual-One = 3,
 *                Manual-Cyc = 4,
 *                Manual-Con = 5,
 *  )
 *  "onDelay" : xxxx,
 *  "onDuration" : yyyy
 * }
*********/
#ifndef model_h
#define model_h

#include <Arduino.h>
#include <ArduinoJson.h>

enum STATUS_OPR
{
    IDLE,
    WAIT,
    ACTIVE,
    MANUAL_ONE,
    MANUAL_CYC,
    MANUAL_CON
};

enum MODE_OPR
{
    MANUAL,
    AUTO
};

enum CYCLIC_OPR
{
    ONE_SHOOT,
    CYCLIC
};

typedef struct config
{
    int idNode; // 0 - 99
    int mode;   //(Manual = 0, Auto = 1)
    int cyclic; // (One Shoot = 0, Cyclic = 1)
    unsigned long onDelay;
    unsigned long onDuration;
} config;

class ODeDu
{
public:
    ODeDu(String);
    String getConfig(); //JSON format
    String getStatus(); //JSON format
    void setConfig(config);
    void init();
    void execute(unsigned long); //sampling periode ms
    void info();

private:
    int _operationLogic();

    boolean _firstRun = true;
    int _oprStatus;
    String _id;
    unsigned long _prevMilli, _samplingTime, _prevOnDelay, _prevOnDuration;
    config _config;
};

#endif
