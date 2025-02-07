/**
 * @file Scheduler.h
 * @author Nathan Staniszewski
 *
 *
 */

#ifndef PROJECT_CSE498_SPRING2025_GROUP_07_TEAM07LIBRARY_SCHEDULER_H
#define PROJECT_CSE498_SPRING2025_GROUP_07_TEAM07LIBRARY_SCHEDULER_H

#include <map>
#include <vector>
#include <algorithm>

namespace cse
{
    class Scheduler
    {
    private:

        std::map<int, int> scheduleMap;
        std::map<int, double> timesCalled;
        std::vector<int> currIds;
        int currProcesses = 0;
        double totalProcesses = 0;
        bool needUpdate = true;

    public:
        Scheduler() = default;
        ~Scheduler() = default;

        void AddProcess(int id);
        void RemoveProcess(int id);
        void UpdateProcessCount(int id, int x);
        int GetNextProcess();
        bool empty() { return currProcesses == 0; }
        int GetUniqueProcesses() const { return currIds.size(); }
        int GetCurrProcesses() const { return currProcesses; }
        int GetTotalProcesses() const { return totalProcesses; }

    };
}
#endif //PROJECT_CSE498_SPRING2025_GROUP_07_TEAM07LIBRARY_SCHEDULER_H
