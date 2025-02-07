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
  ~EventManager(){
    StopQueue();
  };
  /********Member functions that handles event adjustments******/
  ///Adds event_id to a set of paused ids.
  bool PauseEvent(int event_id);
  ///Removes event_id from a paused set.
  bool ResumeEvent(int event_id);
  ///Adds event to the queue.
  bool AddEvent(cse::Event& event);


  /********Member functions that handles queue adjustments***********/
  void StopQueue();
  void StartQueue();
  void RestartQueue();

};


}

