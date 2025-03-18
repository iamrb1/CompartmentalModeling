/**
 * @file EventManager.hpp
 * @author Grace Fitzgerald
 *
 *
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
 */

#pragma once

#include <chrono>
#include <thread>
#include <atomic>
#include <iostream>
#include <utility>
#include <set>
#include <cassert>
#include <map>
#include "../Event/Event.hpp"
#include "../EventQueue/EventQueue.hpp"

namespace cse {

class EventManager {
 private:
  EventQueue event_queue_;
  // Tracks if queue is currently running
  bool running_{0};
  //The ID for the next event to be created
  int next_id_{0};
  // Time when queue was last started
  std::chrono::steady_clock::time_point start_time_;
  // Total time queue has been running in seconds
  std::chrono::duration<double> total_runtime_{0};
  //Set of event ids that are not paused
  std::set<int> running_events_;
  //Map of paused events <id, event>
  std::unordered_map<int, Event> paused_events_;
  //Map of events to repeat <id, time interval>
  std::unordered_map<int, int> repeat_events_;

 public:
  EventManager() = default;
  ~EventManager() { StopQueue(); };
  bool PauseEvent(Event &event);
  bool ResumeEvent(Event &event_id);
  std::optional<Event> AddEvent(int time, std::string data);
  bool RepeatEvent(cse::Event &event, int time_interval);
  void TriggerEvents();
  void StopQueue();
  void StartQueue();
  void RestartQueue();

  /**
   * @brief Get the number of paused events
   * @return The number of paused events
   */
  size_t getNumPaused() const { return paused_events_.size(); }

  /**
   * @brief Get the number of events being managed
   * @return The number of events in event_queue_ as size_t
   */
  size_t getNumEvents() const { return (paused_events_.size() + running_events_.size()); }

  /**
   * @brief Get the current internal time
   * @return The current time in seconds
   */
  double getTime(){
    if(start_time_ == std::chrono::steady_clock::time_point{}){
      return 0;
    }
    double nanoSeconds = ((std::chrono::steady_clock::now() - start_time_) + total_runtime_).count();
    return nanoSeconds/std::pow(10, 9);
  }

  /**
   * @brief Get the set of running events
   * @return A copy of the running events set
   */
  std::set<int> getRunningEvents() const {return running_events_;};

  /**
   * @brief Get the map of paused Events
   * @return A copy of the paused events map
   */
  std::unordered_map<int, Event> getPausedEvents() const {return paused_events_;};

};

}

