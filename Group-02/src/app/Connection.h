/**
 * @file Connection.h
 * @author Nitish Maindoliya
 *
 *
 */

#ifndef CONNECTION_H
#define CONNECTION_H

#include <memory>
#include <string>

class Simulation;
class Compartment;

class Connection {
 private:
  Simulation* m_simulation = nullptr;

  std::string m_name;
  std::shared_ptr<Compartment> m_source;
  std::shared_ptr<Compartment> m_target;
  std::string m_rate_expression;

 public:
  Connection(std::string name, std::shared_ptr<Compartment> source, std::shared_ptr<Compartment> target, std::string rate_expression);
  /// Default constructor (deleted - name, source, and target and rate_expression are required)
  Connection() = delete;
  /// Destructor
  ~Connection() = default;

  [[nodiscard]] const std::string& get_name() const;
  [[nodiscard]] std::shared_ptr<Compartment> get_source() const;
  [[nodiscard]] std::shared_ptr<Compartment> get_target() const;
  [[nodiscard]] const std::string& get_rate_expression() const;
  [[nodiscard]] Simulation* get_simulation() const;

  void set_name(const std::string& name);
  void set_source(std::shared_ptr<Compartment> source);
  void set_target(std::shared_ptr<Compartment> target);
  void set_rate_expression(const std::string& rate_expression);
  void set_simulation(Simulation* simulation);
};

#endif  //CONNECTION_H
