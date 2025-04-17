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
#include <set>
#include <queue>
#include <functional>

namespace cse{
  template <typename ProcessID = int, int numWeights = 1>


  /*
   * Priority queue to schedule processes to run based on how often each process
   * is used
   */
  class Scheduler
  {
    static_assert(numWeights > 0);

  private:
    /// Struct containing a process's id, its priority value, and its weights
    struct Process
    {
      ProcessID id;
      double priorityWeight;
      std::array<double, numWeights> processWeights;

      Process() : id(), priorityWeight(0), processWeights{} {}
      Process(const ProcessID &id, double priorityWeight, const std::array<double, numWeights> &processWeights) : id(id), priorityWeight(std::move(priorityWeight)), processWeights(processWeights) {}
    };

    struct Comparator
    {
      const std::unordered_map<ProcessID, Process> *processMap;
      bool operator()(const ProcessID &a, const ProcessID &b) const
      {
        if ((*processMap).find(a) == (*processMap).end())
        {
          return false;
        }
        else if ((*processMap).find(b) == (*processMap).end())
        {
          return true;
        }
        return (*processMap).at(a).priorityWeight < (*processMap).at(b).priorityWeight;
      }
    };

    /// Lambda function to allow a user to change how priority is calculated in the Scheduler
    std::function<double(const std::array<double, numWeights> &)> priorityCalcLambda;

    /// Heap holding the Processes added to the Scheduler
    std::priority_queue<ProcessID, std::vector<ProcessID>, Comparator> currIds;

    /// Map of each ProcessID to its associated Process object
    std::unordered_map<ProcessID, Process> processMap;

    /// Vector holding the weights used for each priority value given by a Process
    std::array<double, numWeights> weightList;

    /// Queue holding all the Processes that have had their priority values overridden to be pushed to the front of the Scheduler
    std::deque<ProcessID> overrideQueue;

    /// Set holding the ProcessIDs we will delete from the heap at a later point
    std::set<ProcessID> deletedIDs;

    /**
     * Calculates priority from a given set of weights
     * @param weights Set of weights to calculate priority with
     * @return Calculated priority value which is SUM(weights[i]*weightList[i])
     */
    constexpr double calculatePriority(const std::array<double, numWeights> &weights)
    {
      return priorityCalcLambda(weights);
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
    Scheduler(const std::array<double, numWeights> &weights, std::function<double(const std::array<double, numWeights> &)> lambdaPriority = nullptr) : currIds(Comparator{&processMap}), weightList(weights)
    {
      if (lambdaPriority)
      {
        priorityCalcLambda = std::move(lambdaPriority);
      }
      else
      {
        priorityCalcLambda = [this](const std::array<double, numWeights> &weights)
        {
          double priority = 0;
          for (int i = 0; i < numWeights; i++)
          {
            priority += (weightList[i] * weights[i]);
          }
          return priority;
        };
      }
    }

    /**
     * Default Destructor
     */
    ~Scheduler() = default;

    /**
     * Copy Constructor
     */
    Scheduler(const Scheduler &scheduler) : processMap(scheduler.processMap), currIds(Comparator{&processMap}), weightList(scheduler.weightList), overrideQueue(scheduler.overrideQueue), priorityCalcLambda(scheduler.priorityCalcLambda) {}

    /**
     * Move Constructor
     */
    Scheduler(Scheduler &&scheduler) : processMap(std::move(scheduler.processMap)), currIds(Comparator{&processMap}), weightList(std::move(scheduler.weightList)), overrideQueue(std::move(scheduler.overrideQueue)), priorityCalcLambda(std::move(scheduler.priorityCalcLambda)) {}

    /**
     * Adds a process to the Scheduler
     * @param id ID of the process we are adding to the scheduler
     * @param weights Priority weights for this process
     * @return True if the process was successfully added to the Scheduler, false if it was not
     */
    bool AddProcess(const ProcessID &id, const std::array<double, numWeights> &weights)
    {
      if (!processMap.emplace(id, Process(id, calculatePriority(weights), weights)).second)
      {
        return false;
      }

      currIds.push(id);
      return true;
    }

    /**
     * Removes the process of a given ID from the scheduler
     * @param id ID of the process we are removing from the scheduler
     * @return True if the process was successfully removed from the Scheduler, false if it was not
     */
    bool RemoveProcess(const ProcessID &id)
    {
      auto overrideQueuePos = std::find(overrideQueue.begin(), overrideQueue.end(), id);
      if (overrideQueuePos != overrideQueue.end())
      {
        overrideQueue.erase(overrideQueuePos);
        processMap.erase(id);
        return true;
      }

      if (processMap.find(id) != processMap.end())
      {
        deletedIDs.insert(id);
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

      while (!currIds.empty() && deletedIDs.erase(currIds.top()))
      {
        processMap.erase(currIds.top());
        currIds.pop();
      }

      if (currIds.empty())
      {
        return std::nullopt;
      }

      ProcessID outID = currIds.top();
      currIds.pop();
      processMap.erase(outID);
      return outID;
    }

    /**
     * Overrides the scheduling order to bring a process to the front of the Schedule
     * @param id ID of the process which we are overriding the scheduling order for
     * @return True if the process's priority was successfully overriden, false if it was not
     */
    bool OverridePriority(const ProcessID &id)
    {
      if (processMap.find(id) != processMap.end() && deletedIDs.insert(id).second)
      {
        overrideQueue.push_back(id);
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
    bool UpdateProcessPriority(const ProcessID &id, const std::array<double, numWeights> &newWeights)
    {
      if (processMap.find(id) != processMap.end())
      {
        std::vector<ProcessID> heapIDs;
        while (!currIds.empty())
        {
          heapIDs.push_back(currIds.top());
          currIds.pop();
        }
        processMap[id].priorityWeight = calculatePriority(newWeights);
        processMap[id].processWeights = newWeights;

        for (const ProcessID &id : heapIDs)
        {
          currIds.push(id);
        }
        return true;
      }
      return false;
    }

    /**
     * Updates the weights used by the Scheduler
     * @param newWeights The new weights set to be used by the Scheduler
     */
    void UpdateSchedulerWeights(const std::array<double, numWeights> &newWeights)
    {
      weightList = newWeights;
      std::vector<ProcessID> heapIDs;
      while (!currIds.empty())
      {
        heapIDs.push_back(currIds.top());
        currIds.pop();
      }

      for (const ProcessID &id : heapIDs)
      {
        processMap[id].priorityWeight = calculatePriority(processMap[id].processWeights);
        currIds.push(id);
      }
    }

    /**
     * Gets the priority value for a process
     * @param id ID of the process we are getting the priority for
     * @return Priority value for the given process if it is in the Scheduler, returns std::nullopt otherwise
     */
    const std::optional<double> GetProcessPriority(const ProcessID &id)
    {
      if (processMap.find(id) != processMap.end())
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
    const std::optional<std::array<double, numWeights>> GetProcessWeights(const ProcessID &id)
    {
      if (processMap.find(id) != processMap.end())
      {
        return processMap[id].processWeights;
      }

      return std::nullopt;
    }

    /**
     * Check if the scheduler is empty
     * @return True if scheduler is empty, false otherwise
     */
    constexpr bool empty() { return GetCurrProcesses() == 0; }

    /**
     * Get the number of processes currently in the scheduler
     * @return Number of processes currently in the scheduler
     */
    constexpr int GetCurrProcesses() { return currIds.size() + overrideQueue.size() - deletedIDs.size(); }
  };
} // namespace cse
#endif // PROJECT_CSE498_SPRING2025_GROUP_07_TEAM07LIBRARY_SCHEDULER_H
