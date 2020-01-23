#include "Logger.h"
#include "TimeManager.h"
#include "WifiConnectionHandler.h"
#include "Configuration.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

Logger &logger = Logger::GetLogger();
Configuration &configuration = Configuration::GetConfiguration();
TimeManager &timeManager = TimeManager::GetTimeManager();
WifiConnectionHandler &wifiConnectionHandler = WifiConnectionHandler::GetWifiConnectionHandler();
unsigned long lastLog = 0;

#define DHTPIN 5
#define DHTTYPE DHT11

DHT_Unified dht(DHTPIN, DHTTYPE);

void setup()
{
  logger.SetTimeManager(&timeManager);
  configuration.ReadFromEEPROM();
  if (configuration.IsForceSetupPortal())
  {
    configuration.ResetConfigToDefaults();
    wifiConnectionHandler.StartSetupPortal();
    configuration.RestartDevice("Restart after applying changes to configuration.");
  }
  dht.begin();
}

void loop()
{
  if (!wifiConnectionHandler.IsConnected())
  {
    wifiConnectionHandler.Connect();
  }
  if (!timeManager.IsTimeFetched())
  {
    timeManager.FetchTimeFromNTPServers();
  }
  if (millis() - lastLog >= 5 * 1000)
  {
    lastLog = millis();
    sensors_event_t event;
    float temperature;
    float humidity;
    char buffer[100];
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature))
    {
      logger.Log("Error reading temperature.");
    }
    else
    {
      temperature = event.temperature;
      snprintf(buffer, 100, "Temperature: %f°C", temperature);
      logger.Log(buffer);
    }
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity))
    {
      logger.Log("Error reading humidity.");
    }
    else
    {
      humidity = event.relative_humidity;
      snprintf(buffer, 100, "Humidity: %f%%", humidity);
      logger.Log(buffer);
    }
  }
}