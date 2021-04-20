#include "BatteryManagementSystem.h"

CBMSParam::CBMSParam(float min, float max, Language language, string name): m_minThreshold(min),\
                                                                            m_maxThreshold(max),\
                                                                            m_language(language), \
                                                                            m_paramName(name)
{
    m_tolerance = m_maxThreshold * 0.05 ;
}

CTemperature::CTemperature(float min, float max, Language language, \
                           string name, TempUnitType unit) : CBMSParam(min, max, language, name), \
                                                             m_tempUnit(unit)   
{
    convertTemperature();
}

CChargeRate::CChargeRate(float min, float max, Language language, \
                         string name) : CBMSParam(min, max, language, name)
{

}

CStateOfCharge::CStateOfCharge(float min, float max, Language language, 
                               string name) :  CBMSParam(min, max, language, name)
{

}

void CBMSParam::setMsgFromLanguage()
{

}

void CTemperature::setMsgFromLanguage()
{
    switch (m_language)
    {
        case ENGLISH:
            m_errMsg = "Error: Temperature Out of range";
            m_minWarnMsg = "Warning: Approaching required minimum temperature";
            m_maxWarnMsg = "Warning: Approaching maximum temperature";
            break;
        case DEUTSCH:
            m_errMsg = "Fehler: Temperatur außerhalb des Bereichs";
            m_minWarnMsg = "Warnung: Annäherung an die erforderliche Mindesttemperatur";
            m_maxWarnMsg = "Warnung: Annäherung an die maximale Temperatur";
            break;
        default:
            break;
    }
}

void CStateOfCharge::setMsgFromLanguage()
{
    switch (m_language)
    {
        case ENGLISH:
            m_errMsg = "Error: State of charge out of range";
            m_minWarnMsg = "Warning: Approaching discharge";
            m_maxWarnMsg = "Warning: Approaching charge-peak";
            break;
        case DEUTSCH:
            m_errMsg = "Fehler: Ladezustand außerhalb der Reichweite";
            m_minWarnMsg = "Warnung: Annäherung an die Entladung";
            m_maxWarnMsg = "Warnung: Annäherung an die Ladungsspitze";
            break;
        default:
            break;
    }
}

void CChargeRate::setMsgFromLanguage()
{
    switch (m_language)
    {
        case ENGLISH:
            m_errMsg = "Error: Charge rate out of range";
            m_minWarnMsg = "Warning: Approaching minimum charge rate";
            m_maxWarnMsg = "Warning: Approaching maximum charge rate";
            break;
        case DEUTSCH:
            m_errMsg = "Fehler: Laderate außerhalb des Bereichs";
            m_minWarnMsg = "Warnung: Annäherung an die Mindestladerate";
            m_maxWarnMsg = "Warnung: Annäherung an die maximale Laderate";
            break;
        default:
            break;
    }
}

bool CBMSParam::check_max(float value)
{
    if (value <= m_maxThreshold)
    {
        if (value >= (m_maxThreshold - m_tolerance))
            cout << m_paramName << ": " << m_maxWarnMsg << endl;

        return true;
    }
    else
    {
        cout << m_paramName << ": " << m_errMsg << endl;
        return false;
    }
}

bool CBMSParam::check_min(float value)
{
    if (value >= m_minThreshold)
    {
        if (value <= (m_minThreshold + m_tolerance))
            cout << m_paramName << ": " << m_minWarnMsg << endl;
        
        return true;
    }
    else
    {
        cout << m_paramName << ": " << m_errMsg << endl;
        return false;
    }
}

bool CBMSParam::check_threshold(float value)
{
    return (check_min(value) && check_max(value));
}

void CTemperature::convertTemperature()
{
    if (m_tempUnit == FAHRENHEIT)
    {
         m_minThreshold = (m_minThreshold - 32) * (5 / 9);
         m_maxThreshold = (m_maxThreshold - 32) * (5 / 9);
    }
}

CBMS::CBMS(Language language, TempUnitType unit)
{
    if (language == ENGLISH)
    {
        m_tempObj = new CTemperature(MIN_TEMPERATURE_THRESHOLD, MAX_TEMPERATURE_THRESHOLD, \
                                     language, "Temperature", unit);
        m_socObj = new CStateOfCharge(MIN_SOC_THRESHOLD, MAX_SOC_THRESHOLD, language, "State of Charge");
        m_crObj = new CChargeRate(MIN_CHARGE_RATE_THRESHOLD, MAX_CHARGE_RATE_THRESHOLD, language, "Charge Rate");
    }
    else if (language == DEUTSCH)
    {
         m_tempObj = new CTemperature(MIN_TEMPERATURE_THRESHOLD, MAX_TEMPERATURE_THRESHOLD, \
                                     language, "Temperatur", unit);
        m_socObj = new CStateOfCharge(MIN_SOC_THRESHOLD, MAX_SOC_THRESHOLD, language, "Ladezustand");
        m_crObj = new CChargeRate(MIN_CHARGE_RATE_THRESHOLD, MAX_CHARGE_RATE_THRESHOLD, language, "Ladestrom");
    }
}

bool CBMS::IsBatteryOk(float temperature, float soc, float cr)
{
    bool statTemp;
    bool statSoc;
    bool statCr;

    m_tempObj->setMsgFromLanguage();
    statTemp = m_tempObj->check_threshold(temperature);

    m_socObj->setMsgFromLanguage();
    statSoc = m_socObj->check_threshold(soc);

    m_crObj->setMsgFromLanguage();
    statCr = m_crObj->check_threshold(cr);

    return (statTemp && statSoc && statCr);
}