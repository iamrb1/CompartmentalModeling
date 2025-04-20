#ifndef CSE_COMMAND_LINE_CPP_
#define CSE_COMMAND_LINE_CPP_

#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <any>

namespace cse {
/**
 * @brief A call for managing a command-line process.
 * 
 */
class CommandLine {
public:
  CommandLine() {
    // Add a default help command
    addCommand("help", [this]() -> void {
      printHelp();
    }, "Displays this help message");
  }

  /**
   * @brief Adds a command to the command map
   *
   * @tparam Func The type of the function
   * @param command The command string
   * @param func The function to be executed for the command
   * @param description The description of the command
   */
  template <typename Func>
  void addCommand(const std::string& command, Func func, const std::string& description = "") {
    mCommands[command] = [func]() -> std::any {
      if constexpr (std::is_same_v<std::invoke_result_t<Func>, void>) {
        func();
        return std::any(); // Return an empty std::any for void functions
      } else {
        return func();
      }
    };
    mDescriptions[command] = description;
  }

  /**
   * @brief Executes a command if it exists in the command map
   *
   * @param command The command string
   * @return std::any The result of the command
   */
  std::any executeCommand(const std::string& command) {
    if (mCommands.find(command) != mCommands.end()) {
      try {
        auto result = mCommands[command]();
        trackHistory(command);
        return result;
      } catch (const std::exception& e) {
        std::cerr << "Error executing command: " << e.what() << std::endl;
      }
    } else {
      std::cout << "Unknown command: " << command << std::endl;
    }
    return std::any(); // Return an empty std::any if the command is not found or fails
  }

  /**
   * @brief Prints the help message with all available commands
   */
  void printHelp() const {
    std::cout << "Available commands:" << std::endl;

    std::vector<std::string> sortedCommands;
    for (const auto& cmd : mDescriptions) {
      sortedCommands.push_back(cmd.first);
    }

    std::sort(sortedCommands.begin(), sortedCommands.end());

    for (const auto& cmd : sortedCommands) {
      std::cout << cmd << " - " << mDescriptions.at(cmd) << std::endl;
    }
  }

private:
  /**
   * @brief adds a command to history
   * 
   * @param command command to be added 
   */
  void trackHistory(const std::string& command) { mHistory.push_back(command); }

  /**
   * @brief a container which maps commands to functions
   * 
   */
  std::unordered_map<std::string, std::function<std::any()>> mCommands;

  /**
   * @brief a container which maps commands to their descriptions
   * 
   */
  std::unordered_map<std::string, std::string> mDescriptions;

  /**
   * @brief a vector which keeps a list of commands for history purposes
   * 
   */
  std::vector<std::string> mHistory;
};

}  // namespace cse

#endif  // CSE_COMMAND_LINE_CPP_
