#include <vector>
#include <sstream>
#include <string>
#include <iostream>
#include "../../Group-07/Team07Application/Animals.hpp"
#include "../../Group-07/Team07Application/Simulation.hpp"
#include "../../Group-07/Team07Library/OutputLog.hpp"
#include "../../third-party/Catch/single_include/catch2/catch.hpp"

const int SEED = 42;

using cse::OutputLog;
using cse::LogLevel;

const std::array<double, 1> WEIGHT_ARR = {1.0};

TEST_CASE("Simulation_NoTimeSteps", "[Simulation]") {
    std::ostringstream capturedStream;
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::cout.rdbuf(capturedStream.rdbuf());

    OutputLog dummyLog(LogLevel::DEBUG, "dummy.log");
    dummyLog.enableConsoleOutput(true);

    SimulationState state(0, 1); // 0 time steps
    state.AddSpecies({"Rabbits", 100, 0.1, 0.05, 10, 2, "Grass"});
    state.AddSpecies({"Foxes", 10, 0.05, 0.1, 15, 3, "Rabbits"});
    state.AddInteraction({"Foxes", "Rabbits", 0.2});

    SimulationRunner runner(state, WEIGHT_ARR, dummyLog, SEED);
    runner.run();

    std::cout.rdbuf(oldCoutStreamBuf);

    auto rabbits = state.FindSpecies("Rabbits");
    auto foxes   = state.FindSpecies("Foxes");
    REQUIRE(rabbits.has_value());
    REQUIRE(foxes.has_value());
    REQUIRE(rabbits.value().get().population == 100); // no change
    REQUIRE(foxes.value().get().population == 10);

    std::string logs = capturedStream.str();
    REQUIRE(logs.find("Simulation complete.") != std::string::npos); // simulation should be compl.
}

TEST_CASE("Simulation_NoInteractions", "[Simulation]") {
    std::ostringstream capturedStream;
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::cout.rdbuf(capturedStream.rdbuf());

    OutputLog dummyLog(LogLevel::DEBUG, "dummy.log");
    dummyLog.enableConsoleOutput(true);

    SimulationState state(3, 1);
    state.AddSpecies({"Rabbits", 100, 0.1, 0.05, 10, 2, "Grass"});
    state.AddSpecies({"Foxes", 10, 0.05, 0.1, 15, 3, "None"}); 

    SimulationRunner runner(state, WEIGHT_ARR, dummyLog, SEED);
    runner.run();

    std::cout.rdbuf(oldCoutStreamBuf);

    auto rabbits = state.FindSpecies("Rabbits");
    auto foxes   = state.FindSpecies("Foxes");
    REQUIRE(rabbits.has_value());
    REQUIRE(foxes.has_value());
    REQUIRE(rabbits.value().get().population == 101);
    REQUIRE(foxes.value().get().population == 9);

    std::string logs = capturedStream.str();
    REQUIRE(logs.find("Simulation complete.") != std::string::npos);
}

TEST_CASE("Simulation_EarlyExtinction", "[Simulation]") {
    std::ostringstream capturedStream;
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::cout.rdbuf(capturedStream.rdbuf());

    OutputLog dummyLog(LogLevel::DEBUG, "dummy.log");
    dummyLog.enableConsoleOutput(true);

    // set br=0, dr=1 for both species to ensure extinction.
    SimulationState state(5, 1);
    state.AddSpecies({"Rabbits", 50, 0.0, 1.0, 10, 2, "Grass"});
    state.AddSpecies({"Foxes", 5, 0.0, 1.0, 15, 3, "Rabbits"});
    state.AddInteraction({"Foxes", "Rabbits", 0.2});

    SimulationRunner runner(state, WEIGHT_ARR, dummyLog, SEED);
    runner.run();

    std::cout.rdbuf(oldCoutStreamBuf);

    auto rabbits = state.FindSpecies("Rabbits");
    auto foxes   = state.FindSpecies("Foxes");
    REQUIRE(rabbits.has_value());
    REQUIRE(foxes.has_value());
    REQUIRE(rabbits.value().get().population == 0);
    REQUIRE(foxes.value().get().population == 0);

    std::string logs = capturedStream.str();
    REQUIRE(logs.find("Simulation ended early: all species went extinct.") != std::string::npos);
}

TEST_CASE("Simulation_MissingSpeciesInteraction", "[Simulation]") {
    std::ostringstream capturedStream;
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::cout.rdbuf(capturedStream.rdbuf());

    OutputLog dummyLog(LogLevel::DEBUG, "dummy.log");
    dummyLog.enableConsoleOutput(true);

    // Create a simulation state with 3 time steps and only one species.
    SimulationState state(3, 1);
    state.AddSpecies({"Rabbits", 100, 0.1, 0.05, 10, 2, "Grass"});
    state.AddInteraction({"Foxes", "Rabbits", 0.2}); // foxes does not exist in the simulation.

    SimulationRunner runner(state, WEIGHT_ARR, dummyLog, SEED);
    runner.run();

    std::cout.rdbuf(oldCoutStreamBuf);

    auto rabbits = state.FindSpecies("Rabbits");
    REQUIRE(rabbits.has_value());
    REQUIRE(rabbits.value().get().population == 100);

    std::string logs = capturedStream.str();
    REQUIRE(logs.find("Simulation complete.") != std::string::npos);
}

TEST_CASE("Simulation_Regular_Run", "[Simulation]") {
    std::ostringstream capturedStream;
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::cout.rdbuf(capturedStream.rdbuf());

    OutputLog dummyLog(LogLevel::DEBUG, "dummy.log");
    dummyLog.enableConsoleOutput(true);

    SimulationState state(3, 1);
    state.AddSpecies({"Rabbits", 100, 0.1, 0.05, 10, 2, "Grass"});
    state.AddSpecies({"Foxes", 10, 0.05, 0.1, 15, 3, "Rabbits"});
    state.AddInteraction({"Foxes", "Rabbits", 0.1}); 

    SimulationRunner runner(state, WEIGHT_ARR, dummyLog, SEED);
    runner.run();

    std::cout.rdbuf(oldCoutStreamBuf);

    auto rabbits = state.FindSpecies("Rabbits");
    auto foxes   = state.FindSpecies("Foxes");
    REQUIRE(rabbits.has_value());
    REQUIRE(foxes.has_value());
    REQUIRE(rabbits.value().get().population == 102); // 100 + births - deaths
    REQUIRE(foxes.value().get().population == 10);

    std::string logs = capturedStream.str();
    REQUIRE(logs.find("Simulation complete.") != std::string::npos);
}

TEST_CASE("Simulation_Invalid_Access", "[Simulation]") {
    std::ostringstream capturedStream;
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::cout.rdbuf(capturedStream.rdbuf());

    OutputLog dummyLog(LogLevel::DEBUG, "dummy.log");
    dummyLog.enableConsoleOutput(true);

    SimulationState state(3, 1);
    state.AddSpecies({"Rabbits", 100, 0.1, 0.05, 10, 2, "Grass"});
    state.AddSpecies({"Foxes", 10, 0.05, 0.1, 15, 3, "Rabbits"});
    state.AddInteraction({"Foxes", "Rabbits", 0.2});
    
    auto dogs = state.FindSpecies("Dogs");
    REQUIRE(!dogs.has_value()); // should not find the species

    SimulationRunner runner(state, WEIGHT_ARR, dummyLog, SEED);
    runner.run();
    std::cout.rdbuf(oldCoutStreamBuf);
    auto rabbits = state.FindSpecies("Rabbits");
    auto foxes   = state.FindSpecies("Foxes");

    // ensure the species are still present
    REQUIRE(rabbits.has_value()); 
    REQUIRE(foxes.has_value()); 
}