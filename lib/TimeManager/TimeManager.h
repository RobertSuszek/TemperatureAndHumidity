#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <Arduino.h>

class Logger;

class TimeManager
{
private:
    TimeManager();
    TimeManager(const TimeManager &);
    Logger *logger;
    bool TestConnection();
    const char *_server0 = "pool.ntp.org";
    const char *_server1 = "time.google.com";
    const char *_server2 = "pl.pool.ntp.org";
    bool _timeFetched = false;
    int _failedFetchAttempts = 0;
    unsigned long DelayBetweenFetchTries();
    unsigned long _lastFetchAttempt;

public:
    static TimeManager &GetTimeManager();
    time_t GetCurrentTime();
    char *GetCurrentTimeMessage();
    void FetchTimeFromNTPServers();
    bool IsTimeFetched() { return _timeFetched; }
};

#endif