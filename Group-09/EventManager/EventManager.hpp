/**
 * @file EventManager.hpp
 * @author Grace Fitzgerald
 *
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
#include <mutex>
#include "../Event/Event.hpp"
#include "../EventQueue/EventQueue.hpp"

namespace cse {

class EventManager {
 private:
  EventQueue event_queue_;
  std::atomic<int> clock_time_{0};
  //The ID for the next event to be created
  int next_id_{0};
  //Flag indicating if the clock_thread is running
  std::atomic<bool> running_{false};
  //Thread to keep time and trigger events whose time has been reached
  std::thread clock_thread_;
  //Set of event ids that are not paused
  std::set<int> running_events_;
  //Map of paused events <id, event>
  std::unordered_map<int, Event> paused_events_;
  //Map of events to repeat <id, time interval>
  std::unordered_map<int, int> repeat_events_;
  void AdvanceTime();
  void TriggerEvents();

 public:
  EventManager() = default;
  ~EventManager() { StopQueue(); };
  bool PauseEvent(Event &event);
  bool ResumeEvent(Event &event_id);
  std::optional<Event> AddEvent(int time, std::string data);
  bool RepeatEvent(cse::Event &event, int time_interval);
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
  int getTime(){return clock_time_;}

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

