#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <random>

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
