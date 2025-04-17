#include "Sidebar.h"
#include "../Simulation.h"

Sidebar::Sidebar(QObject* parent) : QObject{parent} {


}

void Sidebar::add_variable(const std::string& id, const float &var)
{
  m_simulation->get_variables()[id] = var;
  emit variable_emmited(id,var);
}

void Sidebar::remove_variable(const std::string& id)
{
  m_simulation->get_variables().erase(id);
  emit variable_remove_emmited(id);
}
