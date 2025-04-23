/**
 * @file Simulation.cpp
 * @author Nitish Maindoliya, Matthew Hawkins, Rahul Baragur
 */

// #include "pch.h"
#include "Simulation.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QApplication>
#include <ranges>
#include "Components/Compartment.h"

/**
 * @brief Creates and adds a compartment to the simulation. Also sets the simulation for the compartment.
 */
Simulation::Simulation(QObject *parent) : QObject(parent) {}

void Simulation::add_compartment() {
  static size_t compartment_number = m_compartments.size();
  // Generate a new compartment with a unique symbol (A1,B1..Z1,A2...)
  QString symbol = QString("%1%2").arg(char('A' + (compartment_number % 26))).arg((compartment_number / 26) + 1);
  auto name = QString("Compartment %1").arg(symbol);

  auto compartment = std::make_shared<Compartment>(name, symbol, 0, this);
  m_compartments[symbol] = std::move(compartment);
  compartment_number++;

  // Emit signal to notify that compartments have changed
  emit compartmentsChanged();
}

void Simulation::clear() {
  m_is_running = false;

  // Disconnect timer
  if(m_timer){
    m_timer->stop();
    disconnect(m_timer, nullptr, this, nullptr);
    delete m_timer;
    m_timer = nullptr;
  }

  m_current_time = 0.0;
  m_save_path.clear();
  m_compartments.clear();
  m_connections.clear();
  m_variables.clear();

  emit isRunningChanged();
  emit currentTimeChanged();
  emit compartmentsChanged();
  emit connectionsChanged();
  emit variablesChanged();
}

void Simulation::reset() {
  m_is_running = false;
  m_current_time = 0;

  // Disconnect timer
  if(m_timer){
    m_timer->stop();
    disconnect(m_timer, nullptr, this, nullptr);
    delete m_timer;
    m_timer = nullptr;
  }

  m_current_time = 0.0;
  for (auto& [key, compartment] : m_compartments) {
    compartment->reset();
  }

  emit isRunningChanged();
  emit currentTimeChanged();
}

void Simulation::start() {
  m_is_running = true;
  emit isRunningChanged();

  if (!m_timer) {
    m_timer = new QTimer(this);
  }
  auto connection = connect(m_timer, &QTimer::timeout, this, &Simulation::take_time_step);
  m_timer->start(100);
}

void Simulation::take_time_step()
{
  if (m_current_time >= m_time_steps && m_timer){
    disconnect(m_timer, nullptr, this, nullptr);
    return;
  }

  std::unordered_map<QString, double> delta_amounts;

  /// Calculate changes based on connections
  for (const auto& connection : m_connections) {
    Compartment* source = connection->get_source();
    Compartment* target = connection->get_target();
    std::string rate = connection->get_rate_expression().toStdString();

    double transfer_amount = std::stod(rate) * source->get_current_amount();

    delta_amounts[source->get_symbol()] -= transfer_amount;
    delta_amounts[target->get_symbol()] += transfer_amount;
  }

  QVariantMap new_compartment_amounts;
  for (const auto& [symbol, compartment] : m_compartments) {
    if (!delta_amounts.contains(symbol)) {
      continue;
    }

    double new_amount = compartment->get_current_amount() + delta_amounts[symbol];
    compartment->set_current_amount(new_amount);
    new_compartment_amounts[compartment->get_symbol()] = new_amount;
  }

  emit addGraphingValues(m_current_time, new_compartment_amounts);

  m_current_time++;
  emit currentTimeChanged();
}

void Simulation::pause() {
  m_is_running = false;
  emit isRunningChanged();

  if (m_timer) {
    m_timer->stop();
  }
}

/**
 * @brief Return a list of all compartments in the simulation.
 */
QVector<Compartment*> Simulation::get_compartments() const {
  QVector<Compartment*> compartments;
  for (const auto& compartment : m_compartments | std::views::values) {
    compartments.push_back(compartment.get());
  }
  return compartments;
}

/**
 * @brief Return a list of all connections in the simulation.
 */
QVector<Connection*> Simulation::get_connections() const {
  QVector<Connection*> connections;
  for (const auto& connection : m_connections) {
    connections.push_back(connection.get());
  }

  return connections;
}

/**
 * @brief Return a map of all variables in the simulation.
 */
QVariantMap Simulation::get_variables() const {
  QVariantMap variables;
  for (const auto& [name, value] : m_variables) {
    variables.insert(name, value);
  }

  return variables;
}
/**
 * @brief Adds a variable to the simulation
 * @param name variable name
 * @param value variable value
 */
void Simulation::add_variable(const QString& name, double value) {
  static size_t variable_number = m_variables.size();
  if (name.isEmpty()) {
    auto var_name = QString("k%1").arg(variable_number);
    m_variables.emplace(var_name, value);
    variable_number++;
  } else {
    m_variables[name] = value;
  }

  // qDebug() << "Variables in C++:";
  // for (const auto& [key, val] : m_variables) {
  //   qDebug() << QString::fromStdString(key) << value;
  // }

  emit variablesChanged();
}
/**
 * @brief Removes a variable from the simulation
 * @param name variable name
 */
void Simulation::remove_variable(const QString& name) {
  if (const auto it = m_variables.find(name); it != m_variables.end()) {
    m_variables.erase(it);
    emit variablesChanged();
  }
}

/**
 * @brief Shows error message when called on mid screen
 * @param message error message
 */
void Simulation::throw_error(const QString &message) {
    m_error_message = "test throw_error";
    emit errorModuleShow(message);
}

/**
 * @brief Updates variable with new name/value
 * @param name Current variable name
 * @param new_name Possible new variable name
 * @param value New variable value
 */
void Simulation::update_variable(const QString& name, const QString& new_name, double value) {
  if (m_variables.contains(name)) {
    if (name == new_name) {
      m_variables[name] = value;
    } else {
      auto node_handler = m_variables.extract(name);
      node_handler.key() = new_name;
      node_handler.mapped() = value;
      m_variables.insert(std::move(node_handler));
    }

    emit variablesChanged();
  }
}
/**
 * @brief Removes a connection from the simulation
 * @param connection Connection to be removed
 */
void Simulation::remove_connection(const Connection* connection) {
  for (auto it = m_connections.begin(); it != m_connections.end(); ++it) {
    if (it->get() == connection) {
      m_connections.erase(it);
      break;
    }
  }
  m_sidebar_connection = nullptr;

  emit connectionsChanged();
  emit sidebarConnectionChanged();
}
/**
 * @brief Removes a compartment from the simulation
 * @param symbol Compartment symbol
 */
void Simulation::remove_compartment(const QString& symbol) {
  if (m_compartments.contains(symbol)) {
    // Remove connections associated with the compartment
    bool connection_deleted = false;
    for (auto it = m_connections.begin(); it != m_connections.end();) {
      if (const auto connection = *it;
          connection->get_source()->get_symbol() == symbol || connection->get_target()->get_symbol() == symbol) {
        it = m_connections.erase(it);
        connection_deleted = true;
      } else {
        ++it;
      }
    }

    if (connection_deleted) {
      emit connectionsChanged();
    }

    // Remove the compartment
    m_compartments.erase(symbol);
    m_sidebar_compartment = nullptr;

    emit compartmentsChanged();
    emit sidebarCompartmentChanged();
  }
}

/**
 * Set the target component and make the connection
 * @param target The target compartment
 */
void Simulation::set_target_compartment(Compartment* target) {
  if (!m_source_compartment || !target || !m_connection_mode) {
    return;
  }
  m_target_compartment = target;
  QString connection_name =
      QString("Connection %1 %2").arg(m_source_compartment->get_symbol(), m_target_compartment->get_symbol());

  // Check if connection exists
  bool connection_exists = false;
  for (const auto& connection : m_connections) {
    if (connection->get_source() == m_source_compartment && connection->get_target() == m_target_compartment) {
      connection_exists = true;
      break;
    }
  }

  if (!connection_exists) {
    // Connection logic
    auto connection =
        std::make_shared<Connection>(connection_name, m_source_compartment, m_target_compartment, "0", this);
    m_connections.push_back(std::move(connection));
    emit connectionsChanged();
  }

  m_source_compartment = nullptr;
  m_target_compartment = nullptr;

  emit sourceCompartmentChanged();
  emit targetCompartmentChanged();
}
/**
 * @brief Sets connection for sidebar to present values for
 * @param connection
 */
void Simulation::set_sidebar_connection(Connection* connection) {
  m_sidebar_connection = connection;
  m_sidebar_compartment = nullptr;

  emit sidebarConnectionChanged();
  emit sidebarCompartmentChanged();
}

void Simulation::set_m_connection_mode(bool connection_mode)
{
  if (connection_mode == false) {
    m_source_compartment = nullptr;
    m_target_compartment = nullptr;

    emit sourceCompartmentChanged();
    emit targetCompartmentChanged();
  }
  m_connection_mode = connection_mode;
  emit connectionModeChanged();
}

void Simulation::update_compartment_symbol(const QString &symbol, const QString &new_symbol)
{
  if (m_compartments.contains(symbol)) {
    if (symbol != new_symbol) {
      auto node_handler = m_compartments.extract(symbol);
      node_handler.key() = new_symbol;
      m_compartments.insert(std::move(node_handler));
    }
  }
}

/**
 * @brief Sets compartment for sidebar to present values for
 * @param compartment
 */
void Simulation::set_sidebar_compartment(Compartment* compartment) {
  m_sidebar_compartment = compartment;
  m_sidebar_connection = nullptr;

  emit sidebarCompartmentChanged();
  emit sidebarConnectionChanged();
}

/**
 * @brief load in a xml simulation file.
 * @param filename The file path for the xml file.
 */
void Simulation::load_xml(const QString& filename) {
  const QUrl fileUrl(filename);
  QFile file(fileUrl.toLocalFile());

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qWarning() << "Could not open file:" << filename << "Error:" << file.errorString();
    return;
  }

  QXmlStreamReader xml(&file);
  clear();
  m_save_path = filename;

  // find simulation name
  while (!xml.atEnd() && !xml.hasError()) {
    xml.readNext();
    if (xml.isStartElement() && xml.name() == "simulation") {
      m_name = xml.attributes().value("name").toString();
      break;
    }
  }

  // get the children of simulation
  while (!xml.atEnd() && !xml.hasError()) {
    xml.readNext();
    if (!xml.isStartElement())
      continue;

    if (xml.name() == "compartments") {
      while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "compartments")) {
        xml.readNext();
        if (xml.isStartElement() && xml.name() == "compartment") {
          auto attrs = xml.attributes();
          QString name = attrs.value("name").toString();
          QString symbol = attrs.value("symbol").toString();
          float initial = attrs.value("initial").toFloat();
          const int x = attrs.value("x").toInt();
          const int y = attrs.value("y").toInt();

          auto compartment = std::make_shared<Compartment>(name, symbol, initial, this);
          compartment->set_x(x);
          compartment->set_y(y);
          m_compartments[symbol] = std::move(compartment);
        }
      }
    } else if (xml.name() == "variables") {
      while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "variables")) {
        xml.readNext();
        if (xml.isStartElement() && xml.name() == "variable") {
          auto attrs = xml.attributes();
          auto name = attrs.value("name").toString();
          const double value = attrs.value("value").toDouble();
          m_variables[name] = value;
        }
      }
    } else if (xml.name() == "connections") {
      while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "connections")) {
        xml.readNext();
        if (xml.isStartElement() && xml.name() == "connection") {
          auto attrs = xml.attributes();
          auto name = attrs.value("name").toString();
          auto from = attrs.value("from").toString();
          auto to = attrs.value("to").toString();
          auto rate = attrs.value("rate").toString();

          if (!m_compartments.contains(from) || !m_compartments.contains(to)) {
            qWarning() << "Connection references non-existent compartments:" << from << to;
            continue;
          }

          auto from_compartment = m_compartments[from];
          auto to_compartment = m_compartments[to];

          bool connection_exists = false;
          for (const auto& connection : m_connections) {
            if (connection->get_source() == from_compartment.get() &&
                connection->get_target() == to_compartment.get()) {
              connection_exists = true;
              break;
            }
          }

          if (!connection_exists) {
            auto connection =
                std::make_shared<Connection>(name, from_compartment.get(), to_compartment.get(), rate, this);
            m_connections.push_back(std::move(connection));
          }
        }
      }
    }
  }

  if (xml.hasError()) {
    qWarning() << "XML Error:" << xml.errorString();
  } else {
    emit compartmentsChanged();
    emit connectionsChanged();
    emit variablesChanged();
  }
}

void Simulation::save()
{
  if (!m_save_path.isEmpty()){
    save_xml(m_save_path);
  } else {
    // TODO: Add error
  }
}

/**
 * @brief save the current simulation as a xml simulation file.
 * @param filename The file path for the xml file.
 */
void Simulation::save_xml(const QString& filename) {
  const QUrl fileUrl(filename);
  QFile file(fileUrl.toLocalFile());

  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    qWarning() << "Could not open file:" << filename << "Error:" << file.errorString();
    return;
  }

  QXmlStreamWriter xml(&file);
  xml.setAutoFormatting(true);
  xml.writeStartDocument();

  xml.writeStartElement("simulation");
  xml.writeAttribute("name", m_name);

  xml.writeStartElement("compartments");
  for (const auto& compartment : m_compartments | std::views::values) {
    xml.writeStartElement("compartment");
    xml.writeAttribute("name", compartment->get_name());
    xml.writeAttribute("symbol", compartment->get_symbol());
    xml.writeAttribute("initial", QString::number(compartment->get_initial_amount()));
    xml.writeAttribute("x", QString::number(compartment->get_x()));
    xml.writeAttribute("y", QString::number(compartment->get_y()));

    xml.writeEndElement();
  }
  xml.writeEndElement();

  xml.writeStartElement("variables");
  for (auto& [name, value] : m_variables) {
    xml.writeStartElement("variable");
    xml.writeAttribute("name", name);
    xml.writeAttribute("value", QString::number(value));
    xml.writeEndElement();
  }
  xml.writeEndElement();

  xml.writeStartElement("connections");
  for (const auto& connection : m_connections) {
    xml.writeStartElement("connection");
    xml.writeAttribute("name", connection->get_name());
    xml.writeAttribute("from", connection->get_source()->get_symbol());
    xml.writeAttribute("to", connection->get_target()->get_symbol());
    xml.writeAttribute("rate", connection->get_rate_expression());
    xml.writeEndElement();
  }

  xml.writeEndElement();
  xml.writeEndElement();
  xml.writeEndDocument();
  file.close();
}
