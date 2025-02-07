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

// Simple log levels
namespace cse {
enum class LogLevel { NORMAL, DEBUG };

class OutputLog {
private:
    LogLevel level;
    std::ofstream logFile;

public:
    OutputLog(LogLevel logLevel, const std::string& filename = "log.txt");  // Constructor
    
    ~OutputLog();  // Destructor

    void log(const std::string& message, LogLevel msgLevel);
};
}
#endif // OUTPUT_LOG_HPP
