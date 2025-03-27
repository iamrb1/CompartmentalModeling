//
// Created by trang on 3/14/2025.
//

#ifndef SIMULATION_HPP
#define SIMULATION_HPP
#include <iostream>
#include <ranges>
#include <vector>
#include <random>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <random>
#include <unordered_map>
#include "../Team07Library/OutputLog.hpp"
#include "../Team07Library/Scheduler.hpp"
#include "Animals.hpp"
using namespace cse;

void AddPredators(int count, std::vector<Predator>& predators, cse::Scheduler<int>& scheduler, int& id, std::string prey) {
    for (auto i : std::views::iota(0, count)) {
        (void)i; // suppress unused variable warning
        Predator pred = Predator{id, prey, 0};
        predators.push_back(pred);
        std::vector<double> weights = pred.getWeights();
        scheduler.AddProcess(id, weights);
        id++;
    }
}

void runSimulation(SimulationState& state, OutputLog& log) {
    std::random_device rd;
    std::mt19937 gen(rd());

    log.log("Starting simulation...", LogLevel::NORMAL);



    cse::Scheduler scheduler({-1}); 

    std::vector<Predator> predators; // list of individual predators
    int id =0;
    for (const auto& interaction : state.interactions) {
        Species* predator = state.FindSpecies(interaction.predator);
        Species* prey = state.FindSpecies(interaction.prey);
        
        if (predator && prey) {
            AddPredators(prey->population, predators, scheduler, id, prey->name);
        }
    }

    for (int t = 0; t < state.timeSteps; ++t) {
        log.log("=== Time Step " + std::to_string(t) + " ===", LogLevel::VERBOSE);


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
                // int preyEaten = std::min(prey->population, static_cast<int>(predator->population * interaction.huntRate));
                // std::cout << "Prey eaten: " << preyEaten << std::endl;
                // prey->population -= preyEaten;
                // predator->population += preyEaten / 2; 

                int toEat = std::min(prey->population, static_cast<int>(predator->population * interaction.huntRate));
                for (auto i : std::views::iota(0, toEat)) {
                    (void)i; // suppress unused variable warning
                    auto predID = scheduler.PopNextProcess(); 
                    if (!predID) {
                        break;
                    }
                    prey->population -=1;
                    predators[*predID].lastEaten +=1;
                }

                // create toEat//2 new predators
                AddPredators(toEat/2, predators, scheduler, id, prey->name);
            }
        }

        for (size_t i = 0; i < state.speciesList.size(); ++i) {
            state.speciesList[i].population += birthCounts[i] - deathCounts[i];
            state.speciesList[i].population = std::max(0, state.speciesList[i].population);

            // if the species is predator type, randomly select deathCounts[i] of them to die
            if (state.speciesList[i].foodSource != "None") {
                std::shuffle(predators.begin(), predators.end(), gen);
                
                // Create a list of indices to remove
                std::vector<int> indicesToRemove;
                for (auto j : std::views::iota(0, std::min(deathCounts[i], static_cast<int>(predators.size())))) {
                    indicesToRemove.push_back(j);
                    scheduler.RemoveProcess(predators[j].id);
                }
                
                // Remove from the end to avoid invalidating indices
                std::sort(indicesToRemove.begin(), indicesToRemove.end(), std::greater<int>());
                for (int index : indicesToRemove) {
                    predators.erase(predators.begin() + index);
                }
            }

            // log birth and death counts
            log.log(state.speciesList[i].name + " Births: " + std::to_string(birthCounts[i]), LogLevel::DEBUG);
            log.log(state.speciesList[i].name + " Deaths: " + std::to_string(deathCounts[i]), LogLevel::DEBUG);
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
