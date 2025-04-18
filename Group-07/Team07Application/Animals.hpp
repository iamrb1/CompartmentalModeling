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

/*
* @brief Animal class representing a generic animal in the simulation
* 
* This class contains properties such as species, prey, age, and whether the animal is a predator.
* It also provides methods for getting weights and checking if the animal can reproduce
*/
struct Animal {
        int id;
        std::string species;
        std::string prey;
        uint16_t lastEaten;
        int age;
        bool isPredator;
    
        virtual ~Animal() = default;
    
        /*
        * @brief Constructor for Animal
        *
        * Initializes the animal with its ID, species name, prey name, and whether it is a predator.
        * 
        * @param animalId Unique identifier for the animal
        * @param speciesName Name of the species
        * @param preyName Name of the prey species
        * @param predator Boolean indicating if the animal is a predator. Defaults to false.
        */
        Animal(int animalId, const std::string& speciesName, const std::string& preyName, bool predator = false)
            : id(animalId), species(speciesName), prey(preyName), lastEaten(0), age(0), isPredator(predator) {}
    
        /*
        * @brief Get the weights for the animal
        * 
        * This function returns a static array of weights for the animal.
        * In this case, it returns a single weight of 1.0.
        * 
        * @return const std::array<double, 1>& Reference to the weights array
        */
        virtual const std::array<double, 1>& getWeights() const {
            static std::array<double, 1> weights = { 1.0 };
            return weights;
        }
    
        /*
        * @brief Check if the animal can reproduce
        * 
        * This function checks if the animal has reached the age required for reproduction.
        * 
        * @param reproductionAge The age at which the animal can reproduce
        * @return bool True if the animal can reproduce, false otherwise
        */
        virtual bool canReproduce(int reproductionAge) const {
            return age >= reproductionAge;
        }
    };
    
    /* 
    * @brief Species struct representing a species in the simulation
    * 
    * This struct contains properties such as name, population, birth rate, death rate,
    * maximum age, reproduction age, and food source.
    */
    struct Species {
        std::string name;
        int population;
        double birthRate;
        double deathRate;
        int maxAge;
        int reproductionAge;
        std::string foodSource;
    };
    
    /* 
    * @brief Interaction struct representing predator-prey interactions
    * 
    * This struct contains properties such as predator species, prey species,
    * and the hunt rate (the rate at which the predator hunts the prey).
    */
    struct Interaction {
        std::string predator;
        std::string prey;
        double huntRate;
    };
    
    /* 
    * @brief SimulationState struct representing the state of the simulation
    * 
    * This struct contains properties such as the number of time steps,
    * logging interval, a list of species, interactions, and animals.
    * It also provides methods for adding species, interactions, and populating initial animals.
    */
    struct SimulationState {
        int timeSteps;
        int loggingInterval;
        std::vector<std::shared_ptr<Species>> speciesList;
        std::vector<Interaction> interactions;
        std::vector<Animal> animals;
    
        /*
        * @brief Constructor for SimulationState
        *
        * Initializes the simulation state with the number of time steps and logging interval.
        * 
        * @param steps Number of time steps for the simulation
        * @param logInterval Interval for logging simulation details
        */
        SimulationState(int steps, int logInterval)
            : timeSteps(steps), loggingInterval(logInterval) {}
    
        /* 
        * @brief Add a species to the simulation
        * 
        * This function adds a species to the species list.
        * It takes a Species object as input and stores it in a shared pointer.
        * 
        * @param s The species to be added
        */
        void AddSpecies(const Species& s) {
            speciesList.push_back(std::make_shared<Species>(s));
        }
    
        /* 
        * @brief Add an interaction to the simulation
        * 
        * This function adds a predator-prey interaction to the interactions list.
        * It takes an Interaction object as input and stores it in the list.
        * 
        * @param i The interaction to be added
        */
        void AddInteraction(const Interaction& i) {
            interactions.push_back(i);
        }
    
        /* 
        * @brief Populate initial animals in the simulation
        * 
        * This function populates the initial animals based on the species list.
        * It takes a reference to a global ID counter and uses it to assign unique IDs to each animal.
        * 
        * @param globalId Reference to the global ID counter
        */
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