/**
 * @file Simulation.h
 * @author Nitish Maindoliya Rahul Baragur
 *
 *
 */

#ifndef SIMULATION_H
#define SIMULATION_H

#include <QList>
#include <QObject>
#include <qqml.h>
#include <QVariant>

#include "Components/Compartment.h"
#include "Components/Connection.h"
/**
 * Simulation Class:
 * Represents the entire Compartmental Modeling System
 * Holds all Compartments, Connections, Variables, and other relevant data
 */
class Simulation : public QObject {
  Q_OBJECT
  QML_ELEMENT
  Q_PROPERTY(QVector<Compartment*> compartments READ get_compartments NOTIFY compartmentsChanged)
  Q_PROPERTY(QVector<Connection*> connections READ get_connections NOTIFY connectionsChanged)
  Q_PROPERTY(QString name MEMBER m_name NOTIFY nameChanged)
  Q_PROPERTY(QVariantMap variables READ get_variables NOTIFY variablesChanged)
  Q_PROPERTY(bool connectionMode MEMBER m_connection_mode NOTIFY connectionModeChanged)
  Q_PROPERTY(Compartment* sourceCompartment MEMBER m_source_compartment NOTIFY sourceCompartmentChanged)
  Q_PROPERTY(Compartment* targetCompartment MEMBER m_target_compartment WRITE set_target_compartment NOTIFY targetCompartmentChanged)
  Q_PROPERTY(Compartment* sidebarCompartment MEMBER m_sidebar_compartment WRITE set_sidebar_compartment NOTIFY sidebarCompartmentChanged)
  Q_PROPERTY(Connection* sidebarConnection MEMBER m_sidebar_connection WRITE set_sidebar_connection NOTIFY sidebarConnectionChanged)

 signals:
  /// Signals to notify QML of changes
  void compartmentsChanged();
  void connectionsChanged();
  void nameChanged();
  void variablesChanged();
  void sourceCompartmentChanged();
  void targetCompartmentChanged();
  void connectionModeChanged();
  void sidebarCompartmentChanged();
  void sidebarConnectionChanged();
  void simulationDataUpdated(double time, const QVariantMap& values);

 private:
  /// Simulation name
  QString m_name = "New Simulation";
  /// Save path
  QString m_save_path;

  /// Simulation time

  double m_current_time = 0.0;

  /// Unordered map of compartments with their symbols as keys
  std::unordered_map<QString, std::shared_ptr<Compartment>> m_compartments;
  /// Connections between compartments
  std::vector<std::shared_ptr<Connection>> m_connections;

  /// Variables for the simulation
  std::unordered_map<std::string, double> m_variables = {{"k1", 0.01}, {"k2", 0.1}, {"k3", 0.5}};

  /// UI variables

  /// Connection mode from the toolbar
  bool m_connection_mode = false;
  /// Source compartment
  Compartment* m_source_compartment = nullptr;
  /// Target Compartment
  Compartment* m_target_compartment = nullptr;

  /// Sidebar element
  /// Selected connection
  Connection* m_sidebar_connection = nullptr;
  /// Selected compartment
  Compartment* m_sidebar_compartment = nullptr;

  Q_INVOKABLE double evaluateExpression(const QString& expression);

 public:
  explicit Simulation(QObject* parent = nullptr) : QObject(parent) {}

  Q_INVOKABLE void add_compartment();
  // Q_INVOKABLE void add_connection(Compartment* source, Compartment* target);

  Q_INVOKABLE void startSimulation();
  Q_INVOKABLE void stepSimulation(double dt);
  /**
   * @brief Completely resets the simulation and clears all relevant values
   */
  void clear_simulation() {
    m_current_time = 0.0;
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

  [[nodiscard]] QVector<Compartment*> get_compartments() const;
  [[nodiscard]] QVector<Connection*> get_connections() const;
  [[nodiscard]] QVariantMap get_variables() const;

  void set_sidebar_compartment(Compartment* compartment);
  void set_sidebar_connection(Connection* connection);

  Q_INVOKABLE void add_variable(const QString& name=QString(), double value = 0.0);
  Q_INVOKABLE void remove_variable(const QString& name);
  Q_INVOKABLE void update_variable(const QString& name, const QString& new_name, double value);

  Q_INVOKABLE void remove_connection(const Connection* connection);
  Q_INVOKABLE void remove_compartment(const QString& symbol);

  void set_target_compartment(Compartment* target);
};

#endif  //SIMULATION_H
