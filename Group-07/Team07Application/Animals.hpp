#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <random>
#include <unordered_map>

struct Species {
        std::string name;
        int population;
        double birthRate;
        double deathRate;
        int maxAge;
        int reproductionAge;
        std::string foodSource;
};

struct Interaction {
    std::string predator;
    std::string prey;
    double huntRate;
};
// struct Environment {
//     std::string name;
//     double temperature;  // e.g., affects species survival and reproduction
//     double humidity;     // e.g., affects plant growth for herbivores
//     double foodAbundance; // Overall availability of food resources
//     std::unordered_map<std::string, double> resourceAvailability; // Specific food sources
//
//     Environment(std::string envName, double temp, double hum, double food)
//         : name(envName), temperature(temp), humidity(hum), foodAbundance(food) {}
//
//     /*
//      * Adjusts food availability dynamically based on ecosystem balance.
//      */
//     void UpdateResources() {
//         // Example logic: if food abundance is high, increase prey population support
//         foodAbundance *= 0.98; // Simulating natural food depletion over time
//     }
//
//     /*
//      * Adjusts environmental conditions (e.g., seasonal changes).
//      */
//     void UpdateConditions(int timeStep) {
//         if (timeStep % 10 == 0) { // Example: Every 10 steps, the season shifts
//             temperature += (rand() % 5 - 2); // Small fluctuations in temp
//             humidity += (rand() % 3 - 1); // Small fluctuations in humidity
//         }
//     }
// };

struct SimulationState {
    int timeSteps;
    int loggingInterval;

    Environment environment;
    std::vector<Species> speciesList;
    std::vector<Interaction> interactions;

    SimulationState(int steps, int logInterval)
        : timeSteps(steps), loggingInterval(logInterval){}

    void AddSpecies(const Species& s) {
        speciesList.push_back(s);
    }

    void AddInteraction(const Interaction& i) {
        interactions.push_back(i);
    }


    /*
    * FindSpecies - find a species by name
    * @param name: the name of the species to find
    * @return: a pointer to the species with the given name, or nullptr if not found
    */
    Species* FindSpecies(const std::string& name) {
        for (auto& s : speciesList) {
            if (s.name == name) return &s;
        }
        return nullptr;
    }

};
