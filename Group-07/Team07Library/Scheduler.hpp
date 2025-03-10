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
#include <cassert>
#include <deque>

namespace cse {
/*
 * Priority queue to schedule processes to run based on how often each process
 * is used
 */
class Scheduler {
 private:
  /// Struct containing a process's id and its priority value
  struct Process {
    int id;
    double priorityWeight;
  };

  /// List holding the IDs of all the items added to the scheduler and their priority weights
  std::vector<Process> currIds;

  /// Indicator if we need to sort the scheduler again
  bool needUpdate = true;

  /// Vector holding the weights used for each priority value given by a Process
  std::vector<double> weightList;

  /// Queue holding all the Processes that have had their priority values overridden to be pushed to the front of the Scheduler
  std::deque<Process> overrideQueue;

 public:
  /**
   * Default Constructor is deleted
   */
  Scheduler() = delete;

  /**
   * Constructor for Scheduler with a given set of weights
   * @param weights
   */
  Scheduler(const std::vector<double> &weights) {
    weightList=weights;
  }

  /**
   * Default Destructor
   */
  ~Scheduler() = default;

  /**
   * Adds a process to the Scheduler
   * @param id ID of the process we are adding to the scheduler
   */
  void AddProcess(const int id,const std::vector<double> &weights,const int processCount=1) {
    assert(weights.size()==weightList.size());
    double priority=0;
    int n=weights.size();
    for(int i=0;i<n;i++) {
      priority+=(weights[i]*weightList[i]);
    }

    Process process={id,priority};
    for(int i=0;i<processCount;i++) {
      currIds.push_back(process);
    }
    needUpdate = true;
  }

  /**
   * Removes the process of a given ID from the scheduler
   * @param id ID of the process we are removing from the scheduler
   */
  void RemoveProcess(const int id,int removeCount=1) {
    for(int i=currIds.size()-1;i>=0 && removeCount>0;i--) {
      if(currIds[i].id==id) {
        removeCount--;
        currIds.erase(currIds.begin()+i);
      }
    }

    if(removeCount>0 && !overrideQueue.empty()) {
      for(int i=overrideQueue.size()-1;i>=0 && removeCount>0;i--) {
        if(overrideQueue.at(i).id==id) {
          removeCount--;
          overrideQueue.erase(overrideQueue.begin()+i);
        }
      }
    }
    needUpdate = true;
  }

  /**
   * Gets the ID for the highest priority process according to the scheduler,
   * and removes it from the scheduler
   * @return ID of the highest priority process
   */
  std::optional<int> PopNextProcess() {
    if (empty()) { // Make sure the Scheduler isn't empty before popping a process from it
      return std::nullopt;
    }

    if(overrideQueue.size()>0) {
      int outID=overrideQueue.front().id;
      overrideQueue.pop_front();
      return outID;
    }

    if (needUpdate) {
      std::stable_sort(currIds.begin(),currIds.end(),[this](const Process& a,const Process& b) {
          return a.priorityWeight>b.priorityWeight;
      });
    }
    int outID = currIds[0].id;
    currIds.erase(currIds.begin());
    needUpdate = false;
    return outID;
  }

  void OverridePriority(const int id) {
    for(int i=currIds.size()-1;i>=0;i--) {
      if(id==currIds[i].id) {
        overrideQueue.push_back(currIds[i]);
        currIds.erase(currIds.begin()+1);
        return;
      }
    }
  }

  /**
   * Check if the scheduler is empty
   * @return True if scheduler is empty, false otherwise
   */
  bool empty() { return currIds.size() == 0 && overrideQueue.empty(); }

  /**
   * Get the number of processes currently in the scheduler
   * @return Number of processes currently in the scheduler
   */
  int GetCurrProcesses() const { return currIds.size()+overrideQueue.size(); }
};
}  // namespace cse
#endif  // PROJECT_CSE498_SPRING2025_GROUP_07_TEAM07LIBRARY_SCHEDULER_H
