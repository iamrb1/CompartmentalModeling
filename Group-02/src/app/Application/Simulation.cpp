/**
 * @file Simulation.cpp
 * @author Nitish Maindoliya, Matthew Hawkins, Rahul Baragur
 */

// #include "pch.h"
#include "Simulation.h"
#include <QApplication>
#include <QDirIterator>
#include <QFile>
#include <QXmlStreamReader>
#include <ranges>
#include "../Components/Compartment.h"

Simulation::Simulation(QObject* parent) : QObject(parent) {}

/**
 * @brief Completely resets the simulation and clears all relevant values
 */
void Simulation::clear() {
  m_is_running = false;

  // Disconnect timer
  if (m_timer) {
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
  m_symbol_table.clear();

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
  if (m_timer) {
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
  m_timer->start(m_step_time);
}

void Simulation::pause() {
  m_is_running = false;
  emit isRunningChanged();

  if (m_timer) {
    m_timer->stop();
  }
}

void Simulation::save() {
  if (!m_save_path.isEmpty()) {
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
    prompt("Could not open/save file", PromptType::ERR, PromptMode::TOAST);
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

/**
 * @brief load in a xml simulation file.
 * @param filename The file path for the xml file.
 */
void Simulation::load_xml(const QString& filename) {
  const QUrl fileUrl(filename);
  QFile file(fileUrl.toLocalFile());

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    prompt("Could not open file", PromptType::ERR, PromptMode::TOAST);
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
          compartment->set_initial_amount(initial);
          compartment->set_current_amount(initial);
          m_compartments[symbol] = std::move(compartment);
          bind_variable(symbol, m_compartments[symbol]->get_current_amount());
        }
      }
    } else if (xml.name() == "variables") {
      while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "variables")) {
        xml.readNext();
        if (xml.isStartElement() && xml.name() == "variable") {
          auto attrs = xml.attributes();
          auto name = attrs.value("name").toString();
          const double value = attrs.value("value").toDouble();
          add_variable(name, value);
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
    prompt("XML Error: File is not valid", PromptType::ERR, PromptMode::TOAST);
    clear();
  } else {
    emit compartmentsChanged();
    emit connectionsChanged();
    emit variablesChanged();
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
 * @brief Creates and adds a compartment to the simulation. Also sets the simulation for the compartment.
 */
void Simulation::add_compartment() {
  static size_t compartment_number = m_compartments.size();
  /// Generate a new compartment with a unique symbol and name
  QString symbol = QString("%1%2")
                       .arg(static_cast<char>(BASE_CHAR + (compartment_number % ALPHABET_SIZE)))
                       .arg((compartment_number / ALPHABET_SIZE) + 1);
  while (m_compartments.contains(symbol)) {
    compartment_number++;
    symbol = QString("%1%2")
                 .arg(static_cast<char>('A' + (compartment_number % ALPHABET_SIZE)))
                 .arg((compartment_number / ALPHABET_SIZE) + 1);
  }
  auto name = QString("Compartment %1").arg(symbol);

  auto compartment = std::make_shared<Compartment>(name, symbol, 0, this);
  m_compartments[symbol] = std::move(compartment);
  compartment_number++;

  /// Register the compartment value in the symbol table
  bind_variable(symbol, m_compartments[symbol]->get_current_amount());

  /// Emit signal to notify that compartments have changed
  emit compartmentsChanged();
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
 * @brief Updates compartment with new name/symbol
 * @param symbol Current compartment symbol
 * @param new_symbol Possible new compartment symbol
 */
bool Simulation::update_compartment_symbol(const QString& symbol, const QString& new_symbol) {
  static QRegularExpression regex("^[a-zA-Z0-9]+$");
  if (new_symbol.isEmpty() || !regex.match(new_symbol).hasMatch()) {
    prompt("Non alpha-numeric symbol is invalid", PromptType::WARNING, PromptMode::TOAST);
    return false;
  }

  if (!m_compartments.contains(symbol)) {
    prompt("Compartment does not exist", PromptType::WARNING, PromptMode::TOAST);
    return false;
  }

  if (symbol == new_symbol) {
    return false;
  }

  if (m_compartments.contains(new_symbol)) {
    prompt("Compartment with this symbol already exists", PromptType::WARNING, PromptMode::TOAST);
    return false;
  }

  auto node_handler = m_compartments.extract(symbol);
  node_handler.key() = new_symbol;
  m_compartments.insert(std::move(node_handler));

  unbind_variable(symbol);
  bind_variable(new_symbol, m_compartments[new_symbol]->get_current_amount());
  return true;
}

/**
 * @brief Removes a compartment from the simulation
 * @param symbol Compartment symbol
 */
void Simulation::remove_compartment(const QString& symbol) {
  if (!m_compartments.contains(symbol)) {
    prompt("Compartment does not exist", PromptType::WARNING, PromptMode::TOAST);
    return;
  }

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
  unbind_variable(symbol);
  m_compartments.erase(symbol);
  m_sidebar_compartment = nullptr;

  emit compartmentsChanged();
  emit sidebarCompartmentChanged();
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
 * @brief Sets the connection mode for the simulation
 * @param connection_mode The connection mode
 */
void Simulation::set_connection_mode(const bool connection_mode) {
  if (!connection_mode) {
    set_source_compartment(nullptr);
    set_target_compartment(nullptr);
  }

  m_connection_mode = connection_mode;
  emit connectionModeChanged();
}

/**
 * Set the source component and make the connection
 * @param compartment The source compartment
 */
void Simulation::set_source_compartment(Compartment* compartment) {
  if (!m_connection_mode) {
    return;
  }
  m_source_compartment = compartment;
  emit sourceCompartmentChanged();
}

/**
 * Set the target component and make the connection
 * @param target The target compartment
 */
void Simulation::set_target_compartment(Compartment* target) {
  if (!m_connection_mode) {
    return;
  }
  m_target_compartment = target;
  emit targetCompartmentChanged();
}

/**
 * @brief Adds a connection to the simulation
 */
void Simulation::add_connection() {
  if (!m_connection_mode || !m_source_compartment || !m_target_compartment) {
    set_source_compartment(nullptr);
    set_target_compartment(nullptr);
    return;
  }

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
  } else {
    prompt("Connection already exists", PromptType::WARNING, PromptMode::TOAST);
  }

  emit connectionsChanged();

  set_source_compartment(nullptr);
  set_target_compartment(nullptr);
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

/**
 * @brief Removes a connection from the simulation
 * @param connection Connection to be removed
 */
void Simulation::remove_connection(const Connection* connection) {
  if (const auto it =
          std::ranges::find_if(m_connections, [connection](const auto& conn) { return conn.get() == connection; });
      it != m_connections.end()) {
    m_connections.erase(it);
  }
  m_sidebar_connection = nullptr;

  emit connectionsChanged();
  emit sidebarConnectionChanged();
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
  QString final_name = name;
  if (name.isEmpty()) {
    final_name = QString("k%1").arg(variable_number++);
  }

  if (m_variables.contains(final_name)) {
    prompt("Variable with this name already exists", PromptType::WARNING, PromptMode::TOAST);
    emit variablesChanged();
    return;
  }

  m_variables.emplace(final_name, value);
  bind_variable(final_name, m_variables[final_name]);

  emit variablesChanged();
}

/**
 * @brief Updates variable with new name/value
 * @param name Current variable name
 * @param new_name Possible new variable name
 * @param value New variable value
 */
bool Simulation::update_variable(const QString& name, const QString& new_name, const double value) {
  if (!m_variables.contains(name)) {
    prompt("Variable does not exist", PromptType::WARNING, PromptMode::TOAST);
    return false;
  }

  static QRegularExpression regex("^[a-zA-Z0-9]+$");
  if (new_name.isEmpty() || !regex.match(new_name).hasMatch()) {
    prompt("Non alpha-numeric variable is invalid", PromptType::WARNING, PromptMode::TOAST);
    return false;
  }

  if (name == new_name) {
    m_variables[name] = value;
  } else {

    if (m_variables.contains(new_name)) {
      prompt("Variable with this name already exists", PromptType::WARNING, PromptMode::TOAST);
      return false;
    }

    // Extract node
    auto node_handler = m_variables.extract(name);
    node_handler.key() = new_name;
    node_handler.mapped() = value;
    m_variables.insert(std::move(node_handler));
  }

  unbind_variable(name);
  bind_variable(new_name, m_variables[new_name]);

  emit variablesChanged();
  return true;
}

/**
 * @brief Removes a variable from the simulation
 * @param name variable name
 */
void Simulation::remove_variable(const QString& name) {
  if (const auto it = m_variables.find(name); it != m_variables.end()) {
    unbind_variable(name);
    m_variables.erase(it);
    emit variablesChanged();
  }
}

/**
 * @brief Unbinds a variable from the symbol table
 * @param key variable name
 */
void Simulation::unbind_variable(const QString& key) {
  if (const auto std_key = key.toStdString(); m_symbol_table.symbol_exists(std_key)) {
    m_symbol_table.remove_variable(std_key);
  }
}

/**
 * @brief Binds a variable to the symbol table
 * @param key variable name
 * @param ref variable reference
 */
void Simulation::bind_variable(const QString& key, double& ref) {
  unbind_variable(key);
  m_symbol_table.add_variable(key.toStdString(), ref);
}

/**
 * @brief Evaluates an expression string and returns the result
 * @param expression_string The expression to evaluate
 * @return The result of the evaluation
 */
double Simulation::evaluate_expression(const QString& expression_string) {
  expression_t expression;
  expression.register_symbol_table(m_symbol_table);

  // Print the symbol table for debugging
  std::vector<std::string> variable_list;
  m_symbol_table.get_variable_list(variable_list);

  if (parser_t parser; !parser.compile(expression_string.toStdString(), expression)) {
    prompt("Expression parsing error. Check if your rate expressions are well formed", PromptType::ERR,
           PromptMode::TOAST);
    pause();
    return 0;
  }

  return expression.value();
}

/**
 * @brief Takes a time step in the simulation
 */
void Simulation::take_time_step() {
  if (m_current_time >= m_time_steps && m_timer) {
    disconnect(m_timer, nullptr, this, nullptr);
    pause();
    return;
  }

  std::unordered_map<QString, double> delta_amounts;

  /// Calculate changes based on connections
  for (const auto& connection : m_connections) {
    Compartment* source = connection->get_source();
    const Compartment* target = connection->get_target();
    const QString& rate = connection->get_rate_expression();

    const double transfer_amount = evaluate_expression(rate) * source->get_current_amount();

    delta_amounts[source->get_symbol()] -= transfer_amount;
    delta_amounts[target->get_symbol()] += transfer_amount;
  }

  QVariantMap new_compartment_amounts;
  for (const auto& [symbol, compartment] : m_compartments) {
    if (!delta_amounts.contains(symbol)) {
      continue;
    }

    const double new_amount = compartment->get_current_amount() + delta_amounts[symbol];
    compartment->set_current_amount(new_amount);
    new_compartment_amounts[compartment->get_symbol()] = new_amount;
  }

  emit addGraphingValues(m_current_time, new_compartment_amounts);

  m_current_time++;
  emit currentTimeChanged();
}

/**
 * @brief Displays a prompt message
 * @param message The message to display
 * @param type The type of prompt (error, warning, info)
 * @param mode The mode of the prompt (toast, dialog, log only)
 */
void Simulation::prompt(const QString& message, const Simulation::PromptType type, const Simulation::PromptMode mode) {
  if (type == PromptType::ERR) {
    qDebug() << "Error: " << message;
  } else if (type == PromptType::WARNING) {
    qDebug() << "Warning: " << message;
  } else if (type == PromptType::INFO) {
    qDebug() << "Info: " << message;
  }

  if (PromptMode::LOG_ONLY == mode) {
    return;
  }

  emit promptMessage(message, type, mode);
}
