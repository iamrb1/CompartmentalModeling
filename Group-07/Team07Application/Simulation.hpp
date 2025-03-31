/*
Authors: Bao Tran, Siddhartha Bhattacharya
Note: GitHub Copilot was used to write docstrings for the functions in this file

This file contains the implementation of the SimulationRunner class, 
which is responsible for running the simulation based on the provided SimulationState configuration. 
The SimulationRunner class processes predator-prey interactions, births, and deaths for each species in the simulation. 
It also logs detailed simulation statistics for each time step and checks for early extinction of all species.
*/
#ifndef SIMULATION_HPP
#define SIMULATION_HPP
#include <iostream>
#include <ranges>
#include <vector>
#include <random>
#include <algorithm>
#include <string>
#include <sstream>
#include <unordered_map>
#include "../Team07Library/OutputLog.hpp"
#include "../Team07Library/Scheduler.hpp"
#include "Animals.hpp"
using namespace cse;


/**
 * @brief Add animals to the simulation
 * 
 * Adds a specified number of animals to the simulation state,
 * creating new Animal instances with unique IDs and species names.
 * 
 * @param count Number of animals to add
 * @param state Reference to the simulation state
 * @param scheduler Reference to the scheduler for process management
 * @param id Reference to the current animal ID counter
 * @param speciesName Name of the species for the new animals
 * @param preyName Name of the prey species for the new animals
 * 
 * @note This function updates the simulation state and scheduler in place.
 */
void AddAnimals(int count, SimulationState& state, cse::Scheduler<int>& scheduler, int& id, const std::string& speciesName, const std::string& preyName) {
    for (auto i : std::views::iota(0, count)) {
        (void)i; // suppress unused variable warning
        
        Animal animal(id, speciesName, preyName, preyName != "None");
        state.animals.push_back(animal);
        
        std::vector<double> weights = animal.getWeights();
        scheduler.AddProcess(id, weights);
        id++;
    }
}
class SimulationRunner {
    private:
        SimulationState& state;
        OutputLog& log;
        cse::Scheduler<int> scheduler;
        std::mt19937 gen;
        int id;
        
        /**
         * @brief Process predator-prey interactions for the current time step
         * 
         * This function handles hunting interactions between predator and prey species.
         * For each defined interaction:
         * - Calculates the number of prey animals to be hunted
         * - Removes prey animals from the simulation
         * - Updates predator and prey population counts
         * - Adds new predator animals based on successful hunts
         * 
         * @param speciesPopulations Reference to a map tracking current populations of each species
         */
        void processInteractions(std::unordered_map<std::string, int>& speciesPopulations) {
            for (const auto& interaction : state.interactions) {
                Species* predator = state.FindSpecies(interaction.predator);
                Species* prey = state.FindSpecies(interaction.prey);
    
                if (predator && prey && prey->population > 0) {
                    int toEat = std::min(prey->population, 
                        static_cast<int>(speciesPopulations[predator->name] * interaction.huntRate));
                    toEat = std::min(toEat, static_cast<int>(
                        std::count_if(state.animals.begin(), state.animals.end(), 
                            [&prey](const Animal& a) { return a.species == prey->name; })
                    ));
    
                    // simulate hunting by using scheduler 
                    for (auto i : std::views::iota(0, toEat)) {
                        (void)i; 
                        auto predID = scheduler.PopNextProcess(); 
                        if (!predID) break;
    
                        auto preyIt = std::find_if(state.animals.begin(), state.animals.end(), 
                            [&prey](const Animal& a) { return a.species == prey->name; });
                        
                        if (preyIt != state.animals.end()) {
                            prey->population -= 1;
                            
                            auto predIt = std::find_if(state.animals.begin(), state.animals.end(), 
                                [predID](const Animal& a) { return a.id == *predID; });
                            
                            if (predIt != state.animals.end()) {
                                predIt->lastEaten += 1;
                            }
    
                            state.animals.erase(preyIt);
                        }
                    }
                    AddAnimals(toEat/2, state, scheduler, id, interaction.predator, interaction.prey);
                }
            }
        }
    
        /**
         * @brief Process births and deaths for a specific species
         * 
         * This function handles population dynamics for a given species:
         * - Calculates birth and death counts based on current population and rates
         * - Adds new animals through births
         * - Removes animals through deaths
         * - Updates the species population count
         * 
         * @param birthCount Reference to store the number of births
         * @param deathCount Reference to store the number of deaths
         * @param species Reference to the species being processed
         */
        void processBirthsAndDeaths(int& birthCount, int& deathCount, Species& species) {
            int currentPop = std::count_if(state.animals.begin(), state.animals.end(), 
                [&species](const Animal& a) { return a.species == species.name; });
            
            birthCount = static_cast<int>(currentPop * species.birthRate);
            deathCount = static_cast<int>(currentPop * species.deathRate);
    
            // Add new animals
            AddAnimals(birthCount, state, scheduler, id, species.name, species.foodSource);
    
            // Randomly select animals to remove
            std::shuffle(state.animals.begin(), state.animals.end(), gen);
            std::vector<int> indicesToRemove;
            for (size_t j = 0; j < std::min(static_cast<size_t>(deathCount), state.animals.size()); ++j) {
                if (state.animals[j].species == species.name) {
                    indicesToRemove.push_back(j);
                    scheduler.RemoveProcess(state.animals[j].id);
                }
            }
            
            // remove in reverse order to avoid invalidating indices
            std::ranges::sort(indicesToRemove, std::greater{}); 
            for (int index : indicesToRemove) {
                state.animals.erase(state.animals.begin() + index);
            }
            
            // update pop for species
            species.population = std::count_if(state.animals.begin(), state.animals.end(), 
                [&species](const Animal& a) { return a.species == species.name; });
            species.population = std::max(0, species.population);
        }
    
        /**
         * @brief Check if all species have gone extinct
         * 
         * Determines whether all species in the simulation have zero population.
         * 
         * @return bool True if all species have gone extinct, false otherwise
         */
        bool checkExtinction() {
            for (const auto& species : state.speciesList) {
                if (species.population > 0) {
                    return false;
                }
            }
            return true;
        }
    
        /**
         * @brief Log detailed simulation statistics for the current time step
         * 
         * Logs verbose information about:
         * - Current time step
         * - Births for each species
         * - Deaths for each species
         * - Current population for each species
         * 
         * @param timeStep Current simulation time step
         * @param birthCounts Vector of birth counts for each species
         * @param deathCounts Vector of death counts for each species
         */
        void logSimulationDetails(int timeStep, 
                                   const std::vector<int>& birthCounts, 
                                   const std::vector<int>& deathCounts) {
            log.log("=== Time Step " + std::to_string(timeStep) + " ===", LogLevel::VERBOSE);
    
            for (size_t i = 0; i < state.speciesList.size(); ++i) {
                const auto& species = state.speciesList[i];
                log.log(species.name + " Births: " + std::to_string(birthCounts[i]), LogLevel::DEBUG);
                log.log(species.name + " Deaths: " + std::to_string(deathCounts[i]), LogLevel::DEBUG);
            }
    
            for (const auto& species : state.speciesList) {
                log.log(species.name + " Population: " + std::to_string(species.population), LogLevel::DEBUG);
            }
        }
    
    public:
        /**
         * @brief Construct a new Simulation Runner
         * 
         * Initializes the simulation with the given state, logging, and optional random seed
         * 
         * @param simulationState Reference to the simulation state configuration
         * @param outputLog Reference to the output logging mechanism
         * @param seed Random seed for reproducibility, defaults to 42
         */
        SimulationRunner(SimulationState& simulationState, OutputLog& outputLog, int seed = 42)
            : state(simulationState), log(outputLog), scheduler({-1}), gen(seed), id(0) {
            state.PopulateInitialAnimals(id);
        }
    
        /**
         * @brief Run the full simulation
         * 
         * Executes the simulation loop:
         * - Process predator-prey interactions
         * - Process births and deaths
         * - Log simulation details
         * - Check for extinction
         * 
         * Logs the start and end of the simulation, 
         * and will terminate early if all species go extinct
         */
        void run() {
            log.log("Starting simulation...", LogLevel::NORMAL);
    
            // vecs to track births and deaths
            std::vector<int> birthCounts(state.speciesList.size(), 0);
            std::vector<int> deathCounts(state.speciesList.size(), 0);
    
            // used to track pop of each species
            std::unordered_map<std::string, int> speciesPopulations;
            for (const auto& species : state.speciesList) {
                speciesPopulations[species.name] = 0;
            }
    
            // main simulation loop
            for (int t = 0; t < state.timeSteps; ++t) {
                processInteractions(speciesPopulations);
    
                // process births and deaths for each species
                for (size_t i = 0; i < state.speciesList.size(); ++i) {
                    processBirthsAndDeaths(birthCounts[i], deathCounts[i], state.speciesList[i]);
                }
    
                logSimulationDetails(t, birthCounts, deathCounts);
    
                if (checkExtinction()) {
                    log.log("Simulation ended early: all species went extinct.", LogLevel::NORMAL);
                    break;
                }
            }
    
            log.log("Simulation complete.", LogLevel::NORMAL);
        }
    };
#endif //SIMULATION_HPP