#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

class commandLine {
public:

    void addCommand(const std::string& command, std::function<void()> func) {
        // Add command to the map
        commands[command] = func;
    }

    void executeCommand(const std::string& command) {
        // Execute the command if it exists
        if (commands.find(command) != commands.end()) {
            commands[command]();
        } else {
            std::cout << "Unknown command: " << command << std::endl;
        }
    }

    void trackHistory(const std::string& command) {
        // Track command history
        history.push_back(command);
    }

    void printHistory() const {
        // Print command history
        for (const auto& cmd : history) {
            std::cout << cmd << std::endl;
        }
    }

    void trackState(const std::string& state) {
        // Track state
        currentState = state;
    }

    std::string getState() const {
        // Get current state
        return currentState;
    }

private:
    std::unordered_map<std::string, std::function<void()>> commands; // Map of commands to functions
    std::vector<std::string> history; // Vector of command history
    std::string currentState; // Current state
};

int main(){
    commandLine manager;
    manager.addCommand("greet", []() {
        std::cout << "Hello, World!" << std::endl;
    });

    manager.executeCommand("greet");
}
