#include <vector>

#include "../../Group-07/Team07Library/Scheduler.hpp"
#include "../../third-party/Catch/single_include/catch2/catch.hpp"

TEST_CASE("SchedulerTest Construct", "[Scheduler]") {
  cse::Scheduler scheduler({0.5,0.5});

  CHECK(0 == scheduler.GetCurrProcesses());
  CHECK(true == scheduler.empty());
}

TEST_CASE("SchedulerTest Add", "[Scheduler]") {
  cse::Scheduler scheduler({1});
  scheduler.AddProcess(0,{1});
  CHECK(1 == scheduler.GetCurrProcesses());

  scheduler.AddProcess(1,{4});
  CHECK(2 == scheduler.GetCurrProcesses());

  scheduler.AddProcess(1,{2},2);
  CHECK(4 == scheduler.GetCurrProcesses());

  scheduler.AddProcess(0,{1});
  CHECK(5 == scheduler.GetCurrProcesses());
}

TEST_CASE("SchedulerTest Remove", "[Scheduler]") {
  cse::Scheduler scheduler({0.25,0.25,0.25,0.25});

  scheduler.RemoveProcess(0);
  CHECK(0 == scheduler.GetCurrProcesses());

  scheduler.AddProcess(0,{1,2,3,4});
  scheduler.AddProcess(0,{1,1,1,1});
  scheduler.AddProcess(1,{0,0,0,0},2);

  scheduler.RemoveProcess(0);
  CHECK(3 == scheduler.GetCurrProcesses());

  scheduler.RemoveProcess(1,2);
  CHECK(1 == scheduler.GetCurrProcesses());
}

TEST_CASE("SchedulerTest Empty", "[Scheduler]") {
  cse::Scheduler scheduler({1});
  CHECK(true == scheduler.empty());

  scheduler.AddProcess(0,{1});
  CHECK(false == scheduler.empty());

  scheduler.RemoveProcess(0,{1});
  CHECK(true == scheduler.empty());
}

TEST_CASE("SchedulerTest Queue", "[Scheduler]") {
  cse::Scheduler scheduler({1}), scheduler2({0.5,0.5}), scheduler3({0.25,0.25,0.25,0.25});
  CHECK(std::nullopt == scheduler.PopNextProcess());

  scheduler.AddProcess(1,{1});
  CHECK(1 == scheduler.PopNextProcess());
  CHECK(std::nullopt == scheduler.PopNextProcess());

  scheduler.AddProcess(1,{1});
  scheduler.AddProcess(2,{1.5});
  scheduler.AddProcess(3,{1.75});
  std::vector<std::optional<int>> expected = {3,2,1}, actual = {};
  while (!scheduler.empty()) {
    actual.push_back(scheduler.PopNextProcess());
  }
  CHECK(expected == actual);

  for (int i = 1; i <= 4; i++) {
    scheduler2.AddProcess(i,{i*1.1,i*1.2},i);
  }
  expected = {4, 4, 4, 4, 3, 3, 3, 2, 2, 1};
  actual = {};
  while (!scheduler2.empty()) {
    actual.push_back(scheduler2.PopNextProcess());
  }
  CHECK(expected == actual);

  for (int i = 0; i < 10; i++) {
    scheduler3.AddProcess(1,{1,1,1,1});
    scheduler3.PopNextProcess();
  }
  expected = {1, 4, 4};
  actual = {};
  scheduler3.AddProcess(4,{1,1,1,1},2);
  scheduler3.AddProcess(1,{1,2,3,0});
  while (!scheduler3.empty()) {
    actual.push_back(scheduler3.PopNextProcess());
  }

  CHECK(expected == actual);
}

TEST_CASE("SchedulerTest Override", "[Scheduler]") {
    cse::Scheduler scheduler({1});

    scheduler.OverridePriority(2);

    scheduler.AddProcess(0,{5});
    scheduler.AddProcess(1,{2},2);
    scheduler.OverridePriority(1);

    std::vector<std::optional<int>> actual={},expected={1,0,1};
    while(!scheduler.empty()) {
        actual.push_back(scheduler.PopNextProcess());
    }
    CHECK(expected == actual);

    scheduler.AddProcess(0,{1},2);
    scheduler.OverridePriority(0);
    CHECK(2 == scheduler.GetCurrProcesses());

    scheduler.RemoveProcess(0,2);
    CHECK(0 == scheduler.GetCurrProcesses());
    CHECK(std::nullopt == scheduler.PopNextProcess());
}