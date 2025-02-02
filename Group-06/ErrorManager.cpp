#pragma once
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <functional>
#include <cstdlib>

namespace cse {
    /**
     * @brief A class for convenient management and output of different types of errors
     * 
     * 
     */
    class ErrorManager {
    public:
        enum class ErrorLevel {
            Info,
            Warning,
            Fatal
        };
        /**
         * @brief Writes a message to console with a message type at the start of a line
         * 
         * @param message Message to be sent to user
         * @param level Error level of a message. Defaults to "Info" if not specified
         */ 
        void printError(const std::string& message, ErrorLevel level = ErrorLevel::Info) {
            printError(0, message, level);
        }

        /**
         * @brief Writes a message to console with a message type at line number
         * 
         * @param line Line number where error occured
         * @param message Message to be sent to user
         * @param level Error level of a message. Defaults to "Info"
         */
        void printError(size_t line, const std::string& message, ErrorLevel level = ErrorLevel::Info) {
            std::stringstream ss;
            ss << "[" << stringColorCodes[level] << errorLevelToString(level) << "\033[0m]";
            if (line > 0) {
                ss << " (line " << line << ")";
            }
            ss << ": " << message << "\n";
            std::cout << ss.str();

            if (actions.find(level) != actions.end()) {
                actions[level]();
            }
        }

        /**
         * @brief Set the Action that will be invoked when an error of a certain ErrorType occurs
         * 
         * @param level Error Level to which action will be assigned
         * @param action Function that will be called. Does not accept arguments and returns void
         */
        void setAction(ErrorLevel level, std::function<void()> action) {
            actions[level] = action;
        }

        ErrorManager() {}

        /**
         * @brief Allows to enable or disable coloring for ErrorLevel tag in console output
         * 
         * @param enabled Specifies whether to enable or disable coloring
         */
        void enableColors(bool enabled) {
            this->colorsEnabled = enabled;
        }
    private:
        bool colorsEnabled = true;
        /**
         * @brief Map that matches ErrorLevel to the corresponding ANSI color code
         * 
         */
        std::unordered_map<ErrorLevel, std::string> stringColorCodes = {
            {ErrorLevel::Info, "\033[32m"},
            {ErrorLevel::Warning, "\033[33m"},
            {ErrorLevel::Fatal, "\033[31m"}
        };

        std::unordered_map<ErrorLevel, std::function<void()>> actions;

        std::string errorLevelToString(ErrorLevel level) {
            switch(level) {
                case ErrorLevel::Info:
                    return "Info";
                    break;
                case ErrorLevel::Warning:
                    return "Warning";
                    break;
                case ErrorLevel::Fatal:
                    return "Fatal";
                    break;
                default:
                    return "Unknown";
                    break;      
            }
        }


    };
}