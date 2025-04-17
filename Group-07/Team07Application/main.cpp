
// Anand
#include "../Team07Library/DataFileManager.hpp"
#include "../Team07Library/OutputLog.hpp"
#include "Animals.hpp"
#include "Simulation.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <filesystem>
#include <string>

using namespace std;
using namespace cse;
namespace pt = boost::property_tree;

// Loads SimulationState from Boost ptree
SimulationState loadStateFromJSON(const pt::ptree& pt) {
    int steps = pt.get<int>("simulation_parameters.time_steps", 50);
	int logInterval = pt.get<int>("simulation_parameters.logging_interval", 1);
    SimulationState state(steps, logInterval);

    for (const auto& node : pt.get_child("species")) {
        const auto& sp = node.second;
        state.AddSpecies(Species{
            sp.get<string>("name"),
            sp.get<int>("initial_population"),
            sp.get<double>("birth_rate"),
            sp.get<double>("death_rate"),
            sp.get<int>("max_age"),
            sp.get<int>("reproduction_age"),
            sp.get<std::string>("food_source")
        });
    }

    for (const auto& node : pt.get_child("interactions")) {
    const auto& it = node.second;

    // Only process predator-prey interactions
    if (it.find("predator") != it.not_found() && it.find("prey") != it.not_found()) {
        state.AddInteraction(Interaction{
            it.get<std::string>("predator"),
            it.get<std::string>("prey"),
            it.get<double>("hunt_rate")
        });
    }
}

    return state;
}

// Locate root directory
string getProjectRoot() {
    std::filesystem::path currentPath = std::filesystem::current_path();
    while (!currentPath.empty() && currentPath.filename() != "CSE498-Spring2025") {
        currentPath = currentPath.parent_path();
    }
    return currentPath.string();
}

int main() {
    DataFileManager dfm;
    OutputLog log(LogLevel::DEBUG, "simulation_log.txt");

    string projectRoot = getProjectRoot();
    string path = projectRoot + "/Group-07/Data/config.json";

    dfm.openJSON(path);
    boost::property_tree::ptree pt = dfm.getDataJSON();

    SimulationState state = loadStateFromJSON(pt);


    std::array<double, 1> weight_arr = { 1.0 }; // Example weights
    SimulationRunner runner(state, weight_arr, log);
	runner.run();

    dfm.closeJSON();

    return 0;
}
