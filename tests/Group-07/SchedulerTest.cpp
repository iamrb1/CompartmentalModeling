#include <vector>

#include "../../Group-07/Team07Library/Scheduler.hpp"
#include "../../third-party/Catch/single_include/catch2/catch.hpp"

TEST_CASE("SchedulerTest Construct", "[Scheduler]") {
  cse::Scheduler<int,2> scheduler({0.5,0.5});

  CHECK(0 == scheduler.GetCurrProcesses());
  CHECK(true == scheduler.empty());
}

TEST_CASE("SchedulerTest Add", "[Scheduler]") {
  cse::Scheduler scheduler({1});
  scheduler.AddProcess(0,{1});
  CHECK(1 == scheduler.GetCurrProcesses());

  scheduler.AddProcess(1,{4});
  CHECK(2 == scheduler.GetCurrProcesses());

  scheduler.AddProcess(1,{2});
  CHECK(2 == scheduler.GetCurrProcesses());

  scheduler.AddProcess(2,{1});
  CHECK(3 == scheduler.GetCurrProcesses());
}

TEST_CASE("SchedulerTest Remove", "[Scheduler]") {
  cse::Scheduler<int,4> scheduler({0.25,0.25,0.25,0.25});

  scheduler.RemoveProcess(0);
  CHECK(0 == scheduler.GetCurrProcesses());

  scheduler.AddProcess(0,{1,2,3,4});
  scheduler.AddProcess(0,{1,1,1,1});
  scheduler.AddProcess(1,{0,0,0,0});

  scheduler.RemoveProcess(0);
  CHECK(1 == scheduler.GetCurrProcesses());

  scheduler.RemoveProcess(1);
  CHECK(0 == scheduler.GetCurrProcesses());
}

TEST_CASE("SchedulerTest Empty", "[Scheduler]") {
  cse::Scheduler scheduler({1});
  CHECK(true == scheduler.empty());

  scheduler.AddProcess(0,{1});
  CHECK(false == scheduler.empty());

  scheduler.RemoveProcess(0);
  CHECK(true == scheduler.empty());
}

TEST_CASE("SchedulerTest Queue", "[Scheduler]") {
  cse::Scheduler scheduler({1});
  cse::Scheduler<int,2> scheduler2({0.5,0.5});
  cse::Scheduler<int,4> scheduler3({0.25,0.25,0.25,0.25});
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
    scheduler2.AddProcess(i,{i*1.1,i*1.2});
  }
  expected = {4, 3, 2, 1};
  actual = {};
  while (!scheduler2.empty()) {
    actual.push_back(scheduler2.PopNextProcess());
  }
  CHECK(expected == actual);

  expected = {1, 4};
  actual = {};
  scheduler3.AddProcess(4,{1,1,1,1});
  scheduler3.AddProcess(4,{4,4,4,4});
  scheduler3.AddProcess(1,{1,2,3,0});
  while (!scheduler3.empty()) {
    actual.push_back(scheduler3.PopNextProcess());
  }

  CHECK(expected == actual);
}

TEST_CASE("SchedulerTest Override", "[Scheduler]") {
    cse::Scheduler scheduler({1});

    scheduler.OverridePriority(2);
    CHECK(std::nullopt == scheduler.PopNextProcess());

    scheduler.AddProcess(0,{5});
    scheduler.AddProcess(1,{2});
    scheduler.OverridePriority(1);

    std::vector<std::optional<int>> actual={},expected={1,0};
    while(!scheduler.empty()) {
        actual.push_back(scheduler.PopNextProcess());
    }
    CHECK(expected == actual);

    scheduler.AddProcess(0,{1});
    scheduler.OverridePriority(0);
    scheduler.AddProcess(0,{1});

    CHECK(1 == scheduler.GetCurrProcesses());

    scheduler.RemoveProcess(0);
    CHECK(0 == scheduler.GetCurrProcesses());
    CHECK(std::nullopt == scheduler.PopNextProcess());
}

TEST_CASE("SchedulerTest GetProcessPriority", "[Scheduler]") {
    cse::Scheduler scheduler({1});

    scheduler.AddProcess(0,{1});
    CHECK(1 == scheduler.GetProcessPriority(0));

    scheduler.RemoveProcess(0);
    CHECK(std::nullopt == scheduler.GetProcessPriority(0));

    scheduler.AddProcess(0,{1});
    scheduler.OverridePriority(0);
    CHECK(1 == scheduler.GetProcessPriority(0));
}

TEST_CASE("SchedulerTest GetProcessWeights", "[Scheduler]") {
    cse::Scheduler scheduler({1});

    std::optional<std::array<double,1>> expected=std::array<double,1>{1.0};
    scheduler.AddProcess(0,{1});
    CHECK(expected == scheduler.GetProcessWeights(0));

    scheduler.RemoveProcess(0);
    CHECK(std::nullopt == scheduler.GetProcessWeights(0));

    scheduler.AddProcess(0,{1});
    scheduler.OverridePriority(0);
    CHECK(expected == scheduler.GetProcessWeights(0));
}

TEST_CASE("SchedulerTest UpdateProcessPriority", "[Scheduler]") {
    cse::Scheduler<int,4> scheduler({0.25,0.25,0.25,0.25});

    scheduler.AddProcess(0,{1,1,1,1});
    scheduler.AddProcess(1,{2,2,2,2});
    scheduler.UpdateProcessPriority(0,{25,25,25,25});
    CHECK(0 == scheduler.PopNextProcess());
    CHECK(1 == scheduler.PopNextProcess());

    scheduler.AddProcess(0,{1,1,1,1});
    scheduler.AddProcess(1,{2,2,2,2});
    scheduler.UpdateProcessPriority(1,{0,0,0,0});
    CHECK(0 == scheduler.PopNextProcess());
    CHECK(1 == scheduler.PopNextProcess());

    scheduler.AddProcess(2,{1,1,1,1});
    scheduler.AddProcess(0,{2,2,2,2});
    scheduler.AddProcess(1,{3,3,3,3});
    scheduler.OverridePriority(2);
    scheduler.UpdateProcessPriority(2,{0,0,0,0});
    scheduler.UpdateProcessPriority(0,{25,25,25,25});
    CHECK(2 == scheduler.PopNextProcess());
    CHECK(0 == scheduler.PopNextProcess());
    CHECK(1 == scheduler.PopNextProcess());

    std::optional<std::array<double,4>> expected=std::array<double,4>{4.0,4.0,4.0,4.0};
    scheduler.AddProcess(1,{0,1,2,3});
    scheduler.AddProcess(0,{1,1,1,1});
    scheduler.UpdateProcessPriority(0,{4,4,4,4});
    CHECK(expected == scheduler.GetProcessWeights(0));
    CHECK(4 == scheduler.GetProcessPriority(0));
}

TEST_CASE("SchedulerTest TemplateTest", "[Scheduler]") {
    cse::Scheduler<std::string,1> scheduler({1});

    scheduler.AddProcess("a",{2});
    CHECK(false == scheduler.empty());
    CHECK(1 == scheduler.GetCurrProcesses());

    scheduler.AddProcess("b",{3});
    CHECK("b" == scheduler.PopNextProcess());

    scheduler.RemoveProcess("a");
    CHECK(true == scheduler.empty());

    scheduler.AddProcess("c",{1});
    scheduler.UpdateProcessPriority("c",{2});
    scheduler.UpdateSchedulerWeights({2});
    scheduler.OverridePriority("b");
    scheduler.OverridePriority("c");
    CHECK(4 == scheduler.GetProcessPriority("c"));

    std::optional<std::array<double,1>> expected=std::array<double,1>{2.0};
    CHECK(expected == scheduler.GetProcessWeights("c"));
    CHECK("c" == scheduler.PopNextProcess());
}