#include "TimeManager.h"
#include "Logger.h"
#ifdef ESP32
#include <WiFi.h>
#include <ESP32Ping.h>
#endif
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>
#endif
TimeManager &TimeManager::GetTimeManager()
{
    static TimeManager timeManager;
    return timeManager;
}

TimeManager::TimeManager()
{
    logger = &Logger::GetLogger();
}

time_t TimeManager::GetCurrentTime()
{
    return time(nullptr);
}

char *TimeManager::GetCurrentTimeMessage()
{
    time_t now = GetCurrentTime();
    char *timeMessage = ctime(&now);
    timeMessage[strcspn(timeMessage, "\n")] = 0;
    return timeMessage;
}

void TimeManager::FetchTimeFromNTPServers()
{
    if (millis() - _lastFetchAttempt < DelayBetweenFetchTries() && _failedFetchAttempts > 0)
    {
        return;
    }
    _lastFetchAttempt = millis();
    logger->Log("Getting current time from NTP servers.");
    if (!TestConnection())
    {
        logger->Log("Failed to get current time from NTP servers.");
        return;
    }
    int timezone = 1;
    int dst = 0;
    configTime(timezone * 3600, dst * 0, _server0, _server1, _server2);
    logger->Log("Current time set.");
    _timeFetched = true;
}

bool TimeManager::TestConnection()
{
    logger->Log("Testing connection to NTP servers.");
    if (!Ping.ping(_server0))
    {
        logger->Log("Connection to first server failed.");
    }
    else
    {
        logger->Log("Connection to first server successful.");
        return true;
    }
    if (!Ping.ping(_server1))
    {
        logger->Log("Connection to second server failed.");
    }
    else
    {
        logger->Log("Connection to second server successful.");
        return true;
    }
    if (!Ping.ping(_server2))
    {
        logger->Log("Connection to third server failed.");
    }
    else
    {
        logger->Log("Connection to third server successfull.");
        return true;
    }
    logger->Log("Connection to all servers failed.");
    return false;
}

unsigned long TimeManager::DelayBetweenFetchTries()
{
    if (_failedFetchAttempts < 10)
    {
        return 30 * 1000;
    }
    if (_failedFetchAttempts < 20)
    {
        return 1 * 60 * 1000;
    }
    if (_failedFetchAttempts < 30)
    {
        return 5 * 60 * 1000;
    }

    return 60 * 60 * 1000;
}