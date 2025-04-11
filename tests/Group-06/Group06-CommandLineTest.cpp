#include <string>
#include <iostream>
#include <sstream>
#include "../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../Group-06/CommandLine/CommandLine.cpp"
#include "../../Group-06/StringSet/StringSet.hpp"
#include "../../Group-06/StaticString/StaticString.hpp"

TEST_CASE("CommandLine with StaticString and StringSet", "[CommandLine][StaticString][StringSet]") {
    cse::CommandLine cli;
    cse::StringSet<std::string> stringSet;
    cse::StaticString<10> staticStr;

    // Add a command to set the StaticString value
    cli.addCommand("set_static", [&]() -> std::string {
        staticStr = "example";
        return "StaticString set to 'example'";
    }, "Sets the StaticString value");

    // Add a command to get the StaticString value
    cli.addCommand("get_static", [&]() -> std::string {
        return staticStr.to_string();
    }, "Gets the StaticString value");

    // Add a command to insert a value into the StringSet
    cli.addCommand("add_to_set", [&]() -> std::string {
        stringSet.insert(staticStr.to_string());
        return "Value added to StringSet";
    }, "Adds the StaticString value to the StringSet");

    // Add a command to check if a value exists in the StringSet
    cli.addCommand("check_in_set", [&]() -> bool {
        return stringSet.count(staticStr.to_string()) > 0;
    }, "Checks if the StaticString value exists in the StringSet");

    // Add a command to get the size of the StringSet
    cli.addCommand("set_size", [&]() -> int {
        return stringSet.size();
    }, "Gets the size of the StringSet");

    // Test setting the StaticString value
    SECTION("Set StaticString value") {
        auto result = cli.executeCommand("set_static");
        REQUIRE(result.has_value());
        REQUIRE(std::any_cast<std::string>(result) == "StaticString set to 'example'");
        REQUIRE(staticStr == "example");
    }

    // Test getting the StaticString value
    SECTION("Get StaticString value") {
        staticStr = "example";
        auto result = cli.executeCommand("get_static");
        REQUIRE(result.has_value());
        REQUIRE(std::any_cast<std::string>(result) == "example");
    }

    // Test adding a value to the StringSet
    SECTION("Add value to StringSet") {
        staticStr = "example";
        auto result = cli.executeCommand("add_to_set");
        REQUIRE(result.has_value());
        REQUIRE(std::any_cast<std::string>(result) == "Value added to StringSet");
        REQUIRE(stringSet.count("example") == 1);
    }

    // Test checking if a value exists in the StringSet
    SECTION("Check if value exists in StringSet") {
        staticStr = "example";
        stringSet.insert("example");
        auto result = cli.executeCommand("check_in_set");
        REQUIRE(result.has_value());
        REQUIRE(std::any_cast<bool>(result) == true);
    }

    // Test getting the size of the StringSet
    SECTION("Get size of StringSet") {
        stringSet.insert("example");
        stringSet.insert("another");
        auto result = cli.executeCommand("set_size");
        REQUIRE(result.has_value());
        REQUIRE(std::any_cast<int>(result) == 2);
    }
}

TEST_CASE("CommandLine error handling", "[CommandLine][ErrorHandling]") {
    cse::CommandLine cli;

    // Add a command that throws an exception
    cli.addCommand("throw_error", []() -> void {
        throw std::runtime_error("Test exception");
    }, "Throws an exception");

    // Test executing an unknown command
    SECTION("Unknown command") {
        std::ostringstream oss;
        std::streambuf* p_cout_streambuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());

        auto result = cli.executeCommand("unknown_command");

        std::cout.rdbuf(p_cout_streambuf);
        REQUIRE(!result.has_value());
        REQUIRE(oss.str() == "Unknown command: unknown_command\n");
    }

    // Test executing a command that throws an exception
    SECTION("Command throws an exception") {
        std::ostringstream oss;
        std::streambuf* p_cerr_streambuf = std::cerr.rdbuf();
        std::cerr.rdbuf(oss.rdbuf());

        auto result = cli.executeCommand("throw_error");

        std::cerr.rdbuf(p_cerr_streambuf);
        REQUIRE(!result.has_value());
        REQUIRE(oss.str() == "Error executing command: Test exception\n");
    }
}