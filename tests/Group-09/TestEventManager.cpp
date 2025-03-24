/**
 * @file TestEventManager.cpp
 * @author Grace Fitzgerald
 */

#include "../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../Group-09/EventQueue/EventQueue.hpp"
#include "../../Group-09/EventManager/EventManager.hpp"
#include <cmath>

using namespace cse;

void printEvent(int id){
  std::cout << "Triggering event: " << id << std::endl;
}

TEST_CASE("Check empty EventManager", "[EventManager]")
{
  EventManager<int> eventManager;
  CHECK(eventManager.getNumEvents() == 0);
  CHECK(eventManager.getNumPaused() == 0);
  CHECK(eventManager.getTime() == 0);
}

TEST_CASE("Check getTime in EventManager", "[EventManager]")
{
  EventManager<int> eM;
  Event<int> e1(0, 10, printEvent);
  CHECK(eM.AddEvent(1, printEvent).value() == e1);
  std::set<int> runningEvents;
  runningEvents.insert(0);
  CHECK(eM.getRunningEvents() == runningEvents);
  CHECK(eM.getNumEvents() == 1);
  eM.StartQueue();
  double totalTimeElapsed = 0;
  for(int i = 1; i < 22; i++){
    auto before = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    auto after = std::chrono::steady_clock::now();
    totalTimeElapsed += std::chrono::duration<double>(after - before).count();
    CHECK(std::abs(eM.getTime() - totalTimeElapsed) < .01*i); // Account for sleep_for imprecision
    eM.TriggerEvents();
  }


}

TEST_CASE("Add event to EventManager", "[EventManager]")
{
  EventManager<int> eM;

  Event<int> e1(0, 1, printEvent);
  CHECK(eM.AddEvent(1, printEvent).value() == e1);
  std::set<int> runningEvents;
  runningEvents.insert(0);
  CHECK(eM.getRunningEvents() == runningEvents);
  CHECK(eM.getNumEvents() == 1);

  Event<int> e2(1, 3, printEvent);

  CHECK(eM.AddEvent(3, printEvent).value() == e2);
  runningEvents.insert(1);
  CHECK(eM.getRunningEvents() == runningEvents);
  CHECK(eM.getNumEvents() == 2);

  Event<int> e3(2, 3, printEvent);
  CHECK(eM.AddEvent(3, printEvent).value() == e3);
  runningEvents.insert(2);
  CHECK(eM.getRunningEvents() == runningEvents);
  CHECK(eM.getNumEvents() == 3);
}

TEST_CASE("Pause EventManager event - valid", "[EventManager]")
{
  EventManager<int> eM;

  Event<int> e1 = eM.AddEvent(1, printEvent).value();
  Event<int> e2 = eM.AddEvent(3, printEvent).value();
  Event<int> e3 = eM.AddEvent(3, printEvent).value();
  Event<int> e4 = eM.AddEvent(3, printEvent).value();
  std::set<int> runningEvents;
  runningEvents.insert(0);
  runningEvents.insert(1);
  runningEvents.insert(2);
  runningEvents.insert(3);
  CHECK(eM.getRunningEvents() == runningEvents);
  REQUIRE(eM.getNumEvents() == 4);

  std::unordered_map<int, Event<int>> pausedEvents;
  CHECK(eM.PauseEvent(e1) == true);
  pausedEvents.insert({e1.getID(), e1});
  CHECK(eM.getPausedEvents() == pausedEvents);
  runningEvents.erase(e1.getID());
  CHECK(eM.getRunningEvents() == runningEvents);
  CHECK(eM.getNumPaused() == 1);

  CHECK(eM.PauseEvent(e3) == true);
  pausedEvents.insert({e3.getID(), e3});
  CHECK(eM.getPausedEvents() == pausedEvents);
  runningEvents.erase(e3.getID());
  CHECK(eM.getRunningEvents() == runningEvents);
  CHECK(eM.getNumPaused() == 2);


  CHECK(eM.PauseEvent(e3) == true);
  CHECK(eM.getPausedEvents() == pausedEvents);
  runningEvents.erase(e3.getID());
  CHECK(eM.getRunningEvents() == runningEvents);
  CHECK(eM.getNumPaused() == 2);

}

TEST_CASE("Resume EventManager event", "[EventManager]")
{
  EventManager<int> eM;

  Event<int> e1 = eM.AddEvent(1, printEvent).value();
  Event<int> e2 = eM.AddEvent(3, printEvent).value();
  Event<int> e3 = eM.AddEvent(1202, printEvent).value();
  Event<int> e4 = eM.AddEvent(3, printEvent).value();
  Event<int> e5 = eM.AddEvent(5, printEvent).value();

  std::set<int> runningEvents;
  runningEvents.insert(0);
  runningEvents.insert(1);
  runningEvents.insert(2);
  runningEvents.insert(3);
  runningEvents.insert(4);
  std::unordered_map<int, Event<int>> pausedEvents;

  REQUIRE(eM.getNumEvents() == 5);
  CHECK(eM.getRunningEvents() == runningEvents);
  eM.PauseEvent(e1);
  runningEvents.erase(e1.getID());
  CHECK(eM.getRunningEvents() == runningEvents);
  pausedEvents.insert({e1.getID(), e1});
  CHECK(eM.getPausedEvents() == pausedEvents);


  eM.PauseEvent(e3);
  runningEvents.erase(e3.getID());
  CHECK(eM.getRunningEvents() == runningEvents);
  pausedEvents.insert({e3.getID(), e3});
  CHECK(eM.getPausedEvents() == pausedEvents);

  CHECK(eM.ResumeEvent(e1) == true);
  CHECK(eM.getNumPaused() == 1);
  runningEvents.insert(e1.getID());
  CHECK(eM.getRunningEvents() == runningEvents);
  pausedEvents.erase(e1.getID());
  CHECK(eM.getPausedEvents() == pausedEvents);

  CHECK(eM.ResumeEvent(e3) == true);


  CHECK(eM.ResumeEvent(e3) == true);
  CHECK(eM.getNumPaused() == 0);


  CHECK(eM.ResumeEvent(e2) == true);
  CHECK(eM.getNumPaused() == 0);

}

TEST_CASE("Trigger events", "[EventManager]")
{
  EventManager<int> eM;

  eM.AddEvent(1, printEvent);
  eM.AddEvent(3, printEvent);
  eM.AddEvent(5, printEvent);

  REQUIRE(eM.getNumEvents() == 3);
  eM.StartQueue();
  std::this_thread::sleep_for(std::chrono::seconds(2));
  eM.TriggerEvents();
  CHECK(eM.getNumEvents() == 2);
  std::this_thread::sleep_for(std::chrono::seconds(2));
  eM.TriggerEvents();
  CHECK(eM.getNumEvents() == 1);
  eM.AddEvent(7, printEvent);
  CHECK(eM.getNumEvents() == 2);
  std::this_thread::sleep_for(std::chrono::seconds(2));
  eM.TriggerEvents();
  CHECK(eM.getNumEvents() == 1);
  std::this_thread::sleep_for(std::chrono::seconds(0));
  eM.TriggerEvents();
  CHECK(eM.getNumEvents() == 1);
}

TEST_CASE("Trigger Events with Repeat Events", "[EventManager]")
{
  EventManager<int> eM;
  std::optional<Event<int>> e1 = eM.AddEvent(1, printEvent);
  CHECK(e1.has_value());
  std::optional<Event<int>> e2 = eM.AddEvent(3, printEvent);
  CHECK(e2.has_value());
  std::optional<Event<int>> e3 = eM.AddEvent(5, printEvent);
  CHECK(e3.has_value());
  Event<int> e4(4, 10, printEvent);
  CHECK(eM.RepeatEvent(e1.value(), 4) == true);
  CHECK(eM.RepeatEvent(e4, 10) == false);

  std::cout << "Beginning Trigger Events - Repeat Events Test Case" << std::endl;
  REQUIRE(eM.getNumEvents() == 3);
  eM.StartQueue();
  std::this_thread::sleep_for(std::chrono::seconds(2)); //e1 triggered and readd to queue
  eM.TriggerEvents();
  CHECK(eM.getNumEvents() == 3);
  std::this_thread::sleep_for(std::chrono::seconds(2)); //e2 triggered
  eM.TriggerEvents();
  CHECK(eM.getNumEvents() == 2);
  std::this_thread::sleep_for(std::chrono::seconds(2)); //e1 and e3 triggered, readd e1
  eM.TriggerEvents();
  CHECK(eM.getNumEvents() == 1);
  eM.StopQueue();

}

TEST_CASE("Trigger Events with Pause/Resume", "[EventManager]")
{
  EventManager<int> eM;

  Event<int> e1 = eM.AddEvent(1, printEvent).value();
  eM.AddEvent(3, printEvent);
  eM.AddEvent(5, printEvent);

  eM.PauseEvent(e1);
  std::cout << "Beginning Trigger Events - Pause/Resume" << std::endl;
  REQUIRE(eM.getNumEvents() == 3);
  eM.StartQueue();
  std::this_thread::sleep_for(std::chrono::seconds(2));
  eM.TriggerEvents();
  CHECK(eM.getNumEvents() == 3);
  std::this_thread::sleep_for(std::chrono::seconds(2));
  eM.TriggerEvents();
  CHECK(eM.getNumEvents() == 2);
  eM.ResumeEvent(e1);
  std::this_thread::sleep_for(std::chrono::seconds(4));
  eM.TriggerEvents();
  CHECK(eM.getNumEvents() == 0);
}




