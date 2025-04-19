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

 private:
  Simulation* m_simulation = nullptr;
  QString m_name;
  std::shared_ptr<Compartment> m_source;
  std::shared_ptr<Compartment> m_target;
  QString m_rate_expression;

 public:
  explicit Connection(QObject* parent = nullptr) : QObject(parent) {}

  Connection(QString name, std::shared_ptr<Compartment> source, std::shared_ptr<Compartment> target,
             QString rate_expression, Simulation* parent = nullptr);

  [[nodiscard]] const QString& get_name() const {
    return m_name;
  }
  [[nodiscard]] std::shared_ptr<Compartment> get_source() const {
    return m_source;
  }
  [[nodiscard]] std::shared_ptr<Compartment> get_target() const {
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
  void set_source(std::shared_ptr<Compartment> source) {
    m_source = std::move(source);
  }
  void set_target(std::shared_ptr<Compartment> target) {
    m_target = std::move(target);
  }
  void set_rate_expression(const QString& rate_expression) {
    m_rate_expression = rate_expression;
  }
  void set_simulation(Simulation* simulation) {
    m_simulation = simulation;
  }
};

#endif  //CONNECTION_H
