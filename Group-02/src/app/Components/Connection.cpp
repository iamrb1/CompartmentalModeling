/**
 * @file Connection.cpp
 * @author Nitish Maindoliya
 */

// #include "../pch.h"
#include "Connection.h"
#include "../Simulation.h"

Connection::Connection(QString name, Compartment* source, Compartment* target, QString rate_expression, Simulation* parent)
    : QObject(parent),
      m_simulation(parent),
      m_name(std::move(name)),
      m_source(source),
      m_target(target),
      m_rate_expression(std::move(rate_expression)) {}
