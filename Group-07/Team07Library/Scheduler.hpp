/**
 * @file Scheduler.h
 * @author Nathan Staniszewski
 *
 *
 */

#ifndef PROJECT_CSE498_SPRING2025_GROUP_07_TEAM07LIBRARY_SCHEDULER_H
#define PROJECT_CSE498_SPRING2025_GROUP_07_TEAM07LIBRARY_SCHEDULER_H

#include <algorithm>
#include <cassert>
#include <deque>
#include <unordered_map>
#include <optional>
#include <vector>
#include <array>

namespace cse
{
  template <typename ProcessID = int, int numWeights=1>

  /*
   * Priority queue to schedule processes to run based on how often each process
   * is used
   */
  class Scheduler
  {
  private:
    /// Struct containing a process's id, its priority value, and its weights
    struct Process
    {
      ProcessID id;
      double priorityWeight;
      std::array<double,numWeights> processWeights;

      Process(): id(), priorityWeight(0), processWeights{} {}
      Process(const ProcessID &id, double priorityWeight, const std::array<double,numWeights>& processWeights): id(id), priorityWeight(std::move(priorityWeight)), processWeights(processWeights) {}
    };

    /// List holding the Processes added to the Scheduler
    std::vector<ProcessID> currIds;

    /// Map of each ProcessID to its associated Process object
    std::unordered_map<ProcessID,Process> processMap;

    /// Indicator if we need to sort the scheduler again
    bool needUpdate = true;

    /// Vector holding the weights used for each priority value given by a Process
    std::array<double,numWeights> weightList;

    /// Queue holding all the Processes that have had their priority values overridden to be pushed to the front of the Scheduler
    std::deque<ProcessID> overrideQueue;

    /**
     * Calculates priority from a given set of weights
     * @param weights Set of weights to calculate priority with
     * @return Calculated priority value which is SUM(weights[i]*weightList[i])
     */
    constexpr double calculatePriority(const std::array<double,numWeights> &weights)
    {
      double priority = 0;
      for (int i = 0; i < numWeights; i++)
      {
        priority += (weightList[i] * weights[i]);
      }
      return priority;
    }

    /**
     * Finds the position of a process in currIds from its id
     * @param id ID of the process we are trying to find
     * @return Process's position in currIds if it is there, returns -1 otherwise
     */
    constexpr int findIdInCurrIds(const ProcessID &id)
    {
      int currIdsSize = currIds.size();
      for (int i = 0; i < currIdsSize; i++)
      {
        if (currIds[i] == id)
        {
          return i;
        }
      }
      return -1;
    }

    /**
     * Finds the position of a process in the override queue from its id
     * @param id ID of the process we are trying to find
     * @return Process's position in the override queue if it is there, returns -1 otherwise
     */
    constexpr int findIdInOverrideQueue(const ProcessID &id)
    {
      int overrideQueueSize = overrideQueue.size();
      for (int i = 0; i < overrideQueueSize; i++)
      {
        if (overrideQueue.at(i) == id)
        {
          return i;
        }
      }
      return -1;
    }

  public:
    /**
     * Default Constructor is deleted
     */
    Scheduler() = delete;

    /**
     * Constructor for Scheduler with a given set of weights
     * @param weights Weights used to determine the priority for processes added to the Scheduler
     */
    Scheduler(const std::array<double,numWeights> &weights) : weightList(weights) {}

    /**
     * Default Destructor
     */
    ~Scheduler() = default;

    /**
     * Adds a process to the Scheduler
     * @param id ID of the process we are adding to the scheduler
     * @param weights Priority weights for this process
     * @return True if the process was successfully added to the Scheduler, false if it was not
     */
    bool AddProcess(const ProcessID &id, const std::array<double,numWeights> &weights)
    {
      if (!processMap.emplace(id,Process(id, calculatePriority(weights), weights)).second)
      {
        return false;
      }

      currIds.emplace_back(id);
      needUpdate = true;
      return true;
    }

    /**
     * Removes the process of a given ID from the scheduler
     * @param id ID of the process we are removing from the scheduler
     * @return True if the process was successfully removed from the Scheduler, false if it was not
     */
    bool RemoveProcess(const ProcessID &id)
    {
      int currIdsPos = findIdInCurrIds(id);
      if (currIdsPos != -1)
      {
        currIds.erase(currIds.begin() + currIdsPos);
        processMap.erase(id);
        needUpdate = true;
        return true;
      }

      int overrideQueuePos = findIdInOverrideQueue(id);
      if (overrideQueuePos != -1)
      {
        overrideQueue.erase(overrideQueue.begin() + overrideQueuePos);
        processMap.erase(id);
        return true;
      }
      return false;
    }

    /**
     * Gets the ID for the highest priority process according to the scheduler,
     * and removes it from the scheduler
     * @return ID of the highest priority process
     */
    std::optional<ProcessID> PopNextProcess()
    {
      if (empty())
      { // Make sure the Scheduler isn't empty before popping a process from it
        return std::nullopt;
      }

      if (!overrideQueue.empty())
      {
        ProcessID outID = overrideQueue.front();
        overrideQueue.pop_front();
        processMap.erase(outID);
        return outID;
      }

      if (needUpdate)
      {
        std::stable_sort(currIds.begin(), currIds.end(), [&](const ProcessID &a, const ProcessID &b)
                         { return processMap[a].priorityWeight > processMap[b].priorityWeight; });
      }
      ProcessID outID = currIds[0];
      currIds.erase(currIds.begin());
      processMap.erase(outID);
      needUpdate = false;
      return outID;
    }

    /**
     * Overrides the scheduling order to bring a process to the front of the Schedule
     * @param id ID of the process which we are overriding the scheduling order for
     * @return True if the process's priority was successfully overriden, false if it was not
     */
    bool OverridePriority(const ProcessID &id)
    {
      int currIdsPos = findIdInCurrIds(id);
      if (currIdsPos != -1)
      {
        overrideQueue.push_back(currIds[currIdsPos]);
        currIds.erase(currIds.begin() + currIdsPos);
        return true;
      }
      return false;
    }

    /**
     * Updates the priority value for a process in the Scheduler
     * @param id Id of the process which we are the updating the priority value for
     * @param newWeights New priority weights for the process, which will be used to calculate the new priority value
     * @return True if the process's priority was successfully update, false if it was not
     */
    bool UpdateProcessPriority(const ProcessID &id, const std::array<double,numWeights> &newWeights)
    {
      if(processMap.find(id) != processMap.end())
      {
        processMap[id].priorityWeight = calculatePriority(newWeights);
        processMap[id].processWeights = newWeights;
        needUpdate=true;
        return true;
      }
      return false;
    }

    /**
     * Updates the weights used by the Scheduler
     * @param newWeights The new weights set to be used by the Scheduler
     */
    void UpdateSchedulerWeights(const std::array<double,numWeights> &newWeights)
    {
      weightList = newWeights;
      for(const ProcessID &id : currIds)
      {
        processMap[id].priorityWeight = calculatePriority(processMap[id].processWeights);
      }
      needUpdate = true;
    }

    /**
     * Gets the priority value for a process
     * @param id ID of the process we are getting the priority for
     * @return Priority value for the given process if it is in the Scheduler, returns std::nullopt otherwise
     */
    constexpr std::optional<double> GetProcessPriority(const ProcessID &id)
    {
      if(processMap.find(id) != processMap.end())
      {
        return processMap[id].priorityWeight;
      }

      return std::nullopt;
    }

    /**
     * Gets the weights set for a given process
     * @param id ID of the process we are getting the weights set for
     * @return Weights set for the given process if it is in the Scheduler, returns std::nullopt otherwise
     */
    constexpr std::optional<std::array<double,numWeights>> GetProcessWeights(const ProcessID &id)
    {
      if(processMap.find(id) != processMap.end())
      {
        return processMap[id].processWeights;
      }

      return std::nullopt;
    }

    /**
     * Check if the scheduler is empty
     * @return True if scheduler is empty, false otherwise
     */
    constexpr bool empty() { return currIds.empty() && overrideQueue.empty(); }

    /**
     * Get the number of processes currently in the scheduler
     * @return Number of processes currently in the scheduler
     */
    constexpr int GetCurrProcesses() { return currIds.size() + overrideQueue.size(); }
  };
} // namespace cse
#endif // PROJECT_CSE498_SPRING2025_GROUP_07_TEAM07LIBRARY_SCHEDULER_H
