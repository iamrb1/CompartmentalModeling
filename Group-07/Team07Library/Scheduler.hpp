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
#include <optional>

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

      double priority = 0;
      int weightsSize = weights.size();
      for (int i = 0; i < weightsSize; i++)
      {
        priority += (weights[i] * weightList[i]);
      }

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
      int currIdsSize = currIds.size(), overrideQueueSize = overrideQueue.size();
      for (int i = 0; i < currIdsSize; i++)
      {
        if (currIds[i].id == id)
        {
          currIds.erase(currIds.begin() + i);
          seenIds.erase(id);
          needUpdate = true;
          return;
        }
      }

      for (int i = 0; i < overrideQueueSize; i++)
      {
        if (overrideQueue.at(i).id == id)
        {
          overrideQueue.erase(overrideQueue.begin() + i);
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
      int currIdsSize = currIds.size();
      for (int i = 0; i < currIdsSize; i++)
      {
        if (id == currIds[i].id)
        {
          overrideQueue.push_back(currIds[i]);
          currIds.erase(currIds.begin() + i);
          return;
        }
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
      int weightsSize = weightList.size(), currIdsSize = currIds.size(), overrideQueueSize = overrideQueue.size();
      double newPriority = 0;
      for (int i = 0; i < weightsSize; i++)
      {
        newPriority += (weightList[i] * newWeights[i]);
      }

      for (int i = 0; i < currIdsSize; i++)
      {
        if (id == currIds[i].id)
        {
          currIds[i].priorityWeight = newPriority;
          currIds[i].processWeights = newWeights;
          needUpdate = true;
          return;
        }
      }

      for (int i = 0; i < overrideQueueSize; i++)
      {
        if (overrideQueue.at(i).id == id)
        {
          overrideQueue.at(i).processWeights = newWeights;
          overrideQueue.at(i).priorityWeight = newPriority;
          return;
        }
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

      int currIdsSize = currIds.size(), weightListSize = weightList.size();
      for (int i = 0; i < currIdsSize; i++)
      {
        double newProcessWeight = 0;
        for (int j = 0; j < weightListSize; j++)
        {
          newProcessWeight += (weightList[i] * currIds[i].processWeights[i]);
        }
        currIds[i].priorityWeight = newProcessWeight;
      }
      needUpdate = true;
    }

    /**
     * Gets the priority value for a process
     * @param id ID of the process we are getting the priority for
     * @return Priority value for the given process if it is in the Scheduler, returns std::nullopt otherwise
     */
    std::optional<double> GetProcessPriority(const ProcessID &id)
    {
      int currIdsSize = currIds.size(), overrideQueueSize = overrideQueue.size();

      for (int i = 0; i < currIdsSize; i++)
      {
        if (id == currIds[i].id)
        {
          return currIds[i].priorityWeight;
        }
      }

      for (int i = 0; i < overrideQueueSize; i++)
      {
        if (overrideQueue.at(i).id == id)
        {
          return overrideQueue.at(i).priorityWeight;
        }
      }
      return std::nullopt;
    }

    /**
     * Gets the weights set for a given process
     * @param id ID of the process we are getting the weights set for
     * @return Weights set for the given process if it is in the Scheduler, returns std::nullopt otherwise
     */
    std::optional<std::vector<double>> GetProcessWeights(const ProcessID &id)
    {
      int currIdsSize = currIds.size(), overrideQueueSize = overrideQueue.size();

      for (int i = 0; i < currIdsSize; i++)
      {
        if (id == currIds[i].id)
        {
          return currIds[i].processWeights;
        }
      }

      for (int i = 0; i < overrideQueueSize; i++)
      {
        if (overrideQueue.at(i).id == id)
        {
          return overrideQueue.at(i).processWeights;
        }
      }
      return std::nullopt;
    }

    /**
     * Check if the scheduler is empty
     * @return True if scheduler is empty, false otherwise
     */
    bool empty() const { return currIds.size() == 0 && overrideQueue.empty(); }

    /**
     * Get the number of processes currently in the scheduler
     * @return Number of processes currently in the scheduler
     */
    int GetCurrProcesses() const { return currIds.size() + overrideQueue.size(); }
  };
} // namespace cse
#endif // PROJECT_CSE498_SPRING2025_GROUP_07_TEAM07LIBRARY_SCHEDULER_H
