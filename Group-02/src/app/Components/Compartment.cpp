/**
 * @file Connection.cpp
 * @author Nitish Maindoliya
 */

// #include "../pch.h"
#include "Compartment.h"
#include <QRegularExpression>
#include "../Application/Simulation.h"

Compartment::Compartment(QString name, QString symbol, const double initial_amount, Simulation* parent)
    : QObject(parent),
      m_simulation(parent),
      m_name(std::move(name)),
      m_symbol(std::move(symbol)),
      m_initial_amount(initial_amount),
      m_current_amount(initial_amount) {}

/**
 * @brief Getter for the name of the compartment
 * @return Compartment name
 */
const QString& Compartment::get_name() const {
  return m_name;
}

/**
 * @brief Getter for the symbol of the compartment
 * @return Compartment symbol
 */
const QString& Compartment::get_symbol() const {
  return m_symbol;
}

/**
 * @brief Getter for the initial amount of the compartment
 * @return Compartment initial amount
 */
double Compartment::get_initial_amount() const {
  return m_initial_amount;
}

/**
 * @brief Getter for the current amount of the compartment
 * @return Compartment current amount
 */
double& Compartment::get_current_amount() {
  return m_current_amount;
}

/**
 * @brief Getter for the x position of the compartment
 * @return Compartment x position
 */
int Compartment::get_x() const {
  return m_x;
}

/**
 * @brief Getter for the y position of the compartment
 * @return Compartment y position
 */
int Compartment::get_y() const {
  return m_y;
}

/**
 * @brief Resets the compartment to its initial amount
 */
void Compartment::reset() {
  set_current_amount(m_initial_amount);
}

/**
 * @brief Setter for the Compartment name
 * @param name
 */
void Compartment::set_name(const QString& name) {
  static QRegularExpression regex("^[a-zA-Z0-9 ]+$");
  if (!name.isEmpty() && regex.match(name).hasMatch()) {
    // Alpha numeric non empty with spaces
    m_name = name;
    emit nameChanged();
  }
}

/**
 * @brief Setter for the Compartment symbol
 * @param symbol
 */
void Compartment::set_symbol(const QString& symbol) {
  static QRegularExpression regex("^[a-zA-Z0-9]+$");
  if (!symbol.isEmpty() && regex.match(symbol).hasMatch()) {
    // Alpha numeric non empty

    // Change the hashmap in simulation
    m_simulation->update_compartment_symbol(m_symbol, symbol);
    m_symbol = symbol;
    emit symbolChanged();
  }
}

/**
 * @brief Setter for Compartment initial amount
 * @param initial_amount The amount in the container initially
 */
void Compartment::set_initial_amount(const double initial_amount) {
  m_initial_amount = initial_amount;
  set_current_amount(initial_amount);
  emit initialAmountChanged();
}

/**
 * @brief Setter for Compartment current amount
 * @param current_amount The amount in the container currently
 */
void Compartment::set_current_amount(const double current_amount) {
  m_current_amount = current_amount;
  emit currentAmountChanged();
}

/**
 * @brief Setter for the x position of the compartment
 * @param x The x position of the compartment
 */
void Compartment::set_x(const int x) {
  m_x = x;
  emit xChanged();
}

/**
 * @brief Setter for the y position of the compartment
 * @param y The y position of the compartment
 */
void Compartment::set_y(const int y) {
  m_y = y;
  emit yChanged();
}

/**
 * @brief Setter for the parent simulation
 * @param simulation The parent simulation
 */
void Compartment::set_simulation(Simulation* simulation) {
  m_simulation = simulation;
}
