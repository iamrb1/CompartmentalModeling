/**
 * @file Simulation.h
 * @author Nitish Maindoliya
 *
 *
 */
 
#ifndef SIMULATION_H
#define SIMULATION_H
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Components/Compartment.h"
#include "Components/Connection.h"
#include "Components/Sidebar.h"

class Simulation {
private:
  /// Simulation name
  std::string m_name;
  /// Save path
  std::string m_save_path;

  /// Simulation time
  double m_time = 0.0;

  /// Unordered map of compartments with their symbols as keys
  std::vector<std::shared_ptr<Compartment>> m_compartments;
  /// Connections between compartments
  std::vector<std::shared_ptr<Connection>> m_connections;

  /// Variables for the simulation
  std::unordered_map<std::string, float> m_variables = { {"k1", 0.01f}, {"k2", 0.1f}, {"k3", 0.5f} };

public:
  Simulation() = default;
  /// Destructor
  ~Simulation() = default;

  void add_compartment(std::shared_ptr<Compartment> compartment);
  void add_connection(std::shared_ptr<Connection> connection);
  
  void load_xml(const std::string& filename);
  void save_xml(const std::string& filename);

  std::unordered_map<std::string, float> get_variables(){return m_variables;};


};



#endif //SIMULATION_H
