#include "Logger.h"
#include "TimeManager.h"

Logger &Logger::GetLogger()
{
    static Logger logger;
    return logger;
}

Logger::Logger()
{
    _logsMaxCount = 100;
    _logMaxSize = 100;
    _currentLog = 0;
    _logToSerial = true;
    _verbose = true;

    InitializeSerial();
    _logs = new char *[_logsMaxCount];
    for (int i = 0; i < _logsMaxCount; i++)
    {
        _logs[i] = new char[_logMaxSize];
    }
}
Logger::~Logger()
{
    for (int i = 0; i < _logsMaxCount; i++)
    {
        delete[] _logs[i];
    }

    delete[] _logs;
}

void Logger::InitializeSerial()
{
    Serial.begin(115200);
}

void Logger::Log(const char *message)
{
    char logMessage[_logMaxSize];
    const char *timeMessage = _timeManager->GetCurrentTimeMessage();
    snprintf(logMessage, _logMaxSize, "%s %s", timeMessage, message);
    Serial.println(logMessage);
    _logs[_currentLog] = logMessage;
    _currentLog = ++_currentLog % _logsMaxCount;
}

void Logger::SetTimeManager(TimeManager *ptr)
{
    _timeManager = ptr;
}

bool Logger::IsVerbose()
{
    return _verbose;
}