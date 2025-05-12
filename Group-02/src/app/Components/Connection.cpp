/**
 * @file Connection.cpp
 * @author Nitish Maindoliya
 */

// #include "../pch.h"
#include "Connection.h"
#include "../Application/Simulation.h"

Connection::Connection(QString name, Compartment* source, Compartment* target, QString rate_expression,
                       Simulation* parent)
    : QObject(parent),
      m_simulation(parent),
      m_name(std::move(name)),
      m_source(source),
      m_target(target),
      m_rate_expression(std::move(rate_expression)) {}

/**
 * @brief Getter for the name of the connection
 * @return Compartment name
 */
const QString& Connection::get_name() const {
  return m_name;
}

/**
 * @brief Getter for the source compartment
 * @return Source compartment
 */
Compartment* Connection::get_source() const {
  return m_source;
}

/**
 * @brief Getter for the target compartment
 * @return Target compartment
 */
Compartment* Connection::get_target() const {
  return m_target;
}

/**
 * @brief Getter for the rate of transfer
 * @return Rate of transfer expression
 */
const QString& Connection::get_rate_expression() const {
  return m_rate_expression;
}

/**
 * @brief Getter for the parent simulation
 * @return Parent simulation
 */
Simulation* Connection::get_simulation() const {
  return m_simulation;
}

/**
 * @brief Setter for the name of connection
 * @param name The name of the connection
 */
void Connection::set_name(const QString& name) {
  static QRegularExpression regex("^[a-zA-Z0-9 ]+$");
  if (!name.isEmpty() && regex.match(name).hasMatch()) {
    m_name = name;
  } else {
    m_simulation->prompt("Connection name is not alpha numeric", Simulation::PromptType::ERR, Simulation::PromptMode::TOAST);
  }
  emit nameChanged();
}

/**
 * @brief Setter for source compartment
 * @param source The source compartment
 */
void Connection::set_source(Compartment* source) {
  for (auto& connection : m_simulation->get_connections()) {
    if (connection->get_source() == source && connection->get_target() == m_target && connection != this) {
      m_simulation->prompt("Connection already exists", Simulation::PromptType::ERR, Simulation::PromptMode::TOAST);
      emit sourceChanged();
      return;
    }
  }

  m_source = source;
  emit sourceChanged();
}

/**
 * @brief Setter for target compartment
 * @param target The target compartment
 */
void Connection::set_target(Compartment* target) {
  for (auto& connection : m_simulation->get_connections()) {
    if (connection->get_source() == m_source && connection->get_target() == target && connection != this) {
      m_simulation->prompt("Connection already exists", Simulation::PromptType::ERR, Simulation::PromptMode::TOAST);
      emit sourceChanged();
      return;
    }
  }

  m_target = target;
  emit targetChanged();
}

/**
 * @brief Setter for the rate of transfer
 * @param rate_expression The rate of transfer expression
 */
void Connection::set_rate_expression(const QString& rate_expression) {
  m_rate_expression = rate_expression;
  emit rateExpressionChanged();
}
