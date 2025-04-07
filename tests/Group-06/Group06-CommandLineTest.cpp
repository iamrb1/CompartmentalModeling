#include <string>
#include <iostream>
#include <sstream>
#include "../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../Group-06/CommandLine/CommandLine.cpp"
#include "../../Group-06/StringSet/StringSet.hpp"
#include "../../Group-06/StaticString/StaticString.hpp"

// Test case for adding and executing commands
TEST_CASE("CommandLine addCommand and executeCommand", "[CommandLine]") {
    cse::CommandLine cli;

    bool commandExecuted = false;
    cli.addCommand("test", [&]() { commandExecuted = true; });

    // Test executing an existing command
    SECTION("Execute existing command") {
        cli.executeCommand("test");
        REQUIRE(commandExecuted == true);
    }

    // Test executing a non-existing command
    SECTION("Execute non-existing command") {
        std::ostringstream oss;
        std::streambuf* p_cout_streambuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());

        cli.executeCommand("nonexistent");

        std::cout.rdbuf(p_cout_streambuf);
        REQUIRE(oss.str() == "Unknown command: nonexistent\n");
    }
}

// Test case for tracking and printing command history
TEST_CASE("CommandLine trackHistory and printHistory", "[CommandLine]") {
    cse::CommandLine cli;

    cli.trackHistory("command1");
    cli.trackHistory("command2");

    // Test printing the command history
    SECTION("Print history") {
        std::ostringstream oss;
        std::streambuf* p_cout_streambuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());

        cli.printHistory();

        std::cout.rdbuf(p_cout_streambuf);
        REQUIRE(oss.str() == "command1\ncommand2\n");
    }
}

// Test case for tracking and getting the current state
TEST_CASE("CommandLine trackState and getState", "[CommandLine]") {
    cse::CommandLine cli;

    cli.trackState("state1");
    REQUIRE(cli.getState() == "state1");

    cli.trackState("state2");
    REQUIRE(cli.getState() == "state2");
}

// Test case for adding multiple commands and executing them
TEST_CASE("CommandLine add multiple commands and execute", "[CommandLine]") {
    cse::CommandLine cli;

    bool command1Executed = false;
    bool command2Executed = false;

    cli.addCommand("command1", [&]() { command1Executed = true; });
    cli.addCommand("command2", [&]() { command2Executed = true; });

    // Test executing the first command
    SECTION("Execute command1") {
        cli.executeCommand("command1");
        REQUIRE(command1Executed == true);
        REQUIRE(command2Executed == false);
    }

    // Test executing the second command
    SECTION("Execute command2") {
        cli.executeCommand("command2");
        REQUIRE(command1Executed == false);
        REQUIRE(command2Executed == true);
    }
}

// Test case for printing an empty command history
TEST_CASE("CommandLine track and print empty history", "[CommandLine]") {
    cse::CommandLine cli;

    // Test printing the history when it is empty
    SECTION("Print empty history") {
        std::ostringstream oss;
        std::streambuf* p_cout_streambuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());

        cli.printHistory();

        std::cout.rdbuf(p_cout_streambuf);
        REQUIRE(oss.str().empty());
    }
}

// Test case for clearing the command history
TEST_CASE("CommandLine clearHistory", "[CommandLine]") {
    cse::CommandLine cli;

    cli.trackHistory("command1");
    cli.trackHistory("command2");

    // Test clearing the command history
    SECTION("Clear history") {
        cli.clearHistory();
        std::ostringstream oss;
        std::streambuf* p_cout_streambuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());

        cli.printHistory();

        std::cout.rdbuf(p_cout_streambuf);
        REQUIRE(oss.str().empty());
    }
}

// Test case for the help command
TEST_CASE("CommandLine help command", "[CommandLine]") {
    cse::CommandLine cli;

    cli.addCommand("test", []() {}, "A test command");

    // Test printing the help message
    SECTION("Print help") {
        std::ostringstream oss;
        std::streambuf* p_cout_streambuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());

        cli.executeCommand("help");

        std::cout.rdbuf(p_cout_streambuf);
        REQUIRE(oss.str() == "Available commands:\nhelp - Displays this help message\ntest - A test command\n");
    }
}
// Test case for adding and executing commands from StringSet
TEST_CASE("CommandLine with StringSet commands", "[CommandLine][StringSet]") {
    cse::CommandLine cli;
    cse::StringSet<std::string> stringSet;

    // Add a command to insert a string into the StringSet
    cli.addCommand("add_to_set", [&]() {
        stringSet.insert("test_string");
    }, "Adds a string to the StringSet");

    // Add a command to print the contents of the StringSet
    cli.addCommand("print_set", [&]() {
        std::ostringstream oss;
        for (const auto& str : stringSet) {
            oss << str << " ";
        }
        std::cout << oss.str() << std::endl;
    }, "Prints all strings in the StringSet");

    // Test adding a string to the StringSet
    SECTION("Add string to StringSet") {
        cli.executeCommand("add_to_set");
        REQUIRE(stringSet.count("test_string") == 1);
    }

    // Test printing the StringSet
    SECTION("Print StringSet contents") {
        stringSet.insert("test_string");
        std::ostringstream oss;
        std::streambuf* p_cout_streambuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());

        cli.executeCommand("print_set");

        std::cout.rdbuf(p_cout_streambuf);
        REQUIRE(oss.str() == "test_string \n");
    }
}

// Test case for adding and executing commands from StaticString
TEST_CASE("CommandLine with StaticString commands", "[CommandLine][StaticString]") {
    cse::CommandLine cli;
    cse::StaticString<10> staticStr;

    // Add a command to set the value of the StaticString
    cli.addCommand("set_static", [&]() {
        staticStr = "hello";
    }, "Sets the StaticString value");

    // Add a command to print the value of the StaticString
    cli.addCommand("print_static", [&]() {
        std::cout << staticStr << std::endl;
    }, "Prints the StaticString value");

    // Test setting the StaticString value
    SECTION("Set StaticString value") {
        cli.executeCommand("set_static");
        REQUIRE(staticStr == "hello");
    }

    // Test printing the StaticString value
    SECTION("Print StaticString value") {
        staticStr = "world";
        std::ostringstream oss;
        std::streambuf* p_cout_streambuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());

        cli.executeCommand("print_static");

        std::cout.rdbuf(p_cout_streambuf);
        REQUIRE(oss.str() == "world\n");
    }
}

// Test case for combining StringSet and StaticString commands
TEST_CASE("CommandLine with combined StringSet and StaticString commands", "[CommandLine][StringSet][StaticString]") {
    cse::CommandLine cli;
    cse::StringSet<std::string> stringSet;
    cse::StaticString<10> staticStr;

    // Add a command to insert the StaticString value into the StringSet
    cli.addCommand("add_static_to_set", [&]() {
        stringSet.insert(staticStr.to_string());
    }, "Adds the StaticString value to the StringSet");

    // Test adding the StaticString value to the StringSet
    SECTION("Add StaticString value to StringSet") {
        staticStr = "example";
        cli.executeCommand("add_static_to_set");
        REQUIRE(stringSet.count("example") == 1);
    }
}

TEST_CASE("Test for tokenizing lines", "[tokenize_line]") {
    cse::CommandLine cli;

    SECTION("One-word string tokenized into one-word string") {
        std::string str = "Hello";
        
        auto result = cli.tokenize_line(str);

        REQUIRE(result.size() == 1);
        REQUIRE(result[0] == "Hello");
    }

    SECTION("Multiple-word string is tokenized appropriately") {
        std::string str = "Hello how are you";

        auto result = cli.tokenize_line(str);
        REQUIRE(result.size() == 4);
        REQUIRE(result[0] == "Hello");
        REQUIRE(result[1] == "how");
        REQUIRE(result[2] == "are");
        REQUIRE(result[3] == "you");
    }
}