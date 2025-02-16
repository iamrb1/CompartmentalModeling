#pragma once
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

namespace cse {
/**
 * @brief A class for convenient management and output of different types of
 * errors
 *
 *
 */
class ErrorManager {
 public:
  /**
   * @brief Possible levels of errors
   *
   */
  enum class ErrorLevel { Info, Warning, Fatal };

  /**
   * @brief Executes a function and displays an error if function throws during
   * execution
   *
   * @param function Function to be executed
   * @param message Error message to be displayed if a function fails
   * @param level Error level of a message
   */
  void executeAndHandleError(const std::function<void()>& function,
                             const std::string& message = "",
                             ErrorLevel level = ErrorLevel::Info) {
    assert(function);

    try {
      function();
    } catch (const std::exception& e) {
      printError(message, level);
    }
  }
  /**
   * @brief Writes a message to console with a message type at the start of a
   * line
   *
   * @param message Message to be sent to user
   * @param level Error level of a message. Defaults to "Info" if not specified
   */
  void printError(const std::string& message,
                  ErrorLevel level = ErrorLevel::Info) {
    printError(0, message, level);
  }

  /**
   * @brief Writes a message to console with a message type at line number
   *
   * @param line Line number where error occured
   * @param message Message to be sent to user
   * @param level Error level of a message. Defaults to "Info"
   */
  void printError(int32_t line, const std::string& message,
                  ErrorLevel level = ErrorLevel::Info) {
    assert(!message.empty());
    assert(mStringColorCodes.find(level) != mStringColorCodes.end());
    assert(mTerminationEnabled.find(level) != mTerminationEnabled.end());

    std::stringstream ss;
    ss << "[";
    if (mColorsEnabled) {
      ss << mStringColorCodes[level];
    }
    ss << errorLevelToString(level);
    if (mColorsEnabled) {
      ss << COLOR_RESET;
    }
    ss << "]";

    assert(line >= 0);

    if (line > 0) {
      ss << " (line " << line << ")";
    }
    ss << ": " << message << "\n";
    std::cout << ss.str();

    if (actions.find(level) != actions.end()) {
      actions[level]();
    }

    int code = mTerminationEnabled[level];
    if (code > 0) {
      exit(code);
    }
  }

  /**
   * @brief Set the Action that will be invoked when an error of a certain
   * ErrorType occurs
   *
   * @param level Error Level to which action will be assigned
   * @param action Function that will be called. Does not accept arguments and
   * returns void
   */
  void setAction(ErrorLevel level, const std::function<void()>& action) {
    assert(action);

    actions[level] = action;
  }

  ErrorManager() {}

  /**
   * @brief Allows to enable or disable coloring for ErrorLevel tag in console
   * output
   *
   * @param enabled Specifies whether to enable or disable coloring
   */
  void enableColors(bool enabled) { this->mColorsEnabled = enabled; }

  /**
   * @brief Allows to enable termination via exit() if an error of a specified
   * type occurs
   *
   * @param level The error level for which termination will be enabled or
   * disabled
   * @param enabled Enables or disables termination of the program
   * @param statusCode optional parameter: specifies which status code to use
   * for exit()
   */
  void enableTermination(ErrorLevel level, bool enabled,
                         int32_t statusCode = 1) {
    assert(statusCode >= 0);

    if (enabled) {
      mTerminationEnabled[level] = statusCode;
    } else {
      mTerminationEnabled[level] = 0;
    }
  }

 private:
  static constexpr const char* COLOR_GREEN = "\033[32m";
  static constexpr const char* COLOR_YELLOW = "\033[33m";
  static constexpr const char* COLOR_RED = "\033[31m";
  static constexpr const char* COLOR_RESET = "\033[0m";
  static constexpr const char* INFO_STRING = "Info";
  static constexpr const char* WARNING_STRING = "Warning";
  static constexpr const char* FATAL_STRING = "Fatal";

  bool mColorsEnabled = true;
  /**
   * @brief Map that matches ErrorLevel to the corresponding ANSI color code
   *
   */
  std::unordered_map<ErrorLevel, std::string> mStringColorCodes = {
      {ErrorLevel::Info, COLOR_GREEN},
      {ErrorLevel::Warning, COLOR_YELLOW},
      {ErrorLevel::Fatal, COLOR_RED}};

  std::unordered_map<ErrorLevel, int32_t> mTerminationEnabled = {
      {ErrorLevel::Info, 0}, {ErrorLevel::Warning, 0}, {ErrorLevel::Fatal, 0}};

  std::unordered_map<ErrorLevel, std::function<void()>> actions;

  std::string errorLevelToString(ErrorLevel level) const {
    switch (level) {
      case ErrorLevel::Info:
        return INFO_STRING;
        break;
      case ErrorLevel::Warning:
        return WARNING_STRING;
        break;
      case ErrorLevel::Fatal:
        return FATAL_STRING;
        break;
      default:
        assert(false &&
               "Encountered an unknown ErrorLevel in errorLevelToString");
        return "Unknown";  // to prevent compiler warnings
        break;
    }
  }
};
}  // namespace cse