/**
 * @file EventManager.cpp
 * @author Grace Fitzgerald
 */

#include "EventManager.hpp"

namespace cse {

void EventManager::AdvanceTime(){
  while(running_){
    std::this_thread::sleep_for(std::chrono::seconds(1));
    clock_time_++;
    std::cout << "CLOCK TIME: " << clock_time_ << std::endl;
    TriggerEvents();
  }
}

void EventManager::TriggerEvents(){
  while((event_queue_.size() > 0)&& !paused_events_.contains(event_queue_.peek().getID()) && event_queue_.peek().getTime() <= clock_time_){
    std::cout << event_queue_.peek().getData() << "\n"; //Placeholder for handling events
    event_queue_.pop();
  }
  if (event_queue_.size() == 0){
    running_ = false;
  }
}


bool EventManager::PauseEvent(int event_id){
  if(paused_events_.contains(event_id)){
    return false; //Event is already paused
  }
  paused_events_.insert(event_id);
  return true;
}

bool EventManager::ResumeEvent(int event_id){
  if(paused_events_.contains(event_id)){
    paused_events_.erase(event_id);
  }
  return false; //Event is not paused

}

bool EventManager::AddEvent(Event& event){
  event_queue_.add(event);
  return true;
}

void EventManager::StopQueue(){
  running_ = false;
  if(clock_thread_.joinable()){
    clock_thread_.join();
  }
}

void EventManager::StartQueue(){
  if(running_){ //Queue is already running
    return;
  }
  clock_time_ = 0;
  running_ = true;
  clock_thread_ = std::thread(&EventManager::AdvanceTime, this); //Create thread that executes AdvanceTime
}

void EventManager::RestartQueue(){
  StopQueue();
  clock_time_ = 0;
  StartQueue();
}

}