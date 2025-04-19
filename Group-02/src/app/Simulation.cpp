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
  // Generate a new compartment with a unique symbol
  auto symbol = QString(static_cast<char>('A' + m_compartment_number));
  auto name = QString("Compartment %1").arg(symbol);

  auto compartment = std::make_shared<Compartment>(name, symbol, 0, this);
  m_compartments[symbol] = std::move(compartment);
  m_compartment_number++;

  // Emit signal to notify that compartments have changed
  emit compartmentsChanged();
}

/**
 * @brief Return a list of all compartments in the simulation.
 */
QVector<QObject*> Simulation::get_compartments_as_qobject() const {
  QVector<QObject*> compartments;
  for (const auto& compartment : m_compartments | std::views::values) {
    compartments.push_back(compartment.get());
  }
  return compartments;
}

/**
 * @brief Return a map of all variables in the simulation.
 */
QMap<QString, double> Simulation::get_variables_as_qobject() const {
    QMap<QString, double> variables;
    for (const auto& pair : m_variables) {
        variables.insert(QString::fromStdString(pair.first), pair.second);
    }

//    qDebug() << "Variables in C++:";
//    for (auto it = variables.constBegin(); it != variables.constEnd(); ++it) {
//        qDebug() << "Key:" << it.key() << "Value:" << it.value();
//    }

    return variables;
}

Q_INVOKABLE void Simulation::add_variable(const QString& name, double value) {
    if (name.isEmpty()) {
        int maxNum = 0;
        for (const auto& pair : m_variables) {
            QString key = QString::fromStdString(pair.first);
            if (key.startsWith("k")) {
                bool ok;
                int num = key.mid(1).toInt(&ok);
                if (ok && num > maxNum) {
                    maxNum = num;
                }
            }
        }

        QString nextName = QString("k%1").arg(maxNum + 1);
        m_variables[nextName.toStdString()] = value;
    } else {
        m_variables[name.toStdString()] = value;
    }

    emit variablesChanged();
}

void Simulation::remove_variable(const QString& name) {
  std::string stdName = name.toStdString();
  auto it = m_variables.find(stdName);
  if (it != m_variables.end()) {
    m_variables.erase(it);
    emit variablesChanged();
  }
}

void Simulation::update_variable(const QString& name, double value) {
  std::string stdName = name.toStdString();
  m_variables[stdName] = value;
  emit variablesChanged();
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
    if (xml.isStartElement() && xml.name() == QLatin1String("simulation")) {
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
    } else if (xml.name() == QLatin1String("variables")) {
      while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "variables")) {
        xml.readNext();
        if (xml.isStartElement() && xml.name() == "variable") {
          auto attrs = xml.attributes();
          std::string key = attrs.value("name").toString().toStdString();
          const double value = attrs.value("value").toDouble();
          m_variables[key] = value;
        }
      }
    } else if (xml.name() == QLatin1String("connections")) {
      while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "connections")) {
        xml.readNext();
        if (xml.isStartElement() && xml.name() == QLatin1String("connection")) {
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

          auto connection = std::make_shared<Connection>(name, from_compartment, to_compartment, rate, this);
          m_connections.push_back(std::move(connection));
        }
      }
    }
  }

  if (xml.hasError()) {
    qWarning() << "XML Error:" << xml.errorString();
  } else {
    emit compartmentsChanged();
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

QStringList Simulation::getVariableNames() const {
    QStringList names;
    for (const auto& pair : m_variables) {
        names.append(QString::fromStdString(pair.first));
    }

    std::sort(names.begin(), names.end(), [](const QString &a, const QString &b) {
        if (a.startsWith('k') && b.startsWith('k')) {
            bool okA, okB;
            int numA = a.mid(1).toInt(&okA);
            int numB = b.mid(1).toInt(&okB);

            if (okA && okB) {
                return numA > numB;
            }
        }
        return a > b;
    });

    return names;
}

double Simulation::getVariableValue(const QString& name) const {
    auto it = m_variables.find(name.toStdString());
    if (it != m_variables.end()) {
        return it->second;
    }
    return 0.0;
}
