/**
 * @file Compartment.h
 * @author Nitish Maindoliya
 */

#ifndef COMPARTMENT_H
#define COMPARTMENT_H

#include <QObject>
#include <QString>
#include <qqml.h>
#include <utility>

class Connection;
class Simulation;

/**
 * Compartment Class:
 * This class represents containers in a Compartmental Modeling system
 * These hold a value, and can transfer values between other Compartments via Connections
 */
class Compartment : public QObject {
  Q_OBJECT
  QML_ELEMENT

  /// Expose m_name, m_symbol to QML
  Q_PROPERTY(QString name READ get_name WRITE set_name NOTIFY nameChanged)
  Q_PROPERTY(QString symbol READ get_symbol WRITE set_symbol NOTIFY symbolChanged)
  Q_PROPERTY(double initialAmount READ get_initial_amount WRITE set_initial_amount NOTIFY initialAmountChanged)
  Q_PROPERTY(double currentAmount READ get_current_amount NOTIFY currentAmountChanged)
  Q_PROPERTY(int x READ get_x WRITE set_x NOTIFY xChanged)
  Q_PROPERTY(int y READ get_y WRITE set_y NOTIFY yChanged)

 signals:
  /// Signals to notify QML of changes
  void nameChanged();
  void symbolChanged();
  void xChanged();
  void yChanged();
  void initialAmountChanged();
  void currentAmountChanged();

 private:
  /// The parent simulation
  Simulation* m_simulation = nullptr;
  /// The name of the compartment
  QString m_name = "Test";
  /// The symbol of the compartment (used for variable names)
  QString m_symbol = "X";
  /// The initial amount of the compartment
  double m_initial_amount = 0.0;
  /// The current amount of the compartment
  double m_current_amount = 0.0;
  /// The x position of the compartment
  int m_x = 100;
  /// The y position of the compartment
  int m_y = 100;

 public:
  explicit Compartment(QObject* parent = nullptr) : QObject(parent) {}

  Compartment(QString name, QString symbol, double initial_amount, Simulation* parent = nullptr);

  [[nodiscard]] const QString& get_name() const;
  [[nodiscard]] const QString& get_symbol() const;
  [[nodiscard]] double get_initial_amount() const;
  [[nodiscard]] double& get_current_amount();
  [[nodiscard]] int get_x() const;
  [[nodiscard]] int get_y() const;

  void reset();

  void set_name(const QString& name);
  void set_symbol(const QString& symbol);
  void set_initial_amount(double initial_amount);
  void set_current_amount(double current_amount);
  void set_x(int x);
  void set_y(int y);
  void set_simulation(Simulation* simulation);
};

#endif  //COMPARTMENT_H
