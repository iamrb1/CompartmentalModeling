/**
 * @file Compartment.h
 * @author Nitish Maindoliya
 *
 *
 */

#ifndef COMPARTMENT_H
#define COMPARTMENT_H

#include <QObject>
#include <QString>
#include <qqml.h>
#include <utility>

class Connection;
class Simulation;

class Compartment : public QObject {
  Q_OBJECT
  QML_ELEMENT

  /// Expose m_name, m_symbol to QML
  Q_PROPERTY(QString name MEMBER m_name WRITE set_name NOTIFY nameChanged)
  Q_PROPERTY(QString symbol MEMBER m_symbol WRITE set_symbol NOTIFY symbolChanged)
  Q_PROPERTY(double initialAmount MEMBER m_initial_amount NOTIFY initialAmountChnaged)
  Q_PROPERTY(int x MEMBER m_x NOTIFY xChanged)
  Q_PROPERTY(int y MEMBER m_y NOTIFY yChanged)

 signals:
  /// Signals to notify QML of changes
  void nameChanged();
  void symbolChanged();
  void xChanged();
  void yChanged();
  void initialAmountChnaged();

 private:
  /// The parent simulation
  Simulation* m_simulation = nullptr;
  /// The name of the compartment
  QString m_name = "Test";
  /// The symbol of the compartment (used for variable names)
  QString m_symbol = "X";
  /// The initial amount of the compartment
  double m_initial_amount = 0.0f;
  /// The current amount of the compartment
  double m_current_amount = 0.0f;
  /// The x position of the compartment
  int m_x = 100;
  /// The y position of the compartment
  int m_y = 100;

 public:
  explicit Compartment(QObject* parent = nullptr) : QObject(parent) {}

  Compartment(QString name, QString symbol, double initial_amount, Simulation* parent = nullptr);

  [[nodiscard]] const QString& get_name() const {
    return m_name;
  }
  [[nodiscard]] const QString& get_symbol() const {
    return m_symbol;
  }
  [[nodiscard]] double get_initial_amount() const {
    return m_initial_amount;
  }
  [[nodiscard]] double get_current_amount() const {
    return m_current_amount;
  }
  [[nodiscard]] int get_x() const {
    return m_x;
  }
  [[nodiscard]] int get_y() const {
    return m_y;
  }

  void set_name(const QString& name) {
    m_name = name;
    emit nameChanged();
  }
  void set_symbol(const QString& symbol) {
    if (symbol.isEmpty()) {
      // TODO: Add error
      return;
    } else {
      
    }

    m_symbol = symbol;
    emit symbolChanged();
  }
  void set_initial_amount(const double initial_amount) {
    m_initial_amount = initial_amount;
    emit initialAmountChnaged();
  }
  void set_current_amount(const double current_amount) {
    m_current_amount = current_amount;
  }
  void set_x(const int x) {
    m_x = x;
    emit xChanged();
  }
  void set_y(const int y) {
    m_y = y;
    emit yChanged();
  }

  void set_simulation(Simulation* simulation) {
    m_simulation = simulation;
  }
};

#endif  //COMPARTMENT_H
