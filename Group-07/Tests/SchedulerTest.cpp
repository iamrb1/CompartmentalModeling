#include <vector>
#include <Scheduler.hpp>
#include "gtest/gtest.h"

TEST(SchedulerTest,Construct)
{
    Scheduler scheduler;
}

TEST(SchedulerTest,Add)
{
    Scheduler scheduler;
    scheduler.AddProcess(0);
    ASSERT_EQ(1,scheduler.GetCurrProcesses());
    ASSERT_EQ(1,scheduler.GetTotalProcesses());
    ASSERT_EQ(1,scheduler.GetUniqueProcesses());

    scheduler.AddProcess(1);
    ASSERT_EQ(2,scheduler.GetCurrProcesses());
    ASSERT_EQ(2,scheduler.GetTotalProcesses());
    ASSERT_EQ(2,scheduler.GetUniqueProcesses());

    scheduler.AddProcess(1);
    ASSERT_EQ(3,scheduler.GetCurrProcesses());
    ASSERT_EQ(3,scheduler.GetTotalProcesses());
    ASSERT_EQ(2,scheduler.GetUniqueProcesses());
}

TEST(SchedulerTest,Remove)
{
    Scheduler scheduler;

    scheduler.RemoveProcess(0);
    ASSERT_EQ(0,scheduler.GetCurrProcesses());
    ASSERT_EQ(0,scheduler.GetTotalProcesses());
    ASSERT_EQ(0,scheduler.GetUniqueProcesses());

    scheduler.AddProcess(0);
    scheduler.AddProcess(0);
    scheduler.AddProcess(1);

    scheduler.RemoveProcess(0);
    ASSERT_EQ(2,scheduler.GetCurrProcesses());
    ASSERT_EQ(2,scheduler.GetTotalProcesses());
    ASSERT_EQ(2,scheduler.GetUniqueProcesses());
}

TEST(SchedulerTest,Update)
{
    Scheduler scheduler;
    scheduler.UpdateProcessCount(1,12);
    ASSERT_EQ(12,scheduler.GetCurrProcesses());
    ASSERT_EQ(12,scheduler.GetTotalProcesses());
    ASSERT_EQ(1,scheduler.GetUniqueProcesses());

    scheduler.UpdateProcessCount(1,-1);
    ASSERT_EQ(12,scheduler.GetCurrProcesses());
    ASSERT_EQ(12,scheduler.GetTotalProcesses());
    ASSERT_EQ(1,scheduler.GetUniqueProcesses());

    scheduler.AddProcess(0);
    scheduler.UpdateProcessCount(0,12);
    ASSERT_EQ(24,scheduler.GetCurrProcesses());
    ASSERT_EQ(24,scheduler.GetTotalProcesses());
    ASSERT_EQ(2,scheduler.GetUniqueProcesses());

    scheduler.UpdateProcessCount(1,0);
    ASSERT_EQ(12,scheduler.GetCurrProcesses());
    ASSERT_EQ(12,scheduler.GetTotalProcesses());
    ASSERT_EQ(1,scheduler.GetUniqueProcesses());
}

TEST(SchedulerTest,Queue)
{
    Scheduler scheduler,scheduler2,scheduler3;
    ASSERT_EQ(-1,scheduler.GetNextProcess());

    scheduler.AddProcess(1);
    ASSERT_EQ(1,scheduler.GetNextProcess());
    ASSERT_EQ(-1,scheduler.GetNextProcess());

    scheduler.AddProcess(2);
    scheduler.AddProcess(1);
    scheduler.AddProcess(3);
    std::vector<int> expected={1,2,3},actual={};
    while(!scheduler.empty())
    {
        actual.push_back(scheduler.GetNextProcess());
    }
    ASSERT_EQ(expected,actual);


    for(int i=1;i<=4;i++)
    {
        for(int j=0;j<i;j++)
        {
            scheduler2.AddProcess(i);
        }
    }
    expected={4,4,4,4,3,3,3,2,2,1};
    actual={};
    while(!scheduler2.empty())
    {
        actual.push_back(scheduler2.GetNextProcess());
    }
    ASSERT_EQ(expected,actual);


    for(int i=0;i<10;i++)
    {
        scheduler3.AddProcess(1);
        scheduler3.GetNextProcess();
    }
    expected={1,4,4};
    actual={};
    scheduler3.AddProcess(4);
    scheduler3.AddProcess(1);
    scheduler3.AddProcess(4);
    while(!scheduler3.empty())
    {
        actual.push_back(scheduler3.GetNextProcess());
    }

    ASSERT_EQ(expected,actual);
}