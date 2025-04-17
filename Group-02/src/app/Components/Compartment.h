/**
 * @file Compartment.h
 * @author Nitish Maindoliya
 *
 *
 */

#ifndef COMPARTMENT_H
#define COMPARTMENT_H

#include <QObject>
#include <QtQml>
#include <string>

class Connection;
class Simulation;

class Compartment final {

 private:
  /// The parent simulation
  Simulation* m_simulation = nullptr;
  /// The name of the compartment
  std::string m_name;
  /// The symbol of the compartment (used for variable names)
  std::string m_symbol;
  /// The initial amount of the compartment
  float m_initial_amount = 0.0f;
  /// The current amount of the compartment
  float m_current_amount = 0.0f;
  /// The x position of the compartment
  int m_x = 0;
  /// The y position of the compartment
  int m_y = 0;

 public:
  Compartment(std::string name, std::string symbol, float initial_amount = 0.0f);
  /// Default constructor (deleted - name and symbol are required)
  Compartment() = delete;

  [[nodiscard]] const std::string& get_name() const;
  [[nodiscard]] const std::string& get_symbol() const;
  [[nodiscard]] float get_initial_amount() const;
  [[nodiscard]] float get_current_amount() const;
  [[nodiscard]] int get_x() const;
  [[nodiscard]] int get_y() const;

  void set_name(const std::string& name);
  void set_symbol(const std::string& symbol);
  void set_initial_amount(float initial_amount);
  void set_current_amount(float current_amount);
  void set_simulation(Simulation* simulation);
  void set_x(int x);
  void set_y(int y);
};

#endif  //COMPARTMENT_H
