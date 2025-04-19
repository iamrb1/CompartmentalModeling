/**
 * @file Simulation.h
 * @author Nitish Maindoliya
 *
 *
 */

#ifndef SIMULATION_H
#define SIMULATION_H

#include <QList>
#include <QObject>
#include <qqml.h>

#include "Components/Compartment.h"
#include "Components/Connection.h"

class Simulation : public QObject {
  Q_OBJECT
  QML_ELEMENT
  Q_PROPERTY(QVector<QObject*> compartments READ get_compartments_as_qobject NOTIFY compartmentsChanged)
  Q_PROPERTY(QString name MEMBER m_name NOTIFY nameChanged)
  Q_PROPERTY(QMap<QString, double> variables READ get_variables_as_qobject NOTIFY variablesChanged)

 signals:
  void compartmentsChanged();
  void nameChanged();
  void variablesChanged();

 private:
  /// Simulation name
  QString m_name = "New Simulation";
  /// Save path
  QString m_save_path;

  /// Simulation time
  double m_time = 0.0;

  /// Unordered map of compartments with their symbols as keys
  std::unordered_map<QString, std::shared_ptr<Compartment>> m_compartments;
  /// Connections between compartments
  std::vector<std::shared_ptr<Connection>> m_connections;
  /// Compartment number
  int m_compartment_number = 0;

  /// Variables for the simulation
  std::unordered_map<std::string, double> m_variables = {{"k1", 0.01f}, {"k2", 0.1f}, {"k3", 0.5f}};

 public:
  explicit Simulation(QObject* parent = nullptr) : QObject(parent) {}

  Q_INVOKABLE void add_compartment();

  void clear_simulation() {
    m_time = 0.0;
    m_compartment_number = 0;
    m_save_path.clear();
    m_compartments.clear();
    m_connections.clear();
    m_variables.clear();
  }

  // void reset_simulation() {
  //   m_time = 0.0;
  //   for (auto& compartment : m_compartments) {
  //     compartment->reset();
  //   }
  // }

  Q_INVOKABLE void load_xml(const QString& filename);
  Q_INVOKABLE void save_xml(const QString& filename);

  [[nodiscard]] QVector<QObject*> get_compartments_as_qobject() const;
  [[nodiscard]] QMap<QString, double> get_variables_as_qobject() const;

  Q_INVOKABLE void add_variable(const QString& name, double value);
  Q_INVOKABLE void remove_variable(const QString& name);
  Q_INVOKABLE void update_variable(const QString& name, double value);
};

#endif  //SIMULATION_H
