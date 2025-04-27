/**
 * @file Simulation.h
 * @author Nitish Maindoliya, Rahul Baragur
 */

#ifndef SIMULATION_H
#define SIMULATION_H

#include <QList>
#include <QObject>
#include <QTimer>
#include <QVariant>
#include <exprtk.hpp>
#include <qqml.h>

#include "../Components/Compartment.h"
#include "../Components/Connection.h"

/**
 * Simulation Class:
 * Represents the entire Compartmental Modeling System
 * Holds all Compartments, Connections, Variables, and other relevant data
 */

class Simulation : public QObject {
  Q_OBJECT
  QML_ELEMENT
  Q_PROPERTY(QString name MEMBER m_name NOTIFY nameChanged FINAL)

  // Connections/Compartments
  Q_PROPERTY(QVector<Compartment*> compartments READ get_compartments NOTIFY compartmentsChanged FINAL)
  Q_PROPERTY(QVector<Connection*> connections READ get_connections NOTIFY connectionsChanged FINAL)
  Q_PROPERTY(QVariantMap variables READ get_variables NOTIFY variablesChanged FINAL)

  // Connection
  Q_PROPERTY(bool connectionMode MEMBER m_connection_mode WRITE set_connection_mode NOTIFY connectionModeChanged FINAL)
  Q_PROPERTY(Compartment* sourceCompartment MEMBER m_source_compartment WRITE set_source_compartment NOTIFY
                 sourceCompartmentChanged FINAL)
  Q_PROPERTY(Compartment* targetCompartment MEMBER m_target_compartment WRITE set_target_compartment NOTIFY
                 targetCompartmentChanged FINAL)

  // Sidebar
  Q_PROPERTY(Compartment* sidebarCompartment MEMBER m_sidebar_compartment WRITE set_sidebar_compartment NOTIFY
                 sidebarCompartmentChanged FINAL)
  Q_PROPERTY(Connection* sidebarConnection MEMBER m_sidebar_connection WRITE set_sidebar_connection NOTIFY
                 sidebarConnectionChanged FINAL)

  // Evolution
  Q_PROPERTY(int currentTime MEMBER m_current_time NOTIFY currentTimeChanged FINAL)
  Q_PROPERTY(int timeSteps MEMBER m_time_steps NOTIFY timeStepsChanged FINAL)
  Q_PROPERTY(bool isRunning MEMBER m_is_running NOTIFY isRunningChanged FINAL)

 public:
  enum PromptMode {
    TOAST,
    MODAL,
    LOG_ONLY,
  };
  Q_ENUM(PromptMode);

  enum PromptType {
    INFO,
    WARNING,
    ERR,
  };
  Q_ENUM(PromptType);

 signals:
  /// Signals to notify QML of changes
  void nameChanged();
  void compartmentsChanged();
  void connectionsChanged();
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
  void promptMessage(PromptType type, const QString& message, PromptMode mode = TOAST);

 private:
  static constexpr int DEFAULT_TIME_STEP_MS = 100;

  /// Simulation name
  QString m_name = "New Simulation";
  /// Save path
  QString m_save_path;

  /// Simulation time
  QTimer* m_timer = nullptr;
  int m_current_time = 0.0;
  int m_time_steps = 100;
  bool m_is_running = false;

  /// Unordered map of compartments with their symbols as keys
  std::unordered_map<QString, std::shared_ptr<Compartment>> m_compartments;
  /// Connections between compartments
  std::vector<std::shared_ptr<Connection>> m_connections;
  /// Variables for the simulation
  std::unordered_map<QString, double> m_variables;

  // Connection stuff
  /// Connection mode from the toolbar
  bool m_connection_mode = false;
  /// Source compartment
  Compartment* m_source_compartment = nullptr;
  /// Target Compartment
  Compartment* m_target_compartment = nullptr;

  // Sidebar stuff
  /// Selected connection
  Connection* m_sidebar_connection = nullptr;
  /// Selected compartment
  Compartment* m_sidebar_compartment = nullptr;

  //Parsing stuff
  typedef exprtk::symbol_table<double> symbol_table_t;
  typedef exprtk::expression<double> expression_t;
  typedef exprtk::parser<double> parser_t;
  symbol_table_t m_symbol_table;

  void bind_variable(const QString& key, double& ref);
  void unbind_variable(const QString& key);
  double evaluate_expression(const QString& expression_string);

 public:
  explicit Simulation(QObject* parent = nullptr);

  // Simulation controls
  /**
   * @brief Completely resets the simulation and clears all relevant values
   */
  Q_INVOKABLE void clear();
  Q_INVOKABLE void reset();
  Q_INVOKABLE void start();
  Q_INVOKABLE void pause();
  Q_INVOKABLE void save();
  Q_INVOKABLE void save_xml(const QString& filename);
  Q_INVOKABLE void load_xml(const QString& filename);

  // Compartment stuff
  [[nodiscard]] QVector<Compartment*> get_compartments() const;
  Q_INVOKABLE void add_compartment();
  void set_sidebar_compartment(Compartment* compartment);
  void update_compartment_symbol(const QString& symbol, const QString& new_symbol);
  Q_INVOKABLE void remove_compartment(const QString& symbol);

  // Connections stuff
  [[nodiscard]] QVector<Connection*> get_connections() const;
  void set_connection_mode(bool connection_mode);
  void set_source_compartment(Compartment* compartment);
  void set_target_compartment(Compartment* target);
  Q_INVOKABLE void add_connection();
  void set_sidebar_connection(Connection* connection);
  Q_INVOKABLE void remove_connection(const Connection* connection);

  // Variable stuff
  [[nodiscard]] QVariantMap get_variables() const;
  Q_INVOKABLE void add_variable(const QString& name = QString(), double value = 0.0);
  Q_INVOKABLE void update_variable(const QString& name, const QString& new_name, double value);
  Q_INVOKABLE void remove_variable(const QString& name);

 public slots:
  void take_time_step();
};

#endif  //SIMULATION_H
