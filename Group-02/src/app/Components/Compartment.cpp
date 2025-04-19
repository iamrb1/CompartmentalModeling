/**
* @file Connection.cpp
 * @author Nitish Maindoliya
 */

// #include "../pch.h"
#include "Compartment.h"
#include "../Simulation.h"

Compartment::Compartment(QString name, QString symbol, const double initial_amount, Simulation* parent)
    : QObject(parent),
      m_simulation(parent),
      m_name(std::move(name)),
      m_symbol(std::move(symbol)),
      m_initial_amount(initial_amount),
      m_current_amount(initial_amount) {}