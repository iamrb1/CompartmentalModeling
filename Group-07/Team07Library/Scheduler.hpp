/**
 * @file Scheduler.h
 * @author Nathan Staniszewski
 *
 *
 */

#ifndef PROJECT_CSE498_SPRING2025_GROUP_07_TEAM07LIBRARY_SCHEDULER_H
#define PROJECT_CSE498_SPRING2025_GROUP_07_TEAM07LIBRARY_SCHEDULER_H

#include <algorithm>
#include <map>
#include <vector>

namespace cse {
/*
 * Priority queue to schedule processes to run based on how often each process
 * is used
 */
class Scheduler {
 private:
  /// Maps each process ID to the number of times that the process is currently
  /// in the scheduler
  std::map<int, int> processCount;

  /// Maps each process ID to the number of times that process has been added to
  /// the scheduler
  std::map<int, int> timesCalled;

  /// List holding the IDs of all the items added to the scheduler
  std::vector<int> currIds;

  /// The current number of processes that are in the scheduler
  int currProcesses = 0;

  /// The total number of processes that have been called to the scheduler
  int totalProcesses = 0;

  /// Indicator if we need to sort the scheduler again
  bool needUpdate = true;

 public:
  /**
   * Default Constructor
   */
  Scheduler() = default;

  /**
   * Default Destructor
   */
  ~Scheduler() = default;

  /**
   * Adds a process to the Scheduler
   * @param id ID of the process we are adding to the scheduler
   */
  void AddProcess(const int id) {
    processCount[id]++;
    timesCalled[id]++;
    if (processCount[id] == 1) {
      currIds.push_back(id);
    }
    totalProcesses++;
    currProcesses++;
    needUpdate = true;
  }

  /**
   * Removes the process of a given ID from the scheduler
   * @param id ID of the process we are removing from the scheduler
   */
  void RemoveProcess(const int id) {
    if (processCount.find(id) != processCount.end() && processCount[id] > 0) {
      processCount[id]--;
      timesCalled[id]--;
      totalProcesses--;
      currProcesses--;
      if (processCount[id] == 0) {
        currIds.erase(std::find(currIds.begin(), currIds.end(), id));
      }
    }
    needUpdate = true;
  }

  /**
   * Updates the number of times a process ID occurs in the scheduler
   * @param id ID of the process we are updating the process count for
   * @param x The new process count
   */
  void SetProcessCount(const int id, const int x) {
    if (x < 0) { // Check we are given a valid count for the process
      return;
    }
    if (processCount.find(id) != processCount.end()) {
      totalProcesses -= processCount[id];
      currProcesses -= processCount[id];
      timesCalled[id] -= processCount[id];
      if (x == 0 && processCount[id] != 0) {
        currIds.erase(std::find(currIds.begin(), currIds.end(), id));
      }
    } else if (x != 0) {
      currIds.push_back(id);
    }
    processCount[id] = x;
    timesCalled[id] += x;
    totalProcesses += x;
    currProcesses += x;
    needUpdate = true;
  }

  /**
   * Gets the ID for the highest priority process according to the scheduler,
   * and removes it from the scheduler
   * @return ID of the highest priority process
   */
  std::optional<int> PopNextProcess() {
    if (currProcesses == 0) { // Make sure the Scheduler isn't empty before popping a process from it
      return std::nullopt;
    }
    if (needUpdate) {
      std::stable_sort(currIds.begin(),currIds.end());
    }
    int outID = currIds[0];
    processCount[outID]--;
    currProcesses--;
    if (processCount[outID] == 0) {
      currIds.erase(std::find(currIds.begin(), currIds.end(), outID));
    }
    needUpdate = false;
    return outID;
  }

  /**
   * Check if the scheduler is empty
   * @return True if scheduler is empty, false otherwise
   */
  bool empty() { return currProcesses == 0; }

  /**
   * Get the number of unique processes in the scheduler
   * @return Number of unique processes in the scheduler
   */
  int GetUniqueProcesses() const { return currIds.size(); }

  /**
   * Get the number of processes currently in the scheduler
   * @return Number of processes currently in the scheduler
   */
  int GetCurrProcesses() const { return currProcesses; }

  /**
   * Get the total number of processes that have been in the scheduler
   * @return Total number of processes that have been in the scheduler
   */
  int GetTotalProcesses() const { return totalProcesses; }
};
}  // namespace cse
#endif  // PROJECT_CSE498_SPRING2025_GROUP_07_TEAM07LIBRARY_SCHEDULER_H
