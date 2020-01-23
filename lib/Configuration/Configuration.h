#ifndef CONFIGURATION_H
#define CONFIGURATION_H
class Logger;
#include <EEPROM.h>

class Configuration
{
private:
    Configuration();
    Configuration(const Configuration &);

    struct PersistentVariables
    {
        char ssid[33] = "HomeAutomationDeviceSetup\0";
        char password[65] = "setup123\0";
        bool forceSetupPortal = true;
    };

    PersistentVariables _persistent;
    int _address = 0;
    bool _eepromEnabled = false;
    size_t _eepromSize = 4096;
    Logger *logger;
    void EnableEeprom();
    void DisableEeprom();

public:
    static Configuration &GetConfiguration();
    void ReadFromEEPROM();
    void WriteToEEPROM();
    bool IsForceSetupPortal() { return _persistent.forceSetupPortal; }
    char *GetSSID() { return _persistent.ssid; }
    char *GetPassword() { return _persistent.password; }
    void SetForceSetupPortal(bool value);
    void SetSSID(const char *ssid);
    void SetPassword(const char *password);
    void ResetConfigToDefaults();
    void RestartDevice(const char *reason);
};

#endif