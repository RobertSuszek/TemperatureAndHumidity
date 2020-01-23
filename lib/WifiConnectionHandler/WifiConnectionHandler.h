#ifndef WIFICONNECTIONHANDLER_H
#define WIFICONNECTIONHANDLER_H
class Logger;
class Configuration;
#include <WiFiManager.h>
class WifiConnectionHandler
{
private:
    WifiConnectionHandler();
    WifiConnectionHandler(const WifiConnectionHandler &);
    Logger *_logger;
    Configuration *_configuration;
    WiFiManager _wifiManager;
    static void ResetButtonCallback();
    void SetupWifiManager();
    int _failedConnectAttempts = 0;
    unsigned long DelayBetweenConnectTries();
    unsigned long _lastConnectionAttempt = 0;

public:
    static WifiConnectionHandler &GetWifiConnectionHandler();
    void Initialize(const char *ssid, const char *password);
    void StartSetupPortal();
    void Connect();
    bool IsConnected();
};
#endif