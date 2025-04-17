/**
 * @file Compartment.cpp
 * @author Nitish Maindoliya
 */

#include "Compartment.h"

Compartment::Compartment(std::string name, std::string symbol, const float initial_amount)
    : m_name(std::move(name)),
      m_symbol(std::move(symbol)),
      m_initial_amount(initial_amount),
      m_current_amount(initial_amount) {}

float Compartment::get_current_amount() const {
  return m_current_amount;
}
float Compartment::get_initial_amount() const {
  return m_initial_amount;
}
const std::string& Compartment::get_name() const {
  return m_name;
}
const std::string& Compartment::get_symbol() const {
  return m_symbol;
}
int Compartment::get_x() const {
  return m_x;
}
int Compartment::get_y() const {
  return m_y;
}

void Compartment::set_current_amount(const float current_amount) {
  m_current_amount = current_amount;
}
void Compartment::set_initial_amount(const float initial_amount) {
  m_initial_amount = initial_amount;
}
void Compartment::set_name(const std::string& name) {
  m_name = name;
}
void Compartment::set_symbol(const std::string& symbol) {
  m_symbol = symbol;
}
void Compartment::set_x(const int x) {
  m_x = x;
}
void Compartment::set_y(const int y) {
  m_y = y;
}

void Compartment::set_simulation(Simulation* simulation) {
  m_simulation = simulation;
}
