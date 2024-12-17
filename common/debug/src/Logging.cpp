/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Logging.cpp
*/

#include "Logging.hpp"

Logging::Logging() : _logLevel(LogLevel::DEBUG), _consoleLoggingEnabled(true)
{}

Logging::~Logging()
{
    if (_logFile.is_open()) {
        _logFile.close();
    }
}

Logging &Logging::getInstance()
{
    static Logging instance;
    return instance;
}

void Logging::setLogFile(const std::string &filename)
{
    std::lock_guard lock(_mutex);

    if (_logFile.is_open()) {
        _logFile.close();
    }

    _logFile.open(filename, std::ios::out | std::ios::app);
    if (!_logFile) {
        throw std::runtime_error("Unable to open log file: " + filename);
    }
}

void Logging::setLogLevel(const LogLevel level)
{
    std::lock_guard lock(_mutex);
    _logLevel = level;
}

void Logging::enableConsoleLogging(const bool enable)
{
    std::lock_guard lock(_mutex);
    _consoleLoggingEnabled = enable;
}

std::string Logging::getTimestamp()
{
    const std::time_t now = std::time(nullptr);
    const std::tm *now_tm = std::localtime(&now);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", now_tm);
    return {buffer};
}

std::string Logging::logLevelToString(const LogLevel level)
{
    switch (level) {
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARNING:
            return "WARNING";
        case LogLevel::ERROR_LOG:
            return "ERROR_LOG";
        default:
            return "UNKNOWN";
    }
}

const std::vector<std::string> &Logging::getLogs()
{
    static std::vector<std::string> logVector;
    std::lock_guard<std::mutex> lock(_mutex);
    logVector.assign(_logs.begin(), _logs.end());
    return logVector;
}

void Logging::clearLogs()
{
    std::lock_guard<std::mutex> lock(_mutex);
    _logs.clear();
}

void Logging::setMaxLogCount(size_t count)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _maxLogCount = count;
    while (_logs.size() > _maxLogCount) {
        _logs.pop_front();
    }
}
