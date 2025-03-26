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
#include <map>
#include <set>
#include <optional>
#include <vector>

namespace cse
{
  template <typename ProcessID = int>

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
      std::vector<double> processWeights;
    };

    /// List holding the Processes added to the Scheduler
    std::vector<Process> currIds;

    /// Indicator if we need to sort the scheduler again
    bool needUpdate = true;

    /// Vector holding the weights used for each priority value given by a Process
    std::vector<double> weightList;

    /// Queue holding all the Processes that have had their priority values overridden to be pushed to the front of the Scheduler
    std::deque<Process> overrideQueue;

    /// Set containing all the IDs of Processes currently in the Scheduler
    std::set<ProcessID> seenIds;

    /**
     * Calculates priority from a given set of weights
     * @param weights Set of weights to calculate priority with
     * @return Calculated priority value which is SUM(weights[i]*weightList[i])
     */
    constexpr double calculatePriority(const std::vector<double> &weights)
    {
      double priority = 0;
      int weightListSize = weightList.size();
      for (int i = 0; i < weightListSize; i++)
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
        if (currIds[i].id == id)
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
        if (overrideQueue.at(i).id == id)
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
    Scheduler(const std::vector<double> &weights)
    {
      weightList = weights;
    }

    /**
     * Default Destructor
     */
    ~Scheduler() = default;

    /**
     * Adds a process to the Scheduler
     * @param id ID of the process we are adding to the scheduler
     * @param weights Priority weights for this process
     */
    void AddProcess(const ProcessID &id, const std::vector<double> &weights)
    {
      assert(weights.size() == weightList.size());

      size_t numProcesses = seenIds.size();
      seenIds.insert(id);
      if (numProcesses == seenIds.size())
      {
        return;
      }

      double priority = calculatePriority(weights);

      Process process = {id, priority, weights};
      currIds.push_back(process);

      needUpdate = true;
    }

    /**
     * Removes the process of a given ID from the scheduler
     * @param id ID of the process we are removing from the scheduler
     */
    void RemoveProcess(const ProcessID &id)
    {
      int currIdsPos = findIdInCurrIds(id);
      if (currIdsPos != -1)
      {
        currIds.erase(currIds.begin() + currIdsPos);
        seenIds.erase(id);
        needUpdate = true;
        return;
      }

      int overrideQueuePos = findIdInOverrideQueue(id);
      if (overrideQueuePos != -1)
      {
        overrideQueue.erase(overrideQueue.begin() + overrideQueuePos);
        seenIds.erase(id);
        return;
      }
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
        ProcessID outID = overrideQueue.front().id;
        overrideQueue.pop_front();
        seenIds.erase(outID);
        return outID;
      }

      if (needUpdate)
      {
        std::stable_sort(currIds.begin(), currIds.end(), [this](const Process &a, const Process &b)
                         { return a.priorityWeight > b.priorityWeight; });
      }
      ProcessID outID = currIds[0].id;
      currIds.erase(currIds.begin());
      seenIds.erase(outID);
      needUpdate = false;
      return outID;
    }

    /**
     * Overrides the scheduling order to bring a process to the front of the Schedule
     * @param id ID of the process which we are overriding the scheduling order for
     */
    void OverridePriority(const ProcessID &id)
    {
      int currIdsPos = findIdInCurrIds(id);
      if (currIdsPos != -1)
      {
        overrideQueue.push_back(currIds[currIdsPos]);
        currIds.erase(currIds.begin() + currIdsPos);
        return;
      }
    }

    /**
     * Updates the priority value for a process in the Scheduler
     * @param id Id of the process which we are the updating the priority value for
     * @param newWeights New priority weights for the process, which will be used to calculate the new priority value
     */
    void UpdateProcessPriority(const ProcessID &id, const std::vector<double> &newWeights)
    {
      assert(newWeights.size() == weightList.size());
      int currIdsPos = findIdInCurrIds(id);
      double newPriority = calculatePriority(newWeights);

      if (currIdsPos != -1)
      {
        currIds[currIdsPos].priorityWeight = newPriority;
        currIds[currIdsPos].processWeights = newWeights;
        needUpdate = true;
        return;
      }

      int overrideQueuePos = findIdInOverrideQueue(id);
      if (overrideQueuePos != -1)
      {
        overrideQueue.at(overrideQueuePos).processWeights = newWeights;
        overrideQueue.at(overrideQueuePos).priorityWeight = newPriority;
        return;
      }
    }

    /**
     * Updates the weights used by the Scheduler
     * @param newWeights The new weights set to be used by the Scheduler
     */
    void UpdateSchedulerWeights(const std::vector<double> &newWeights)
    {
      assert(newWeights.size() == weightList.size());
      weightList = newWeights;

      int currIdsSize = currIds.size();
      for (int i = 0; i < currIdsSize; i++)
      {
        double newProcessWeight = calculatePriority(currIds[i].processWeights);
        currIds[i].priorityWeight = newProcessWeight;
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
      int currIdsPos = findIdInCurrIds(id);
      if (currIdsPos != -1)
      {
        return currIds[currIdsPos].priorityWeight;
      }

      int overrideQueuePos = findIdInOverrideQueue(id);
      if (overrideQueuePos != -1)
      {
        return overrideQueue.at(overrideQueuePos).priorityWeight;
      }
      return std::nullopt;
    }

    /**
     * Gets the weights set for a given process
     * @param id ID of the process we are getting the weights set for
     * @return Weights set for the given process if it is in the Scheduler, returns std::nullopt otherwise
     */
    constexpr std::optional<std::vector<double>> GetProcessWeights(const ProcessID &id)
    {
      int currIdsPos = findIdInCurrIds(id);
      if (currIdsPos != -1)
      {
        return currIds[currIdsPos].processWeights;
      }

      int overrideQueuePos = findIdInOverrideQueue(id);
      if (overrideQueuePos != -1)
      {
        return overrideQueue.at(overrideQueuePos).processWeights;
      }
      return std::nullopt;
    }

    /**
     * Check if the scheduler is empty
     * @return True if scheduler is empty, false otherwise
     */
    constexpr bool empty() { return currIds.size() == 0 && overrideQueue.empty(); }

    /**
     * Get the number of processes currently in the scheduler
     * @return Number of processes currently in the scheduler
     */
    constexpr int GetCurrProcesses() { return currIds.size() + overrideQueue.size(); }
  };
} // namespace cse
#endif // PROJECT_CSE498_SPRING2025_GROUP_07_TEAM07LIBRARY_SCHEDULER_H
