/**
 * @file Connection.h
 * @author Nitish Maindoliya
 *
 *
 */

#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <qqml.h>

class Simulation;
class Compartment;

class Connection : public QObject {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(QString name MEMBER m_name NOTIFY nameChanged FINAL)
  Q_PROPERTY(Compartment* source MEMBER m_source NOTIFY sourceChanged FINAL)
  Q_PROPERTY(Compartment* target MEMBER m_target NOTIFY targetChanged FINAL)
  Q_PROPERTY(QString rateExpression MEMBER m_rate_expression NOTIFY rateExpressionChanged FINAL)

 signals:
  void nameChanged();
  void sourceChanged();
  void targetChanged();
  void rateExpressionChanged();

 private:
  Simulation* m_simulation = nullptr;
  QString m_name;
  Compartment* m_source = nullptr;
  Compartment* m_target = nullptr;
  QString m_rate_expression;

 public:
  explicit Connection(QObject* parent = nullptr) : QObject(parent) {}

  Connection(QString name, Compartment* source, Compartment* target, QString rate_expression,
             Simulation* parent = nullptr);

  [[nodiscard]] const QString& get_name() const {
    return m_name;
  }
  [[nodiscard]] Compartment* get_source() const {
    return m_source;
  }
  [[nodiscard]] Compartment* get_target() const {
    return m_target;
  }
  [[nodiscard]] const QString& get_rate_expression() const {
    return m_rate_expression;
  }
  [[nodiscard]] Simulation* get_simulation() const {
    return m_simulation;
  }

  void set_name(const QString& name) {
    m_name = name;
  }
  void set_source(Compartment* source) {
    m_source = source;
  }
  void set_target(Compartment* target) {
    m_target = target;
  }
  void set_rate_expression(const QString& rate_expression) {
    m_rate_expression = rate_expression;
  }
  void set_simulation(Simulation* simulation) {
    m_simulation = simulation;
  }
};

#endif  //CONNECTION_H
