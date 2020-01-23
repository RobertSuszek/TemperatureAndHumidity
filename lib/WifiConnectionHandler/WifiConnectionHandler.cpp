#include "WifiConnectionHandler.h"
#include "Logger.h"
#include "Configuration.h"
WifiConnectionHandler &WifiConnectionHandler::GetWifiConnectionHandler()
{
    static WifiConnectionHandler wifiConnectionHandler;
    return wifiConnectionHandler;
}

WifiConnectionHandler::WifiConnectionHandler()
{
    _logger = &Logger::GetLogger();
    _configuration = &Configuration::GetConfiguration();
    SetupWifiManager();
}

void WifiConnectionHandler::StartSetupPortal()
{
    _logger->Log("Starting setup portal.");
    _logger->Log("IP address is: 192.168.4.1");
    if (!_wifiManager.startConfigPortal(_configuration->GetSSID(), _configuration->GetPassword()))
    {
        _configuration->RestartDevice("Failed to connect to WiFi.");
    }
    _logger->Log("Connected to WiFi successfully.");
    _logger->Log("Saving new configuration");
    _configuration->SetSSID(_wifiManager.getWiFiSSID().c_str());
    _configuration->SetPassword(_wifiManager.getWiFiPass().c_str());
    _configuration->SetForceSetupPortal(false);
    _configuration->WriteToEEPROM();
    _logger->Log("Configuration saved.");
}

void WifiConnectionHandler::SetupWifiManager()
{
    _wifiManager.setConfigPortalTimeout(300);
    _wifiManager.setDebugOutput(false);
    _wifiManager.setConfigResetCallback(ResetButtonCallback);
    _wifiManager.setRestorePersistent(false);
}
void WifiConnectionHandler::ResetButtonCallback()
{
    Configuration &configuration = Configuration::GetConfiguration();
    configuration.ResetConfigToDefaults();
    configuration.WriteToEEPROM();
    configuration.RestartDevice("Resetting configuration to default.");
}

void WifiConnectionHandler::Connect()
{
    if (millis() - _lastConnectionAttempt < DelayBetweenConnectTries() && _failedConnectAttempts > 0)
    {
        return;
    }
    _lastConnectionAttempt = millis();
    _logger->Log("Trying to connect to WiFi.");

    WiFi.begin(_configuration->GetSSID(), _configuration->GetPassword());
    delay(10000);
    if (IsConnected())
    {
        _logger->Log("Wifi connection successful.");
        _failedConnectAttempts = 0;
    }
    else
    {
        _failedConnectAttempts++;
        char buffer[100];
        snprintf(buffer, 100, "%s %i.", "Wifi connection attempt failed. Attempt number: ", _failedConnectAttempts);
        _logger->Log(buffer);
    }
}

bool WifiConnectionHandler::IsConnected()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return true;
    }

    return false;
}

unsigned long WifiConnectionHandler::DelayBetweenConnectTries()
{
    if (_failedConnectAttempts < 10)
    {
        return 30 * 1000;
    }
    if (_failedConnectAttempts < 20)
    {
        return 1 * 60 * 1000;
    }
    if (_failedConnectAttempts < 30)
    {
        return 5 * 60 * 1000;
    }

    return 60 * 60 * 1000;
}