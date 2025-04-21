/**
 * @file Simulation.cpp
 * @author Nitish Maindoliya, Matthew Hawkins, Rahul Baragur
 */

// #include "pch.h"
#include "Simulation.h"
#include <QFile>
#include <QXmlStreamReader>
#include <ranges>
#include "Components/Compartment.h"

/**
 * @brief Creates and adds a compartment to the simulation. Also sets the simulation for the compartment.
 */
void Simulation::add_compartment() {
  static size_t compartment_number = m_compartments.size();
  // Generate a new compartment with a unique symbol
  auto symbol = QString(static_cast<char>('A' + compartment_number));
  auto name = QString("Compartment %1").arg(symbol);

  auto compartment = std::make_shared<Compartment>(name, symbol, 0, this);
  m_compartments[symbol] = std::move(compartment);
  compartment_number++;

  // Emit signal to notify that compartments have changed
  emit compartmentsChanged();
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
    variables.insert(QString::fromStdString(name), value);
  }

  return variables;
}

void Simulation::add_variable(const QString& name, double value) {
  static size_t variable_number = m_variables.size();
  if (name.isEmpty()) {
    std::string var_name = "k" + std::to_string(variable_number + 1);
    m_variables.emplace(var_name, value);
    variable_number++;
  } else {
    m_variables[name.toStdString()] = value;
  }

  // qDebug() << "Variables in C++:";
  // for (const auto& [key, val] : m_variables) {
  //   qDebug() << QString::fromStdString(key) << value;
  // }

  emit variablesChanged();
}

void Simulation::remove_variable(const QString& name) {
  const auto key = name.toStdString();
  if (const auto it = m_variables.find(key); it != m_variables.end()) {
    m_variables.erase(it);
    emit variablesChanged();
  }
}

void Simulation::update_variable(const QString& name, const QString& new_name, double value) {
  if (const auto key = name.toStdString(); m_variables.contains(key)) {
    if (name == new_name) {
      m_variables[key] = value;
    } else {
      auto node_handler = m_variables.extract(key);
      node_handler.key() = new_name.toStdString();
      node_handler.mapped() = value;
      m_variables.insert(std::move(node_handler));
    }

    emit variablesChanged();
  }
}

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

void Simulation::set_sidebar_connection(Connection* connection) {
  m_sidebar_connection = connection;
  m_sidebar_compartment = nullptr;

  emit sidebarConnectionChanged();
  emit sidebarCompartmentChanged();
}

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
  clear_simulation();
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
          std::string key = attrs.value("name").toString().toStdString();
          const double value = attrs.value("value").toDouble();
          m_variables[key] = value;
        }
      }
    } else if (xml.name() == "connections") {
      while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "connections")) {
        xml.readNext();
        if (xml.isStartElement() && xml.name() == "connection") {
          auto attrs = xml.attributes();
          QString name = attrs.value("name").toString();
          QString from = attrs.value("from").toString();
          QString to = attrs.value("to").toString();
          QString rate = attrs.value("rate").toString();

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
    xml.writeAttribute("name", QString::fromStdString(name));
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

void Simulation::startSimulation() {
    m_current_time = 0.0;

    for (const auto& compartment : m_compartments | std::views::values) {
        compartment->set_current_amount(compartment->get_initial_amount());
    }
}

void Simulation::stepSimulation(double dt) {
    m_current_time += dt;

    std::unordered_map<QString, double> delta_amounts;

    for (const auto& [symbol, compartment] : m_compartments) {
        delta_amounts[symbol] = 0.0;
    }

    /// Calculate changes based on connections
    for (const auto& connection : m_connections) {
        Compartment* source = connection->get_source();
        Compartment* target = connection->get_target();

        /// Expression parser called for rate? need to see this
        double rate = evaluateExpression(connection->get_rate_expression());

        /// based on claude -> not sure what is to be graphed i just asked generic compartamental project stuff
        double transfer_amount = rate * source->get_current_amount() * dt;

        delta_amounts[source->get_symbol()] -= transfer_amount;
        delta_amounts[target->get_symbol()] += transfer_amount;
    }

    /// setting amounts after each step
    QVariantMap values;
    for (const auto& [symbol, compartment] : m_compartments) {
        double new_amount = compartment->get_current_amount() + delta_amounts[symbol];
        compartment->set_current_amount(new_amount);
        values[symbol] = new_amount;
    }

    /// emit signal with current time and values
    emit simulationDataUpdated(m_current_time, values);
}

double Simulation::evaluateExpression(const QString& expression) {

    /// This is just some base expression parsing from claude can be changed to whatever u seem fit for

    std::string expr_str = expression.toStdString();

    bool ok;
    double value = expression.toDouble(&ok);
    if (ok) {
        return value;
    }

    if (m_variables.contains(expr_str)) {
        return m_variables[expr_str];
    }

    QStringList parts = expression.split('*');
    double result = 1.0;

    for (const QString& part : parts) {
        QString trimmed = part.trimmed();

        /// number check here
        value = trimmed.toDouble(&ok);
        if (ok) {
            result *= value;
            continue;
        }

        /// variable check here
        std::string part_str = trimmed.toStdString();
        if (m_variables.contains(part_str)) {
            result *= m_variables[part_str];
            continue;
        }

        /// COmpartment name check here
        if (m_compartments.contains(trimmed)) {
            result *= m_compartments[trimmed]->get_current_amount();
            continue;
        }

        qWarning() << "Unknown term in expression:" << trimmed;
        return 0.0;
    }

    return result;
}
