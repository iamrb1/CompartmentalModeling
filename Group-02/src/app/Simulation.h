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

#include "Compartment.h"
#include "Connection.h"

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
  std::unordered_map<std::string, float> m_variables;

public:
  Simulation() = default;
  /// Destructor
  ~Simulation() = default;

  void add_compartment(std::shared_ptr<Compartment> compartment);
  void add_connection(std::shared_ptr<Connection> connection);
  
  void load_xml(const std::string& filename);
  void save_xml(const std::string& filename);
};



#endif //SIMULATION_H
