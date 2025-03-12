/**
 * @file TestEventManager.cpp
 * @author Grace Fitzgerald
 */

#include "../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../Group-09/EventManager/EventManager.cpp"
#include "../../Group-09/EventQueue/EventQueue.cpp"
#include <cmath>

using namespace cse;


TEST_CASE("Thread Synchronization", "[EventManager]")
{
  EventManager eM;

  Event e1 = eM.AddEvent(1, "Event 1").value();
  eM.AddEvent(3, "Event 2");
  eM.AddEvent(9, "Event 3");

  CHECK(eM.getTime() == 0);
  auto nextTime = std::chrono::steady_clock::now();
  for(int i = 1; i < 8; i++){
    eM.StartQueue();
    nextTime += std::chrono::seconds(1);
    std::this_thread::sleep_until(nextTime);
    //Tolerance to account in thread scheduling and sleep function variability
    CHECK( std::abs(eM.getTime() - i) <= 1);
    eM.StopQueue();
  }

  eM.StartQueue();
  nextTime += std::chrono::seconds(1);
  std::this_thread::sleep_until(nextTime);
  //Tolerance to account in thread scheduling and sleep function variability
  CHECK( std::abs(eM.getTime() - 8) <= 1);
  nextTime += std::chrono::seconds(1);
  std::this_thread::sleep_until(nextTime);
  //Tolerance to account in thread scheduling and sleep function variability
  CHECK( std::abs(eM.getTime() - 9) <= 1);
  eM.StopQueue();
}


TEST_CASE("Add event to EventManager", "[EventManager]")
{
  EventManager eM;

  Event e1(0, 1, "Event 1");
  CHECK(eM.AddEvent(1, "Event 1").value() == e1);
  std::set<int> runningEvents;
  runningEvents.insert(0);
  CHECK(eM.getRunningEvents() == runningEvents);
  CHECK(eM.getNumEvents() == 1);

  Event e2(1, 3, "Event 2!");

  CHECK(eM.AddEvent(3, "Event 2!").value() == e2);
  runningEvents.insert(1);
  CHECK(eM.getRunningEvents() == runningEvents);
  CHECK(eM.getNumEvents() == 2);

  Event e3(2, 3, "Event 3\n");
  CHECK(eM.AddEvent(3, "Event 3\n").value() == e3);
  runningEvents.insert(2);
  CHECK(eM.getRunningEvents() == runningEvents);
  CHECK(eM.getNumEvents() == 3);
}

TEST_CASE("Pause EventManager event - valid", "[EventManager]")
{
  EventManager eM;

  Event e1 = eM.AddEvent(1, "Event 1").value();
  Event e2 = eM.AddEvent(3, "Event 2!").value();
  Event e3 = eM.AddEvent(3, "Event 3\n").value();
  Event e4 = eM.AddEvent(3, "_Event_4_").value();
  std::set<int> runningEvents;
  runningEvents.insert(0);
  runningEvents.insert(1);
  runningEvents.insert(2);
  runningEvents.insert(3);
  CHECK(eM.getRunningEvents() == runningEvents);
  REQUIRE(eM.getNumEvents() == 4);

  std::unordered_map<int, Event> pausedEvents;
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
  EventManager eM;

  Event e1 = eM.AddEvent(1, "E0").value();
  Event e2 = eM.AddEvent(3, "E1").value();
  Event e3 = eM.AddEvent(1202, "E2").value();
  Event e4 = eM.AddEvent(3, "E3").value();
  Event e5 = eM.AddEvent(5, "E4").value();

  std::set<int> runningEvents;
  runningEvents.insert(0);
  runningEvents.insert(1);
  runningEvents.insert(2);
  runningEvents.insert(3);
  runningEvents.insert(4);
  std::unordered_map<int, Event> pausedEvents;

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
  EventManager eM;

  eM.AddEvent(1, "Event 1");
  eM.AddEvent(3, "Event 2");
  eM.AddEvent(5, "Event 3");

  REQUIRE(eM.getNumEvents() == 3);
  eM.StartQueue();
  std::this_thread::sleep_for(std::chrono::seconds(2));
  CHECK(eM.getNumEvents() == 2);
  std::this_thread::sleep_for(std::chrono::seconds(2));
  CHECK(eM.getNumEvents() == 1);
  eM.AddEvent(7, "Event 4");
  CHECK(eM.getNumEvents() == 2);
  std::this_thread::sleep_for(std::chrono::seconds(2));
  CHECK(eM.getNumEvents() == 1);
  std::this_thread::sleep_for(std::chrono::seconds(0));
  CHECK(eM.getNumEvents() == 1);
}

TEST_CASE("Trigger Events with Repeat Events", "[EventManager]")
{
  EventManager eM;
  std::optional<Event> e1 = eM.AddEvent(1, "Event 1");
  CHECK(e1.has_value());
  std::optional<Event> e2 = eM.AddEvent(3, "Event 2");
  CHECK(e2.has_value());
  std::optional<Event> e3 = eM.AddEvent(5, "Event 3");
  CHECK(e3.has_value());
  Event e4(4, 10, "Event 4");
  CHECK(eM.RepeatEvent(e1.value(), 4) == true);
  CHECK(eM.RepeatEvent(e4, 10) == false);

  std::cout << "Beginning Trigger Events - Repeat Events Test Case" << std::endl;
  REQUIRE(eM.getNumEvents() == 3);
  eM.StartQueue();
  std::this_thread::sleep_for(std::chrono::seconds(2)); //e1 triggered and readd to queue
  CHECK(eM.getNumEvents() == 3);
  std::this_thread::sleep_for(std::chrono::seconds(2)); //e2 triggered
  CHECK(eM.getNumEvents() == 2);
  std::this_thread::sleep_for(std::chrono::seconds(2)); //e1 and e3 triggered, readd e1
  CHECK(eM.getNumEvents() == 1);
}

TEST_CASE("Trigger Events with Pause/Resume", "[EventManager]")
{
  EventManager eM;

  Event e1 = eM.AddEvent(1, "Event 1").value();
  eM.AddEvent(3, "Event 2");
  eM.AddEvent(5, "Event 3");

  eM.PauseEvent(e1);
  std::cout << "Beginning Trigger Events - Pause/Resume" << std::endl;
  REQUIRE(eM.getNumEvents() == 3);
  eM.StartQueue();
  std::this_thread::sleep_for(std::chrono::seconds(2));
  CHECK(eM.getNumEvents() == 3);
  std::this_thread::sleep_for(std::chrono::seconds(2));
  CHECK(eM.getNumEvents() == 2);
  eM.ResumeEvent(e1);
  std::this_thread::sleep_for(std::chrono::seconds(4));
  CHECK(eM.getNumEvents() == 0);
}




