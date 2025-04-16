/**
 * @file Simulation.cpp
 * @author Nitish Maindoliya
 */

#include "Simulation.h"

/**
 * @brief Add a compartment to the simulation. Also sets the simulation for the compartment.
 * @param compartment Compartment to add
 */
void Simulation::add_compartment(std::shared_ptr<Compartment> compartment) {
  compartment->set_simulation(this);
  m_compartments.push_back(std::move(compartment));
}

/**
 * @brief Add a connection to the simulation. Also sets the simulation for the connection.
 * @param connection Connection to add
 */
void Simulation::add_connection(std::shared_ptr<Connection> connection) {
  connection->set_simulation(this);
  m_connections.push_back(std::move(connection));
}