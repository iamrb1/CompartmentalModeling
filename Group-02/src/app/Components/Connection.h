/**
 * @file Connection.h
 * @author Nitish Maindoliya
 */

#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <qqml.h>

class Simulation;
class Compartment;
/**
 * Connection Class:
 * Connections represent links between Compartment Objects
 * These represent movement of data from one Compartment to another
 */
class Connection : public QObject {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(QString name MEMBER m_name NOTIFY nameChanged FINAL)
  Q_PROPERTY(Compartment* source MEMBER m_source NOTIFY sourceChanged FINAL)
  Q_PROPERTY(Compartment* target MEMBER m_target NOTIFY targetChanged FINAL)
  Q_PROPERTY(QString rateExpression MEMBER m_rate_expression WRITE set_rate_expression NOTIFY rateExpressionChanged FINAL)

 signals:
  /// Signals to notify QML of changes
  void nameChanged();
  void sourceChanged();
  void targetChanged();
  void rateExpressionChanged();

 private:
  ///The parent simulation
  Simulation* m_simulation = nullptr;
  ///Name of connection
  QString m_name;
  ///Source compartment
  Compartment* m_source = nullptr;
  ///Target compartment
  Compartment* m_target = nullptr;
  ///Rate of transfer expression
  QString m_rate_expression;

 public:
  explicit Connection(QObject* parent = nullptr) : QObject(parent) {}

  Connection(QString name, Compartment* source, Compartment* target, QString rate_expression,
             Simulation* parent = nullptr);
  /**
   * @brief Getter for the name of the connection
   * @return Compartment name
   */
  [[nodiscard]] const QString& get_name() const {
    return m_name;
  }
  /**
   * @brief Getter for the source compartment
   * @return Source compartment
   */
  [[nodiscard]] Compartment* get_source() const {
    return m_source;
  }
  /**
   * @brief Getter for the target compartment
   * @return Target compartment
   */
  [[nodiscard]] Compartment* get_target() const {
    return m_target;
  }
  /**
   * @brief Getter for the rate of transfer
   * @return Rate of transfer expression
   */
  [[nodiscard]] const QString& get_rate_expression() const {
    return m_rate_expression;
  }
  /**
   * @brief Getter for the parent simulation
   * @return Parent simulation
   */
  [[nodiscard]] Simulation* get_simulation() const {
    return m_simulation;
  }
  /**
   * @brief Setter for the name of connection
   * @param name The name of the connection
   */
  void set_name(const QString& name) {
    m_name = name;
    emit nameChanged();
  }
  /**
   * @brief Setter for source compartment
   * @param source The source compartment
   */
  void set_source(Compartment* source) {
    m_source = source;
    emit sourceChanged();
  }
  /**
   * @brief Setter for target compartment
   * @param target The target compartment
   */
  void set_target(Compartment* target) {
    m_target = target;
    emit targetChanged();
  }
  /**
   * @brief Setter for the rate of transfer expression
   * @param rate_expression The rate of transfer
   */
  void set_rate_expression(const QString& rate_expression) {
    m_rate_expression = rate_expression;
    emit rateExpressionChanged();
  }
  /**
   * Setter for the parent simulation
   * @param simulation The simulation instance
   */
  void set_simulation(Simulation* simulation) {
    m_simulation = simulation;
  }
};

#endif  //CONNECTION_H
