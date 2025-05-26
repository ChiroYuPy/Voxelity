//
// Created by adrian on 26/05/25.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <string>

enum class LogLevel {
    None,
    Trace,
    Debug,
    Info,
    Success,
    Warn,
    Error,
    Fatal
};

class Logger {
public:
    static void setLevel(LogLevel level);
    static LogLevel getLevel();

    static void sendLogo();

    static void log(LogLevel level, const std::string& message);

    static void trace(const std::string& message);
    static void debug(const std::string& message);
    static void info(const std::string& message);
    static void success(const std::string& message);
    static void warn(const std::string& message);
    static void error(const std::string& message);
    static void fatal(const std::string &message);

    static void separator();
    static void separator(const std::string& message);

private:
    static const int SeparatorWidth;
    static const std::string SeparatorString;
    static LogLevel currentLevel;

    static std::string getColor(LogLevel level);
    static std::string getHeader(LogLevel level);
};

#endif //LOGGER_H
