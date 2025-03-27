#include <vector>
#include <sstream>
#include <string>
#include <iostream>
#include "../../Group-07/Team07Application/Animals.hpp"
#include "../../Group-07/Team07Application/Simulation.hpp"
#include "../../Group-07/Team07Library/OutputLog.hpp"
#include "../../third-party/Catch/single_include/catch2/catch.hpp"


using cse::OutputLog;
using cse::LogLevel;
//
// Test 1: No Time Steps
// If timeSteps is zero, the simulation should immediately complete and species populations remain unchanged.
//
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

    runSimulation(state, dummyLog);

    std::cout.rdbuf(oldCoutStreamBuf);

    Species* rabbits = state.FindSpecies("Rabbits");
    Species* foxes   = state.FindSpecies("Foxes");
    REQUIRE(rabbits != nullptr);
    REQUIRE(foxes != nullptr);
    REQUIRE(rabbits->population == 100); // no change
    REQUIRE(foxes->population == 10);

    std::string logs = capturedStream.str();
    REQUIRE(logs.find("Simulation complete.") != std::string::npos); // simulation should be compl.
}

//
// Test 2: No Interactions
// When there are no interactions, only births and deaths affect the populations.
//
TEST_CASE("Simulation_NoInteractions", "[Simulation]") {
    std::ostringstream capturedStream;
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::cout.rdbuf(capturedStream.rdbuf());

    OutputLog dummyLog(LogLevel::DEBUG, "dummy.log");
    dummyLog.enableConsoleOutput(true);

    SimulationState state(3, 1);
    state.AddSpecies({"Rabbits", 100, 0.1, 0.05, 10, 2, "Grass"});
    state.AddSpecies({"Foxes", 10, 0.05, 0.1, 15, 3, "None"}); 


    runSimulation(state, dummyLog);

    std::cout.rdbuf(oldCoutStreamBuf);

    // pop change only based on birth and death rates.
    Species* rabbits = state.FindSpecies("Rabbits");
    Species* foxes   = state.FindSpecies("Foxes");
    REQUIRE(rabbits != nullptr);
    REQUIRE(foxes != nullptr);
    REQUIRE(rabbits->population == 116);
    REQUIRE(foxes->population == 9);

    std::string logs = capturedStream.str();
    REQUIRE(logs.find("Simulation complete.") != std::string::npos);
}

//
// Test 3: Early Extinction
// Configure the simulation so that both species go extinct quickly.
// This tests the early termination when all species are extinct.
//
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

    runSimulation(state, dummyLog);

    std::cout.rdbuf(oldCoutStreamBuf);
    // Expect both species to go extinct.
    Species* rabbits = state.FindSpecies("Rabbits");
    Species* foxes   = state.FindSpecies("Foxes");
    REQUIRE(rabbits != nullptr);
    REQUIRE(foxes != nullptr);
    REQUIRE(rabbits->population == 0);
    REQUIRE(foxes->population == 0);

    std::string logs = capturedStream.str();
    // simulation should end early due to extinction.
    REQUIRE(logs.find("Simulation ended early: all species went extinct.") != std::string::npos);
}

//
// Test 4: Missing Species in Interaction
// If an interaction refers to a species that is not present in the simulation,
// the simulation should skip that interaction without error.
//
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

    runSimulation(state, dummyLog);

    std::cout.rdbuf(oldCoutStreamBuf);

    // Since "Foxes" does not exist, only the rabbits should update via births and deaths.
    Species* rabbits = state.FindSpecies("Rabbits");
    REQUIRE(rabbits != nullptr);
    REQUIRE(rabbits->population == 116);

    std::string logs = capturedStream.str();
    REQUIRE(logs.find("Simulation complete.") != std::string::npos);
}

// Normal Simulation
// Run a normal simulation with multiple species and interactions.
//
TEST_CASE("Simulation_Regular_Run", "[Simulation]") {
    std::ostringstream capturedStream;
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::cout.rdbuf(capturedStream.rdbuf());

    OutputLog dummyLog(LogLevel::DEBUG, "dummy.log");
    dummyLog.enableConsoleOutput(true);

    // Create a simulation state with 3 time steps and only one species.
    SimulationState state(3, 1);
    state.AddSpecies({"Rabbits", 100, 0.1, 0.05, 10, 2, "Grass"});
    state.AddSpecies({"Foxes", 10, 0.05, 0.1, 15, 3, "Rabbits"});
    state.AddInteraction({"Foxes", "Rabbits", 0.1}); 

    runSimulation(state, dummyLog);

    std::cout.rdbuf(oldCoutStreamBuf);

    Species* rabbits = state.FindSpecies("Rabbits");
    REQUIRE(rabbits != nullptr);
    Species* foxes = state.FindSpecies("Foxes");
    REQUIRE(foxes != nullptr);

    REQUIRE(rabbits->population == 114);
    REQUIRE(foxes->population == 9);

    std::string logs = capturedStream.str();
    REQUIRE(logs.find("Simulation complete.") != std::string::npos);

}
