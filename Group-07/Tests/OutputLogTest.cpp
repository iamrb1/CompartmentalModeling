/*
* OutputLogTest.cpp
 */
#include <gtest/gtest.h>
#include "../Team07Library/OutputLog.hpp"
#include <fstream>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <string>

// Test Constructor
TEST(OutputLogTest, Construct) {
    OutputLog logger(LogLevel::NORMAL, "test_log.txt");
}

// Test Normal Log Writing
TEST(OutputLogTest, NormalLogging) {
    OutputLog logger(LogLevel::NORMAL, "test_log.txt");
    logger.log("This is a NORMAL log message.", LogLevel::NORMAL);

    std::ifstream logFile("test_log.txt");
    std::string line;
    bool found = false;

    while (getline(logFile, line)) {
        if (line == "This is a NORMAL log message.") {
            found = true;
            break;
        }
    }

    ASSERT_TRUE(found);
}


// Test Debug Logging (Allowed when LogLevel is DEBUG)
TEST(OutputLogTest, DebugLoggingAllowed) {
    OutputLog logger(LogLevel::DEBUG, "test_log.txt");
    logger.log("This is a DEBUG log message.", LogLevel::DEBUG);

    std::ifstream logFile("test_log.txt");
    std::string line;
    bool found = false;

    while (getline(logFile, line)) {
        if (line == "This is a DEBUG log message.") {
            found = true;
            break;
        }
    }

    ASSERT_TRUE(found);
}

// Test File Opening Failure (Invalid Filename)


// Test Destructor Properly Closes File
TEST(OutputLogTest, DestructorClosesFile) {
    std::string filename = "test_log.txt";
    {
        OutputLog logger(LogLevel::NORMAL, filename);
    }
    std::ifstream testFile(filename);
    ASSERT_TRUE(testFile.good());  // File should be accessible
}

// Test Output to Console
TEST(OutputLogTest, ConsoleOutput) {
    OutputLog logger(LogLevel::NORMAL, "test_log.txt");

    // Redirect std::cout
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    logger.log("Console message", LogLevel::NORMAL);

    // Restore std::cout
    std::cout.rdbuf(oldCout);

    ASSERT_EQ(buffer.str(), "Console message\n");
}


