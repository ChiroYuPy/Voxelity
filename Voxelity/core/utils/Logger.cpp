//
// Created by adrian on 26/05/25.
//

#include "Logger.h"

#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>

const int Logger::SeparatorWidth = 48;
const std::string Logger::SeparatorString = std::string(SeparatorWidth, '-');
LogLevel Logger::currentLevel = LogLevel::Info;

std::string loadLogoFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return "[LOGO FILE NOT FOUND]";
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

const std::string AsciiLogo = loadLogoFromFile("resources/ascii_logo.txt");

void Logger::sendLogo() {
    std::cout << "\033[1;36m" << AsciiLogo << "\033[0m" << std::endl;
}

void Logger::setLevel(const LogLevel level) {
    currentLevel = level;
}

LogLevel Logger::getLevel() {
    return currentLevel;
}

std::string Logger::getColor(const LogLevel level) {
    switch (level) {
        case LogLevel::Trace:   return "\033[1;90m"; // Gris clair
        case LogLevel::Debug:   return "\033[1;35m"; // Magenta
        case LogLevel::Info:    return "\033[1;36m"; // Cyan
        case LogLevel::Success: return "\033[1;32m"; // Vert
        case LogLevel::Warn:    return "\033[1;33m"; // Jaune
        case LogLevel::Error:   return "\033[1;31m"; // Rouge
        case LogLevel::Fatal:   return "\033[1;41m"; // Fond rouge
        default:                return "\033[0m";    // Réinitialisation
    }
}

std::string Logger::getHeader(const LogLevel level) {
    switch (level) {
        case LogLevel::Trace:   return " [TRACE] ";
        case LogLevel::Debug:   return " [DEBUG] ";
        case LogLevel::Info:    return " [INFO]  ";
        case LogLevel::Success: return " [OK]    ";
        case LogLevel::Warn:    return " [WARN]  ";
        case LogLevel::Error:   return " [ERROR] ";
        case LogLevel::Fatal:   return " [FATAL] ";
        default:                return "         ";
    }
}

std::string getTimestamp() {
    using namespace std::chrono;

    const auto now = system_clock::now();
    auto time = system_clock::to_time_t(now);
    std::tm localTime{};

    // Verify if we are on windows, so we can use the right time
#ifdef _WIN32
    localtime_s(&localTime, &time);
#else
    localtime_r(&time, &localTime);
#endif

    std::ostringstream oss;
    oss << "[" << std::put_time(&localTime, "%H:%M:%S") << "]"; // time format
    return oss.str();
}

void Logger::log(LogLevel level, const std::string& message) {
    if (static_cast<int>(level) < static_cast<int>(currentLevel)) return;

    std::ostream& out = (level == LogLevel::Error) ? std::cerr : std::cout;
    out << getColor(level) << getTimestamp() << getHeader(level) << message << getColor(LogLevel::None) << std::endl;
}

void Logger::trace(const std::string& message) {
    log(LogLevel::Trace, message);
}

void Logger::debug(const std::string& message) {
    log(LogLevel::Debug, message);
}

void Logger::info(const std::string& message) {
    log(LogLevel::Info, message);
}

void Logger::success(const std::string& message) {
    log(LogLevel::Success, message);
}

void Logger::warn(const std::string& message) {
    log(LogLevel::Warn, message);
}

void Logger::error(const std::string& message) {
    log(LogLevel::Error, message);
}

void Logger::fatal(const std::string& message) {
    log(LogLevel::Fatal, message);
    std::terminate();
}

void Logger::separator() {
    if (static_cast<int>(LogLevel::Info) < static_cast<int>(currentLevel)) return;
    std::cout << SeparatorString << std::endl;
}

void Logger::separator(const std::string& message) {
    if (static_cast<int>(LogLevel::Info) < static_cast<int>(currentLevel)) return;

    if (message.empty()) {
        separator();
        return;
    }

    std::string label = "[ " + message + " ]";
    int totalPadding = SeparatorWidth - static_cast<int>(label.size());

    if (totalPadding <= 0) {
        label = "[ " + message.substr(0, SeparatorWidth - 8) + "... ]";
        totalPadding = SeparatorWidth - static_cast<int>(label.size());
    }

    const int left = totalPadding / 2;
    const int right = totalPadding - left;

    std::cout << std::string(left, '-') << label << std::string(right, '-') << std::endl;
}