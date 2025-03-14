//
// Created by trang on 3/14/2025.
//

#ifndef SIMULATION_HPP
#define SIMULATION_HPP
#include <iostream>
#include <vector>
#include <random>
#include "../Team07Library/OutputLog.hpp"
#include "Animals.hpp"
using namespace cse;


void runSimulation(SimulationState& state, OutputLog& log) {
    std::random_device rd;
    std::mt19937 gen(rd());

    log.log("Starting simulation...", LogLevel::NORMAL);

    for (int t = 0; t < state.timeSteps; ++t) {
        log.log("=== Time Step " + std::to_string(t) + " ===", LogLevel::VERBOSE);

        // Update environment
        // state.environment.UpdateResources();
        // state.environment.UpdateConditions(t);

        std::vector<int> birthCounts(state.speciesList.size(), 0);
        std::vector<int> deathCounts(state.speciesList.size(), 0);

        for (size_t i = 0; i < state.speciesList.size(); ++i) {
            Species& species = state.speciesList[i];
            int births = static_cast<int>(species.population * species.birthRate);
            int deaths = static_cast<int>(species.population * species.deathRate);

            birthCounts[i] = births;
            deathCounts[i] = deaths;
        }

        for (const auto& interaction : state.interactions) {
            Species* predator = state.FindSpecies(interaction.predator);
            Species* prey = state.FindSpecies(interaction.prey);

            if (predator && prey && prey->population > 0) {
                int preyEaten = std::min(prey->population, static_cast<int>(predator->population * interaction.huntRate));

                prey->population -= preyEaten;
                predator->population += preyEaten / 2;
            }
        }

        for (size_t i = 0; i < state.speciesList.size(); ++i) {
            state.speciesList[i].population += birthCounts[i] - deathCounts[i];
            state.speciesList[i].population = std::max(0, state.speciesList[i].population);
        }

        for (const auto& species : state.speciesList) {
            log.log(species.name + " Population: " + std::to_string(species.population), LogLevel::DEBUG);
        }

        bool extinct = true;
        for (const auto& species : state.speciesList) {
            if (species.population > 0) {
                extinct = false;
                break;
            }
        }
        if (extinct) {
            log.log("Simulation ended early: all species went extinct.", LogLevel::NORMAL);
            break;
        }
    }

    log.log("Simulation complete.", LogLevel::NORMAL);
}


#endif //SIMULATION_HPP
