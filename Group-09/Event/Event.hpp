/**
 * @file Event.hpp
 *
 * @author Owen Haiar
 * @brief An Event contains an id, priority, function, and the arguments to call it with
 */

#pragma once

#include <functional>

namespace cse {

/**
 * @brief A struct to represent an event
 * @tparam Args The types of the arguments to pass to the function
 */
template<typename... Args>
struct Event {
 private:
  int id_; /**< Unique identifier for the event */
  int time_; /**< Priority for event execution */
  std::function<void(Args...)> function_; /**< Function to be executed */
  std::tuple<Args...> args_; /**< Args to pass to function_ */
 public:
  Event(int id, int time, std::function<void(Args...)> function, Args... args)
	  : id_(id), time_(time), function_(function), args_(std::make_tuple(std::forward<Args>(args)...)) {}

  /**
   * @brief Get the id of the event
   */
  [[nodiscard]] int getID() const { return id_; }

  /**
   * @brief Get the time of the event
   */
  [[nodiscard]] int getTime() const { return time_; }

  /**
   * @brief Set the time of the event
   * @param time The new time for the event
   * @attention This will not reheapfiy an EventQueue, use EventQueue::update() for that
   */
  [[nodiscard]] int setTime(int time) { time_ = time; }

  /**
   * @brief Execute the event by calling its function with its arguments
   */
  void execute() { std::apply(function_, args_); }

  /**
   * Equality based on id
   */
  bool operator==(const Event &other) const { return id_ == other.id_; }
  bool operator!=(const Event &other) const { return id_ != other.id_; }
};

} // namespace cse

