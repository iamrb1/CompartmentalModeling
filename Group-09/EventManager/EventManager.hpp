/**
 * @file EventManager.h
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
#include "../Event/Event.hpp"
#include "../EventQueue/EventQueue.hpp"

namespace cse {


class EventManager {
 private:
  EventQueue event_queue_;
  int clock_time_ = 0; //Temporary to keep track of time
  std::thread clock_thread_;
  std::atomic<bool> running_{false};
  std::set<int> paused_events_;
  ///Handles running clock.
  void AdvanceTime();
  ///Checks time and triggers all events whose trigger time is <= clock_time
  void TriggerEvents();

 public:
  EventManager() = default;
  ~EventManager(){ StopQueue();};
  bool PauseEvent(int event_id);
  bool ResumeEvent(int event_id);
  bool AddEvent(cse::Event& event);
  void StopQueue();
  void StartQueue();
  void RestartQueue();

};


}

