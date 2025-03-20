/**
 * @file Event.h
 *
 * @author Owen Haiar
 * @brief Contains Event struct for storing a void function, its ID, and its priority
 */

#pragma once

#include <functional>

namespace cse {

/**
 * @brief A struct to represent an event
 */
template<typename... Args>
struct Event {
 private:
  int id_;
  int time_;
  std::function<void(Args...)> function_;

 public:
  Event(int id, int time, std::function<void(Args...)> function)
	  : id_(id), time_(time), function_(std::move(function)) {}
  [[nodiscard]] int getID() const { return id_; }
  [[nodiscard]] int getTime() const { return time_; }
  [[nodiscard]] const std::function<void(Args...)> &getFunction() const { return function_; }
  bool operator==(const Event &other) const { return id_ == other.id_; }
  bool operator!=(const Event &other) const { return id_ != other.id_; }
};

} // namespace cse

