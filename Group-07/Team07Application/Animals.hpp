#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <random>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <optional>
#include <functional>

struct Animal {
        int id;
        std::string species;
        std::string prey;
        uint16_t lastEaten;
        int age;
        bool isPredator;
    
        virtual ~Animal() = default;
    
        Animal(int animalId, const std::string& speciesName, const std::string& preyName, bool predator = false)
            : id(animalId), species(speciesName), prey(preyName), lastEaten(0), age(0), isPredator(predator) {}
    
        virtual const std::array<double, 1>& getWeights() const {
            static std::array<double, 1> weights = { 1.0 };
            return weights;
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
        std::vector<std::shared_ptr<Species>> speciesList;
        std::vector<Interaction> interactions;
        std::vector<Animal> animals;
    
        SimulationState(int steps, int logInterval)
            : timeSteps(steps), loggingInterval(logInterval) {}
    
        void AddSpecies(const Species& s) {
            speciesList.push_back(std::make_shared<Species>(s));
        }
    
        void AddInteraction(const Interaction& i) {
            interactions.push_back(i);
        }
    
        void PopulateInitialAnimals(int& globalId) {
            // populate the initial animals based on species list
            animals.clear();
            for (const auto& species : speciesList) {
                for (int i = 0; i < species->population; ++i) {
                    animals.emplace_back(globalId++, species->name, species->foodSource, species->foodSource != "None");
                }
            }
        }
    
    /*
    * @brief Find a species by name
    * 
    * This function searches for a species in the species list by its name.
    * If found, it returns a reference to the species wrapped in std::optional.
    * If not found, it returns std::nullopt.
    */
    std::optional<std::reference_wrapper<Species>> FindSpecies(const std::string& name) {
        auto it = std::find_if(speciesList.begin(), speciesList.end(),
            [&name](const std::shared_ptr<Species>& species) {
                return species->name == name;
            });

        if (it != speciesList.end()) {
            return *(*it); // dereference shared_ptr to get Species&
        }

        return std::nullopt;
    }
    };