#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <random>
#include <unordered_map>

class Animal {
public:
    int id;
    std::string species;
    std::string prey;
    uint16_t lastEaten;
    int age;
    bool isPredator;

    virtual ~Animal() = default;

    Animal(int animalId, const std::string& speciesName, const std::string& preyName, bool predator = false)
        : id(animalId), species(speciesName), prey(preyName), lastEaten(0), age(0), isPredator(predator) {}

    virtual std::vector<double> getWeights() {
        return std::vector<double>{static_cast<double>(lastEaten)};
    }

    virtual bool canReproduce(int reproductionAge) const {
        return age >= reproductionAge;
    }
};

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
    std::vector<Species> speciesList;
    std::vector<Interaction> interactions;
    std::vector<Animal> animals;

    SimulationState(int steps, int logInterval)
        : timeSteps(steps), loggingInterval(logInterval) {}

    void AddSpecies(const Species& s) {
        speciesList.push_back(s);
    }

    void AddInteraction(const Interaction& i) {
        interactions.push_back(i);
    }

    // Populate initial animals based on species population
    void PopulateInitialAnimals(int& globalId) {
        animals.clear();
        for (const auto& species : speciesList) {
            for (int i = 0; i < species.population; ++i) {
                animals.emplace_back(globalId++, species.name, species.foodSource, species.foodSource != "None");
            }
        }
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