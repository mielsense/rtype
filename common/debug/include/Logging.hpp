/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Logging
*/

#pragma once

#include <deque>
#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>

#ifdef THROW_ERROR
#undef THROW_ERROR // Not recommended unless you're sure you won't use Windows GDI
#endif

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING, ERROR_LOG };

class Logging {
    public:
        static Logging &getInstance();

        template <typename T>
        void log(LogLevel level, const T &message);

        void setLogFile(const std::string &filename);
        void setLogLevel(LogLevel level);
        void enableConsoleLogging(bool enable);

        const std::vector<std::string> &getLogs();
        void clearLogs();
        void setMaxLogCount(size_t count);

        Logging(const Logging &) = delete;
        Logging &operator=(const Logging &) = delete;

    private:
        Logging();
        ~Logging();

        static std::string getTimestamp();
        static std::string logLevelToString(LogLevel level);

        std::ofstream _logFile;
        LogLevel _logLevel;
        bool _consoleLoggingEnabled;
        std::mutex _mutex;

        std::deque<std::string> _logs;
        size_t _maxLogCount = 1000;
};

#define LOG(level, ...) Logging::getInstance().log(level, fmt::format(__VA_ARGS__))
#define LOG_DEBUG(...) LOG(LogLevel::DEBUG, __VA_ARGS__)
#define LOG_INFO(...) LOG(LogLevel::INFO, __VA_ARGS__)
#define LOG_WARNING(...) LOG(LogLevel::WARNING, __VA_ARGS__)
#define LOG_ERROR(...) LOG(LogLevel::ERROR_LOG, __VA_ARGS__)

template <typename T>
void Logging::log(LogLevel level, const T &message)
{
    std::lock_guard<std::mutex> lock(_mutex);

    if (level < _logLevel) {
        return;
    }

    std::string logMessage = fmt::format("{} [{}] {}", getTimestamp(), logLevelToString(level), message);

    if (_consoleLoggingEnabled) {
        std::cout << logMessage << std::endl;
    }
    _logFile << logMessage << std::endl;

    _logs.push_back(logMessage);
    if (_logs.size() > _maxLogCount) {
        _logs.pop_front();
    }
}
