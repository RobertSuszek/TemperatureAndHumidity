#ifndef LOGGER_H
#define LOGGER_H
#include <Arduino.h>
class TimeManager;

class Logger
{
public:
    static Logger &GetLogger();

    void Log(const char *message);
    char **GetAllLogs();
    char *GetLog(int n);
    void SetTimeManager(TimeManager *ptr);
    bool IsVerbose();

private:
    Logger();
    Logger(const Logger &);
    ~Logger();
    TimeManager *_timeManager;
    char **_logs;
    int _logsMaxCount;
    int _logMaxSize;
    int _currentLog;
    bool _logToSerial;
    bool _verbose;

    void InitializeSerial();
};
#endif