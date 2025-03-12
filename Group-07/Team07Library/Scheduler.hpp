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
#include <set>
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
    std::vector<double> processWeights;
  };

  /// List holding the IDs of all the items added to the scheduler and their priority weights
  std::vector<Process> currIds;

  /// Indicator if we need to sort the scheduler again
  bool needUpdate = true;

  /// Vector holding the weights used for each priority value given by a Process
  std::vector<double> weightList;

  /// Queue holding all the Processes that have had their priority values overridden to be pushed to the front of the Scheduler
  std::deque<Process> overrideQueue;

  /// Set containing all the IDs of Processes currently in the Scheduler
  std::set<int> seenIds;

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
  void AddProcess(const int id,const std::vector<double> &weights) {
    assert(weights.size()==weightList.size());

    size_t numProcesses=seenIds.size();
    seenIds.insert(id);
    if(numProcesses == seenIds.size()) {
        return;
    }

    double priority=0;
    int weightsSize=weights.size();
    for(int i=0;i<weightsSize;i++) {
      priority+=(weights[i]*weightList[i]);
    }

    Process process={id,priority,weights};
    currIds.push_back(process);

    needUpdate = true;
  }

  /**
   * Removes the process of a given ID from the scheduler
   * @param id ID of the process we are removing from the scheduler
   */
  void RemoveProcess(const int id) {
    int currIdsSize=currIds.size(),overrideQueueSize=overrideQueue.size();
    for(int i=0;i<currIdsSize;i++) {
      if(currIds[i].id==id) {
        currIds.erase(currIds.begin()+i);
        seenIds.erase(id);
        needUpdate=true;
        return;
      }
    }

    for(int i=0;i<overrideQueueSize;i++) {
      if(overrideQueue.at(i).id==id) {
        overrideQueue.erase(overrideQueue.begin()+i);
        seenIds.erase(id);
        return;
      }
    }
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

    if(!overrideQueue.empty()) {
      int outID=overrideQueue.front().id;
      overrideQueue.pop_front();
      seenIds.erase(outID);
      return outID;
    }

    if (needUpdate) {
      std::stable_sort(currIds.begin(),currIds.end(),[this](const Process& a,const Process& b) {
          return a.priorityWeight>b.priorityWeight;
      });
    }
    int outID = currIds[0].id;
    currIds.erase(currIds.begin());
    seenIds.erase(outID);
    needUpdate = false;
    return outID;
  }

  void OverridePriority(const int id) {
    int currIdsSize=currIds.size();
    for(int i=0;i<currIdsSize;i++) {
      if(id==currIds[i].id) {
        overrideQueue.push_back(currIds[i]);
        currIds.erase(currIds.begin()+i);
        return;
      }
    }
  }

  void UpdateProcessPriority(const int id,const std::vector<double> &newWeights) {
      assert(newWeights.size()==weightList.size());
      int weightsSize=weightList.size(),currIdsSize=currIds.size(),overrideQueueSize=overrideQueue.size();
      double newPriority=0;
      for(int i=0;i<weightsSize;i++) {
          newPriority+=(weightList[i]*newWeights[i]);
      }

      for(int i=0;i<currIdsSize;i++) {
          if(id==currIds[i].id) {
              currIds[i].priorityWeight=newPriority;
              currIds[i].processWeights=newWeights;
              needUpdate=true;
              return;
          }
      }

      for(int i=0;i<overrideQueueSize;i++) {
         if(overrideQueue.at(i).id==id) {
            overrideQueue.at(i).processWeights=newWeights;
            overrideQueue.at(i).priorityWeight=newPriority;
            return;
         }
      }
  }

  void UpdateSchedulerWeights(std::vector<double> &newWeights) {
      assert(newWeights.size()==weightList.size());
      weightList=newWeights;

      int currIdsSize=currIds.size(),weightListSize=weightList.size();
      for(int i=0;i<currIdsSize;i++) {
          double newProcessWeight=0;
          for(int j=0;j<weightListSize;j++) {
              newProcessWeight+=(weightList[i]*currIds[i].processWeights[i]);
          }
          currIds[i].priorityWeight=newProcessWeight;
      }
      needUpdate=true;
  }

  std::optional<double> GetProcessPriority(const int id) {
      int currIdsSize=currIds.size(),overrideQueueSize=overrideQueue.size();

      for(int i=0;i<currIdsSize;i++) {
          if(id==currIds[i].id) {
              return currIds[i].priorityWeight;
          }
      }

      for(int i=0;i<overrideQueueSize;i++) {
          if(overrideQueue.at(i).id==id) {
              return overrideQueue.at(i).priorityWeight;
          }
      }
      return std::nullopt;
  }

  std::optional<std::vector<double>> GetProcessWeights(const int id) {
      int currIdsSize=currIds.size(),overrideQueueSize=overrideQueue.size();

      for(int i=0;i<currIdsSize;i++) {
          if(id==currIds[i].id) {
              return currIds[i].processWeights;
          }
      }

      for(int i=0;i<overrideQueueSize;i++) {
          if(overrideQueue.at(i).id==id) {
              return overrideQueue.at(i).processWeights;
          }
      }
      return std::nullopt;
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
