/**
 * @file Connection.h
 * @author Nitish Maindoliya
 */

#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QRegularExpression>
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

  Q_PROPERTY(QString name READ get_name WRITE set_name NOTIFY nameChanged FINAL)
  Q_PROPERTY(Compartment* source READ get_source WRITE set_source NOTIFY sourceChanged FINAL)
  Q_PROPERTY(Compartment* target READ get_target WRITE set_target NOTIFY targetChanged FINAL)
  Q_PROPERTY(
      QString rateExpression READ get_rate_expression WRITE set_rate_expression NOTIFY rateExpressionChanged FINAL)

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
  /// Delete default constructor
  Connection() = delete;
  /// Delete copy constructor
  Connection(const Connection&) = delete;
  /// Delete copy assignment operator
  Connection& operator=(const Connection&) = delete;

  Connection(QString name, Compartment* source, Compartment* target, QString rate_expression, Simulation* parent);

  [[nodiscard]] const QString& get_name() const;
  [[nodiscard]] Compartment* get_source() const;
  [[nodiscard]] Compartment* get_target() const;
  [[nodiscard]] const QString& get_rate_expression() const;
  [[nodiscard]] Simulation* get_simulation() const;

  void set_name(const QString& name);
  void set_source(Compartment* source);
  void set_target(Compartment* target);
  void set_rate_expression(const QString& rate_expression);
};

#endif  //CONNECTION_H
