#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <algorithm>

namespace cse {

    /**
     * @brief A class for users to create their own command line interface
     * 
     */
    class CommandLine {
    public:

        CommandLine() {
            // Add a default help command
            addCommand("help", [this]() { printHelp(); }, "Displays this help message");
        }

        /**
         * @brief Adds a command to the command map
         * 
         * @param command The command string
         * @param func The function to be executed for the command
         * @param description The description of the command
         */
        void addCommand(const std::string& command, std::function<void()> func, const std::string& description = "") {
            // Add command to the map
            commands[command] = func;
            descriptions[command] = description;
        }

        /**
         * @brief Executes a command if it exists in the command map
         * 
         * @param command The command string to be executed
         */
        void executeCommand(const std::string& command) {
            // Execute the command if it exists
            if (commands.find(command) != commands.end()) {
                try {
                    commands[command]();
                    trackHistory(command);
                } catch (const std::exception& e) {
                    std::cerr << "Error executing command: " << e.what() << std::endl;
                }
            } else {
                std::cout << "Unknown command: " << command << std::endl;
            }
        }

        /**
         * @brief Tracks the history of executed commands
         * 
         * @param command The command string to be tracked
         */
        void trackHistory(const std::string& command) {
            // Track command history
            history.push_back(command);
        }

        /**
         * @brief Prints the history of executed commands
         * 
         */
        void printHistory() const {
            // Print command history
            for (const auto& cmd : history) {
                std::cout << cmd << std::endl;
            }
        }

        /**
         * @brief Clears the history of executed commands
         * 
         */
        void clearHistory() {
            history.clear();
        }

        /**
         * @brief Tracks the current state
         * 
         * @param state The state string to be tracked
         */
        void trackState(const std::string& state) {
            // Track state
            currentState = state;
        }

        /**
         * @brief Gets the current state
         * 
         * @return std::string The current state
         */
        std::string getState() const {
            // Get current state
            return currentState;
        }

        /**
         * @brief Prints the help message with all available commands
         * 
         */
        void printHelp() const {
            std::cout << "Available commands:" << std::endl;
            // Create a vector of command names and sort it
            std::vector<std::string> sortedCommands;
            for (const auto& cmd : descriptions) {
                sortedCommands.push_back(cmd.first);
            }
            std::sort(sortedCommands.begin(), sortedCommands.end());
            // Print the sorted commands with their descriptions
            for (const auto& cmd : sortedCommands) {
                std::cout << cmd << " - " << descriptions.at(cmd) << std::endl;
            }
        }

    private:
        std::unordered_map<std::string, std::function<void()>> commands; // Map of commands to functions
        std::unordered_map<std::string, std::string> descriptions; // Map of commands to descriptions
        std::vector<std::string> history; // Vector of command history
        std::string currentState; // Current state
    };

}

// Ivan: moved this test to the main.cpp in Group-06 folder
/*
int main() {
    cse::CommandLine cmd;

    // Add some commands
    cmd.addCommand("greet", []() { std::cout << "Hello, World!" << std::endl; });
    cmd.addCommand("farewell", []() { std::cout << "Goodbye, World!" << std::endl; });

    // Execute commands
    cmd.executeCommand("greet");
    cmd.executeCommand("farewell");
    cmd.executeCommand("unknown"); // This should print "Unknown command: unknown"

    // Track and print history
    cmd.trackHistory("greet");
    cmd.trackHistory("farewell");
    cmd.printHistory();

    // Track and get state
    cmd.trackState("active");
    std::cout << "Current state: " << cmd.getState() << std::endl;

    return 0;
}*/