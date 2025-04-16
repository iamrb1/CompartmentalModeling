/**
 * @file Connection.cpp
 * @author Nitish Maindoliya
 */

#include "Connection.h"
#include <utility>

Connection::Connection(std::string name, std::shared_ptr<Compartment> source,
                       std::shared_ptr<Compartment> target, std::string rate_expression)
    : m_name(std::move(name)),
      m_source(std::move(source)),
      m_target(std::move(target)),
      m_rate_expression(std::move(rate_expression)) {}

const std::string& Connection::get_name() const {
  return m_name;
}
std::shared_ptr<Compartment> Connection::get_source() const {
  return m_source;
}
std::shared_ptr<Compartment> Connection::get_target() const {
  return m_target;
}
const std::string& Connection::get_rate_expression() const {
  return m_rate_expression;
}
Simulation* Connection::get_simulation() const {
  return m_simulation;
}

void Connection::set_name(const std::string& name) {
  m_name = name;
}
void Connection::set_source(std::shared_ptr<Compartment> source) {
  m_source = std::move(source);
}
void Connection::set_target(std::shared_ptr<Compartment> target) {
  m_target = std::move(target);
}
void Connection::set_rate_expression(const std::string& rate_expression) {
  m_rate_expression = rate_expression;
}
void Connection::set_simulation(Simulation* simulation) {
  m_simulation = simulation;
}
