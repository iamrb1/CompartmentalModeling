/**
* @file Connection.cpp
 * @author Nitish Maindoliya
 */

// #include "../pch.h"
#include "Compartment.h"
#include <QRegularExpression>
#include "../Simulation.h"

Compartment::Compartment(QString name, QString symbol, const double initial_amount, Simulation* parent)
    : QObject(parent),
      m_simulation(parent),
      m_name(std::move(name)),
      m_symbol(std::move(symbol)),
      m_initial_amount(initial_amount),
      m_current_amount(initial_amount) {}

/**
   * @brief Setter for the Compartment name
   * @param name
   */
void Compartment::set_name(const QString& name) {
  static QRegularExpression regex("^[a-zA-Z0-9 ]+$");
  qDebug() << "Here";
  if (!name.isEmpty() && regex.match(name).hasMatch()) {
    qDebug() << "Works";
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

void Compartment::reset() {
  set_current_amount(m_initial_amount);
  m_amounts.clear();
  emit amountsChanged();
}

/**
   * @brief Setter for Compartment initial amount
   * @param initial_amount The amount in the container initially
   */
void Compartment::set_initial_amount(const double initial_amount) {
  m_initial_amount = initial_amount;
  set_current_amount(initial_amount);
  emit initialAmountChnaged();
}

void Compartment::add_amount(double amount) {
  set_current_amount(amount);
  m_amounts.push_back(amount);
  emit amountsChanged();
}
