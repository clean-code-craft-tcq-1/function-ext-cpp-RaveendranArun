#ifndef BATTERY_MANAGMENT_SYSTEM_H
#define BATTERY_MANAGMENT_SYSTEM_H

#include <iostream>


using namespace std;

#define MIN_TEMPERATURE_THRESHOLD			0
#define MAX_TEMPERATURE_THRESHOLD			45
#define MIN_SOC_THRESHOLD			        20
#define MAX_SOC_THRESHOLD			        80
#define MAX_CHARGE_RATE_THRESHOLD			0.8
#define MIN_CHARGE_RATE_THRESHOLD           0

/* Supported languages */
typedef enum {
    ENGLISH,
    DEUTSCH
} Language;

/* Supported temperature */
typedef enum
{
    CELCIUS,
    FAHRENHEIT
} TempUnitType;

class CBMSParam
{
protected:
    string m_paramName;
    string m_errMsg;
    string m_minWarnMsg;
    string m_maxWarnMsg;
    Language m_language;
    float m_minThreshold;
    float m_maxThreshold;
    float m_tolerance;
public:
    CBMSParam(float min, float max, Language language, string name);
    virtual void setMsgFromLanguage();
    bool check_threshold(float value);
    bool check_min(float value);
    bool check_max(float value);
};

class CTemperature : public CBMSParam
{
private:
     TempUnitType m_tempUnit;
public:
    CTemperature(float min, float max, Language language, string name, TempUnitType unit);
    void setMsgFromLanguage();
    void convertTemperature();
};

class CStateOfCharge : public CBMSParam
{
public:
    CStateOfCharge(float min, float max, Language language, string name);
    void setMsgFromLanguage();
};

class CChargeRate : public CBMSParam
{
public:
    CChargeRate(float min, float max, Language language, string name);
    void setMsgFromLanguage();
};

class CBMS
{
private:
    CBMSParam* m_tempObj;
    CBMSParam* m_socObj;
    CBMSParam* m_crObj;
public:
    CBMS(Language language, TempUnitType unit);
    bool IsBatteryOk(float temperature, float soc, float cr);
};

#endif