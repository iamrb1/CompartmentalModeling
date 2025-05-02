/**
 * @file EventManager.hpp
 * @author Grace Fitzgerald, Owen Haiar
 * @brief Contains EventManager class
 */

#pragma once

#include <atomic>
#include <cassert>
#include <chrono>
#include <iostream>
#include <map>
#include <set>
#include <thread>
#include <utility>

#include "../Event/Event.hpp"
#include "../EventQueue/EventQueue.hpp"

namespace cse {

/**
 * @brief Coordinates scheduled events, allowing for pausing, resuming,
 * and handling of recurring events.
 *
 *
 * Key Features:
 *  - Starts and stops event queue
 *  - Uniquely assigns ids to and creates events
 *  - Tracks paused and currently running events
 *  - Allows events to repeat at time intervals
 *
 * @tparam Args The types of the arguments to pass to the Events
 */
template <typename... Args>
class EventManager {

 private:

  static constexpr double NS_TO_S = 1e9;
  EventQueue<Args...> event_queue_; /**< Queue of events to be triggered */
  bool running_{0}; /**< Tracks if queue is currently running */
  int next_id_{0}; /**< ID for the next event to be created */
  std::chrono::steady_clock::time_point start_time_; /**< Time when queue was last started */
  std::chrono::duration<double> total_runtime_{0}; /**< Total time queue has been running */
  std::set<int> running_events_; /**< Set of currently running events */
  std::unordered_map<int, Event<Args...>> paused_events_; /**< Map of paused events */
  std::unordered_map<int, int> repeat_events_; /**< Map of events to repeat */

 public:
  EventManager() = default;
  ~EventManager() { StopQueue(); };
  /**
   * @brief Checks for and triggers events
   */
  void TriggerEvents() {

    while (event_queue_.size() && event_queue_.peek().getTime() <= getTime()) {  // Events to be popped
      auto e = event_queue_.peek();

      if (paused_events_.find(std::stoi(e.getID())) != paused_events_.end()) {
        event_queue_.pop();  // Skip over paused events
        continue;
      }

      e.execute();

      //Handle repeating events
      if (repeat_events_.find(std::stoi(e.getID())) != repeat_events_.end()) {
        e.setTime(e.getTime() +
                  repeat_events_[std::stoi(e.getID())]);  // Readd repeats to the queue
        event_queue_.update(e);
      } else {
        running_events_.erase(std::stoi(e.getID()));
        event_queue_.pop();
      }

      // Add a wait time to reduce CPU strain
      std::this_thread::sleep_for(std::chrono::milliseconds(10));

    }

    //Check for empty queue
    if (event_queue_.size() == 0) {
      running_ = false;
    }

  }

  /**
   * @brief Stops clock from running and triggering events
   */
  void StopQueue() noexcept{
    running_ = false;
    if (start_time_ == std::chrono::steady_clock::time_point{}) {
      total_runtime_ += std::chrono::steady_clock::now() - start_time_;
    }
  }

  /**
   * @brief Starts queue allowing for time updates
   */
  void StartQueue() noexcept{
    if (!running_) {
      running_ = true;
      start_time_ = std::chrono::steady_clock::now();
    }
  }

  /**
   * @brief Restarts clock
   */
  void RestartQueue() {
    StopQueue();
    total_runtime_ = std::chrono::duration<double>::zero();
    StartQueue();
  }

  /**
   * @brief Adds event to event_queue
   * @param time The time in seconds when the event should be triggered
   * @param func The function to be called when the event is triggered
   * @param args The arguments to be passed to the function
   * @return added Event
   */
  template <typename FUN_T>
  std::optional<Event<Args...>> AddEvent(int time,
                                       FUN_T && func,
                                       Args... args) {
    if (time < 0) {
      throw std::invalid_argument("Time must be positive");
    }
    Event<Args...> event(std::to_string(next_id_), time, std::forward<FUN_T>(func), std::forward<Args>(args)...);
    ++next_id_;
    event_queue_.add(event);
    running_events_.insert(std::stoi(event.getID()));
    return event;
  }

  /**
   * @brief Adds event to paused_events
   * @param event The event to be paused
   * @return true if successful, false if event does not exist in queue
   */
  bool PauseEvent(const Event<Args...> &event) {
    int id = std::stoi(event.getID());
    assert(((paused_events_.find(id) != paused_events_.end()) ||
            (running_events_.count(id) > 0)) &&
           "Event ID must be a managed ID.");
    if (paused_events_.count(id) > 0) {
      return true;
    } else if (running_events_.count(id) > 0) {
      paused_events_.insert({id, event});
      running_events_.erase(id);
      return true;
    }
    std::cerr << "Warning: Event ID not found in paused or running events.\n";
    return false;
  }

  /**
   * @brief Removes event to paused_events set
   * @param event The event to be resumed
   * @return true if successfully removed, false if event is not currently
   * paused
   */
  bool ResumeEvent(Event<Args...> &event) {
    int event_id = std::stoi(event.getID());
    assert(((paused_events_.find(event_id) != paused_events_.end()) ||
            running_events_.count(event_id)) &&
           "Event ID must be a managed ID.");
    if (running_events_.count(event_id)) {
      return true;
    } else if (paused_events_.find(event_id) != paused_events_.end()) {
      if (event.getTime() <= this->getTime()) {
        event_queue_.add(std::move(paused_events_.at(event_id)));
      }
      paused_events_.erase(event_id);
      running_events_.insert(event_id);
      return true;
    }
    std::cerr << "Warning: Event ID not found in paused or running events.\n";
    return false;
  }

  /**
   * @brief Adds event to repeat_events_
   * @param event The event to be repeated
   * @return true if successfully added to repeat_events, false if unsuccessful
   */
  bool RepeatEvent(const cse::Event<Args...> &event, int time_interval) {
    assert(time_interval > 0);
    if (time_interval > 0 && (paused_events_.count(std::stoi(event.getID())) +
                              running_events_.count(std::stoi(event.getID())))) {
      repeat_events_.insert({std::stoi(event.getID()), time_interval});
      return true;
    }
    return false;
  }

  /**
   * @brief Get the number of paused events
   * @return The number of paused events
   */
  size_t getNumPaused() const { return paused_events_.size(); }

  /**
   * @brief Get the number of events being managed
   * @return The number of events in event_queue_ as size_t
   */
  size_t getNumEvents() const noexcept{
    return (paused_events_.size() + running_events_.size());
  }

  /**
   * @brief Get the current internal time
   * @return The current time in seconds
   */
  double getTime() const noexcept{
    if (start_time_ == std::chrono::steady_clock::time_point{}) {
      return 0;
    }
    double nanoSeconds =
        ((std::chrono::steady_clock::now() - start_time_) + total_runtime_)
            .count();
    return nanoSeconds / NS_TO_S;
  }

  /**
   * @brief Get the set of running events
   * @return A copy of the running events set
   */
  const std::set<int>& getRunningEvents() const noexcept{ return running_events_; };

  /**
   * @brief Get the map of paused Events
   * @return A copy of the paused events map
   */
  const std::unordered_map<int, Event<Args...>>& getPausedEvents() const noexcept{
    return paused_events_;
  };
};

}  // namespace cse
