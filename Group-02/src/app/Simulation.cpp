/**
 * @file Simulation.cpp
 * @author Nitish Maindoliya, Matthew Hawkins
 */

#include "Simulation.h"
#include <QFile>
#include <QXmlStreamReader>
#include <stdexcept>

/**
 * @brief Add a compartment to the simulation. Also sets the simulation for the compartment.
 * @param compartment Compartment to add
 */
void Simulation::add_compartment(std::shared_ptr<Compartment> compartment) {
  compartment->set_simulation(this);
  m_compartments.push_back(std::move(compartment));
}

/**
 * @brief Add a connection to the simulation. Also sets the simulation for the connection.
 * @param connection Connection to add
 */
void Simulation::add_connection(std::shared_ptr<Connection> connection) {
  connection->set_simulation(this);
  m_connections.push_back(std::move(connection));
}

/**
 * @brief load in a xml simulation file.
 * @param filename The file path for the xml file.
 */
void Simulation::load_xml(const std::string& filename) {
  QFile file(QString::fromStdString(filename));
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    throw std::runtime_error("Cannot open file for reading: " + filename);
  }

  QXmlStreamReader xml(&file);
  m_save_path = filename;

  std::unordered_map<std::string, std::shared_ptr<Compartment>> comp_map;

  // find simulation name
  while (!xml.atEnd() && !xml.hasError()) {
    xml.readNext();
    if (xml.isStartElement() && xml.name() == QLatin1String("simulation")) {
      m_name = xml.attributes().value("name").toString().toStdString();
      break;
    }
  }

  // get the children of simulation
  while (!xml.atEnd() && !xml.hasError()) {
    xml.readNext();
    if (!xml.isStartElement())
      continue;

    if (xml.name() == QLatin1String("compartments")) {
      while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QLatin1String("compartments"))) {
        xml.readNext();
        if (xml.isStartElement() && xml.name() == QLatin1String("compartment")) {
          auto attrs = xml.attributes();
          std::string sym = attrs.value("name").toString().toStdString();
          float initial = attrs.value("initial").toFloat();

          auto comp_ptr = std::make_shared<Compartment>(sym, sym, initial);
          add_compartment(comp_ptr);
          comp_map[sym] = comp_ptr;
        }
      }
    } else if (xml.name() == QLatin1String("parameters")) {
      while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QLatin1String("parameters"))) {
        xml.readNext();
        if (xml.isStartElement() && xml.name() == QLatin1String("parameter")) {
          auto attrs = xml.attributes();
          std::string key = attrs.value("name").toString().toStdString();
          float value = attrs.value("value").toFloat();
          m_variables[key] = value;
        }
      }
    } else if (xml.name() == QLatin1String("connections")) {
      while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QLatin1String("connections"))) {
        xml.readNext();
        if (xml.isStartElement() && xml.name() == QLatin1String("connection")) {
          auto attrs = xml.attributes();
          std::string nm = attrs.value("name").toString().toStdString();
          std::string from = attrs.value("from").toString().toStdString();
          std::string to = attrs.value("to").toString().toStdString();
          std::string rate = attrs.value("rate").toString().toStdString();

          auto source_iter = comp_map.find(from);
          auto to_iter = comp_map.find(to);
          if (source_iter == comp_map.end() || to_iter == comp_map.end()) {
            throw std::runtime_error("Unknown compartment in connection: " + from + " → " += to);
          }

          auto connPtr = std::make_shared<Connection>(nm, source_iter->second, to_iter->second, rate);
          add_connection(connPtr);
        }
      }
    }
  }

  if (xml.hasError()) {
    throw std::runtime_error("XML parse error in " + filename + ": " + xml.errorString().toStdString());
  }
}

/**
 * @brief save the current simulation as a xml simulation file.
 * @param filename The file path for the xml file.
 */
void Simulation::save_xml(const std::string& filename) {
  QFile file(QString::fromStdString(filename));
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    throw std::runtime_error("Cannot open file for writing: " + filename);
  }

  QXmlStreamWriter xml(&file);
  xml.setAutoFormatting(true);
  xml.writeStartDocument();

  xml.writeStartElement("simulation");
  xml.writeAttribute("name", QString::fromStdString(m_name));

  xml.writeStartElement("compartments");
  for (auto& comp : m_compartments) {
    xml.writeStartElement("compartment");
    xml.writeAttribute("name", QString::fromStdString(comp->get_symbol()));
    xml.writeAttribute("initial", QString::number(comp->get_initial_amount()));
    xml.writeEndElement();
  }
  xml.writeEndElement();

  xml.writeStartElement("parameters");
  for (auto& kv : m_variables) {
    xml.writeStartElement("parameter");
    xml.writeAttribute("name", QString::fromStdString(kv.first));
    xml.writeAttribute("value", QString::number(kv.second));
    xml.writeEndElement();
  }
  xml.writeEndElement();

  xml.writeStartElement("connections");
  for (auto& conn : m_connections) {
    xml.writeStartElement("connection");
    xml.writeAttribute("name", QString::fromStdString(conn->get_name()));
    xml.writeAttribute("from", QString::fromStdString(conn->get_source()->get_symbol()));
    xml.writeAttribute("to", QString::fromStdString(conn->get_target()->get_symbol()));
    xml.writeAttribute("rate", QString::fromStdString(conn->get_rate_expression()));
    xml.writeEndElement();
  }

  xml.writeEndElement();
  xml.writeEndElement();
  xml.writeEndDocument();
  file.close();
}
