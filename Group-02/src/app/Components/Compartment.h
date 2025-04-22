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

/**
 * Compartment Class:
 * This class represents containers in a Compartmental Modeling system
 * These hold a value, and can transfer values between other Compartments via Connections
 */
class Compartment : public QObject {
  Q_OBJECT
  QML_ELEMENT

  /// Expose m_name, m_symbol to QML
  Q_PROPERTY(QString name MEMBER m_name WRITE set_name NOTIFY nameChanged)
  Q_PROPERTY(QString symbol MEMBER m_symbol WRITE set_symbol NOTIFY symbolChanged)
  Q_PROPERTY(double initialAmount MEMBER m_initial_amount WRITE set_initial_amount NOTIFY initialAmountChnaged)
  Q_PROPERTY(double currentAmount MEMBER m_current_amount NOTIFY currentAmountChanged)
  Q_PROPERTY(QVector<double> amounts MEMBER m_amounts NOTIFY amountsChanged);
  Q_PROPERTY(int x MEMBER m_x NOTIFY xChanged)
  Q_PROPERTY(int y MEMBER m_y NOTIFY yChanged)

 signals:
  /// Signals to notify QML of changes
  void nameChanged();
  void symbolChanged();
  void xChanged();
  void yChanged();
  void initialAmountChnaged();
  void currentAmountChanged();
  void amountsChanged();

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
  /// Array of values for the compartment
  QVector<double> m_amounts;
  /// The x position of the compartment
  int m_x = 100;
  /// The y position of the compartment
  int m_y = 100;

 public:
  explicit Compartment(QObject* parent = nullptr) : QObject(parent) {}

  Compartment(QString name, QString symbol, double initial_amount, Simulation* parent = nullptr);
  /**
   * @brief Getter for the name of the compartment
   * @return Compartment name
   */
  [[nodiscard]] const QString& get_name() const {
    return m_name;
  }
  /**
   * @brief Getter for the symbol of the compartment
   * @return Compartment symbol
   */
  [[nodiscard]] const QString& get_symbol() const {
    return m_symbol;
  }
  /**
   * @brief Getter for the initial value
   * @return Compartment initial value
   */
  [[nodiscard]] double get_initial_amount() const {
    return m_initial_amount;
  }
  /**
   * @brief Getter for the current amount
   * @return Compartment current amount
   */
  [[nodiscard]] double get_current_amount() const {
    return m_current_amount;
  }
  /**
   * @brief Getter for X Position
   * @return Compartment X position
   */
  [[nodiscard]] int get_x() const {
    return m_x;
  }
  /**
   * @brief Getter for Y Position
   * @return Compartment Y position
   */
  [[nodiscard]] int get_y() const {
    return m_y;
  }

  void set_name(const QString& name);

  void set_symbol(const QString& symbol);

  void reset();

  void add_amount(double amount);

  void set_initial_amount(const double initial_amount);

  /**
   * Setter for Compartment current amount
   * @param current_amount
   */
  void set_current_amount(const double current_amount) {
    m_current_amount = current_amount;
    emit currentAmountChanged();
  }

  /**
   * @brief Setter for Compartment X Position
   * @param x
   */
  void set_x(const int x) {
    m_x = x;
    emit xChanged();
  }
  /**
   * @brief Setter for Compartment Y Position
   * @param Y
   */
  void set_y(const int y) {
    m_y = y;
    emit yChanged();
  }
  /**
   * Setter for the running Simulation
   * @param simulation
   */
  void set_simulation(Simulation* simulation) {
    m_simulation = simulation;
  }
};

#endif  //COMPARTMENT_H
