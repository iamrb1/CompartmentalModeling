// OutputLog.hpp
// Author: Bao Tran
// Description: A logging system with multiple verbosity levels
// Created for the Euler Group Project (CSE498-Spring2025)
// 
// Some parts of this implementation were generated with the assistance of ChatGPT (OpenAI).
// Date of ChatGPT Assistance: 2/3/2025
#ifndef OUTPUT_LOG_HPP
#define OUTPUT_LOG_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <cassert>

// Simple log levels
namespace cse {

enum class LogLevel { SILENT, NORMAL, VERBOSE, DEBUG };

class OutputLog {
private:
    LogLevel level;
    std::ofstream logFile;

    bool consoleOutput = true; // Enable or disable console logging

    std::string getCurrentTime() const {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::ostringstream oss;
        oss << std::put_time(std::localtime(&now_time), "[%Y-%m-%d %H:%M:%S] ");
        return oss.str();
    }

public:
    OutputLog(LogLevel logLevel, const std::string& filename) : level(logLevel) {
    logFile.open(filename, std::ios::app); // Open file in append mode
    assert(logFile.is_open() && "Error: Could not open log file!");
}
    
    ~OutputLog() {
    if (logFile.is_open()) {
        logFile.close();
    }
}
	template <typename T>
    void log(const T& message, LogLevel msgLevel) {
         if (level == LogLevel::SILENT) {
            return;
        }

        if (static_cast<int>(msgLevel) > static_cast<int>(level)) {
            return; // Ignore messages above the current log level
        }

        std::ostringstream oss;
        oss << getCurrentTime() << message;
        std::string logMessage = oss.str();

        if (consoleOutput) {
            std::cout << logMessage << std::endl;
        }

        if (logFile.is_open()) {
            logFile << logMessage << std::endl;
            assert(logFile.good() && "Error: Failed to write to log file!");
        }
    }
    void setLogLevel(LogLevel newLevel) {
        level = newLevel;
    }

    void enableConsoleOutput(bool enable) {
        consoleOutput = enable;
    }
};
}
#endif // OUTPUT_LOG_HPP
