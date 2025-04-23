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
#include <QTimer>

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
  Q_PROPERTY(QString name MEMBER m_name NOTIFY nameChanged)

  // Error module
  Q_PROPERTY(QString error_name MEMBER m_error_message NOTIFY errorModuleShow)


    // Connections/Compartments
  Q_PROPERTY(QVector<Compartment*> compartments READ get_compartments NOTIFY compartmentsChanged)
  Q_PROPERTY(QVector<Connection*> connections READ get_connections NOTIFY connectionsChanged)
  Q_PROPERTY(QVariantMap variables READ get_variables NOTIFY variablesChanged)

  // Connection
  Q_PROPERTY(bool connectionMode MEMBER m_connection_mode WRITE set_m_connection_mode NOTIFY connectionModeChanged)
  Q_PROPERTY(Compartment* sourceCompartment MEMBER m_source_compartment NOTIFY sourceCompartmentChanged)
  Q_PROPERTY(Compartment* targetCompartment MEMBER m_target_compartment WRITE set_target_compartment NOTIFY targetCompartmentChanged)

  // Sidebar
  Q_PROPERTY(Compartment* sidebarCompartment MEMBER m_sidebar_compartment WRITE set_sidebar_compartment NOTIFY sidebarCompartmentChanged)
  Q_PROPERTY(Connection* sidebarConnection MEMBER m_sidebar_connection WRITE set_sidebar_connection NOTIFY sidebarConnectionChanged)

  // Evolution
  Q_PROPERTY(int currentTime MEMBER m_current_time NOTIFY currentTimeChanged)
  Q_PROPERTY(int timeSteps MEMBER m_time_steps NOTIFY timeStepsChanged)
  Q_PROPERTY(bool isRunning MEMBER m_is_running  NOTIFY isRunningChanged)

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
  void currentTimeChanged();
  void timeStepsChanged();
  void isRunningChanged();
  void addGraphingValues(double time, QVariant new_compartment_amounts);
  void errorModuleShow(const QString &message);

 private:
  /// Simulation name
  QString m_name = "New Simulation";
  /// Save path
  QString m_save_path;

  /// Simulation time
  QTimer* m_timer = nullptr;
  int m_current_time = 0.0;
  int m_time_steps = 100;
  bool m_is_running = false;

  /// error message
  QString m_error_message;


    /// Unordered map of compartments with their symbols as keys
  std::unordered_map<QString, std::shared_ptr<Compartment>> m_compartments;
  /// Connections between compartments
  std::vector<std::shared_ptr<Connection>> m_connections;

  /// Variables for the simulation
  std::unordered_map<QString, double> m_variables;

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

  // Q_INVOKABLE double evaluateExpression(const QString& expression);

 public:
  explicit Simulation(QObject* parent = nullptr);

  Q_INVOKABLE void add_compartment();

  /**
   * @brief Completely resets the simulation and clears all relevant values
   */
  Q_INVOKABLE void clear();

  Q_INVOKABLE void reset();
  Q_INVOKABLE void start();
  Q_INVOKABLE void pause();

  Q_INVOKABLE void load_xml(const QString& filename);
  Q_INVOKABLE void save ();
  Q_INVOKABLE void save_xml(const QString& filename);

  [[nodiscard]] QVector<Compartment*> get_compartments() const;
  [[nodiscard]] QVector<Connection*> get_connections() const;
  [[nodiscard]] QVariantMap get_variables() const;

  void set_sidebar_compartment(Compartment* compartment);
  void set_sidebar_connection(Connection* connection);
  void set_m_connection_mode(bool connection_mode);

  void update_compartment_symbol(const QString& symbol, const QString& new_symbol);

  Q_INVOKABLE void add_variable(const QString& name=QString(), double value = 0.0);
  Q_INVOKABLE void remove_variable(const QString& name);
  Q_INVOKABLE void update_variable(const QString& name, const QString& new_name, double value);
  Q_INVOKABLE void throw_error(const QString &message);


  Q_INVOKABLE void remove_connection(const Connection* connection);
  Q_INVOKABLE void remove_compartment(const QString& symbol);

  void set_target_compartment(Compartment* target);

 public slots:
  void take_time_step();
};

#endif  //SIMULATION_H
