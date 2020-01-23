#include "Configuration.h"
#include "Logger.h"

Configuration &Configuration::GetConfiguration()
{
    static Configuration configuration;
    return configuration;
}

Configuration::Configuration()
{
    logger = &Logger::GetLogger();
}

void Configuration::EnableEeprom()
{
    if (!_eepromEnabled)
    {
        logger->Log("Enabling EEPROM.");

        EEPROM.begin(_eepromSize);
        _eepromEnabled = true;
        logger->Log("EEPROM Enabled.");
    }
}
void Configuration::DisableEeprom()
{
    if (_eepromEnabled)
    {
        logger->Log("Disabling EEPROM.");

        EEPROM.end();
        _eepromEnabled = false;
        logger->Log("EEPROM disabled.");
    }
}

void Configuration::ReadFromEEPROM()
{
    EnableEeprom();
    logger->Log("Reading configuration.");
    EEPROM.get(_address, _persistent);
    logger->Log("Configuration read.");
    DisableEeprom();
}

void Configuration::WriteToEEPROM()
{
    EnableEeprom();
    logger->Log("Writing configuration.");
    EEPROM.put(_address, _persistent);
    logger->Log("Configuration written.");
    DisableEeprom();
}

void Configuration::ResetConfigToDefaults()
{
    logger->Log("Resetting device to factory defaults.");
    PersistentVariables persistent;
    _persistent = persistent;
    logger->Log("Configuration set to factory defaults.");
}

void Configuration::RestartDevice(const char *reason)
{
    logger->Log("Device restart initialized. See following log for reason.");
    logger->Log(reason);
    logger->Log("Restarting in 5 seconds.");
    delay(5000);
    ESP.restart();
}

void Configuration::SetSSID(const char *ssid) { snprintf(_persistent.ssid, 32, "%s", ssid); }
void Configuration::SetPassword(const char *password) { snprintf(_persistent.password, 64, "%s", password); }
void Configuration::SetForceSetupPortal(bool value) { _persistent.forceSetupPortal = value; }