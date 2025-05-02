#ifndef TAGMANAGER_TAGMANAGER_HPP
#define TAGMANAGER_TAGMANAGER_HPP

/**
 * @class   TagManager
 * @file    TagManager.hpp
 * @author  Kelsi Elliott
 * @brief   The TagManager class helps to "label" items with a two-way
 *          association to make looking up by "tag" or "task" simple
 */

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

namespace cse {

class TagManager {
    private:
        // Maps from task -> vector of tags, and from tag -> vector of tasks
        std::unordered_map<std::string, std::vector<std::string>> mTaskToTag;
        std::unordered_map<std::string, std::vector<std::string>> mTagToTask;

        // Helper function to add unique item to a vector
        void addUnique(std::vector<std::string>& vec, const std::string& item) {
            if (std::find(vec.begin(), vec.end(), item) == vec.end()) {
                vec.push_back(item);
            }
        }

        // Helper function to remove an item from a vector
        void removeItem(std::vector<std::string>& vec, const std::string& item) {
            auto it = std::remove(vec.begin(), vec.end(), item);
            if (it != vec.end()) {
                vec.erase(it, vec.end());
            }
        }

public:
        // Default constructor
        TagManager() = default;

        // Copy constructor
        TagManager(const TagManager& other) = default;

        // Copy assignment operator
        TagManager& operator=(const TagManager&) = default;

        // Move constructor
        TagManager(TagManager&& other) = default;

        // Destructor
        ~TagManager() = default;

        /**
         * Adds a tag to a task
         * 
         * @param task
         * @param tag
         */
        void addTag(const std::string& task, const std::string& tag) {
            assert(!task.empty() && !tag.empty() && "Task or tag cannot be empty");
            addUnique(mTaskToTag[task], tag);
            addUnique(mTagToTask[tag], task);
        }

        /**
         * Removes a tag from a task
         * 
         * @param task
         * @param tag
         */
        void removeTag(const std::string& task, const std::string& tag) {
            assert(!task.empty() && !tag.empty());
            auto taskIt = mTaskToTag.find(task);
            if (taskIt != mTaskToTag.end()) {
                removeItem(taskIt->second, tag);
                if (taskIt->second.empty()) {
                    mTaskToTag.erase(taskIt);
                }
            }

            auto tagIt = mTagToTask.find(tag);
            if (tagIt != mTagToTask.end()) {
                removeItem(tagIt->second, task);
                if (tagIt->second.empty()) {
                    mTagToTask.erase(tagIt);
                }
            }
        }

        /**
         * Return all the tags 
         * 
         * @param task
         * @return tags --> retrun empty set if no
         */
        std::vector<std::string> getTags(const std::string& task) const {
            static const std::unordered_set<std::string> emptySet; // fallback for missing keys
            auto it = mTags.find(id);
            return it != mTags.end() ? it->second : emptySet;
        }

        /**
         * Return all the tags associated with all the tasks
         * 
         * @param task
         * @return tags --> retrun empty set if no
         */
        std::vector<std::string> getTaskTags(const std::string& tag) const {
            auto it = mTagToTask.find(tag);
            if (it != mTagToTask.end()) {
                return it->second;
            }
            return {};
        }

        /**
         * Clears all the tags associated with the task
         * 
         * @param task
         */
        void clearTagsForTask(const std::string& task) {
            assert(!task.empty());
            auto it = mTaskToTag.find(task);
            if (it != mTaskToTag.end()) {
                for (const auto& tag : it->second) {
                    auto& tasks = mTagToTask[tag];
                    removeItem(tasks, task);
                    if (tasks.empty()) {
                        mTagToTask.erase(tag);
                    }
                }
                mTaskToTag.erase(it);
            }
        }

        /**
         * Clears all the tags associated with a specific tag
         * 
         * @param tag
         */
        void clearTags(const std::string& tag) {
            assert(!tag.empty());
            auto it = mTagToTask.find(tag);
            if (it != mTagToTask.end()) {
                for (const auto& task : it->second) {
                    auto& tags = mTaskToTag[task];
                    removeItem(tags, tag);
                    if (tags.empty()) {
                        mTaskToTag.erase(task);
                    }
                }
                mTagToTask.erase(it);
            }
        }

        /**
         * Checks if a task has a specific tag
         * 
         * @param task
         * @return bool 
         */
        [[nodiscard]] bool hasTag(const std::string& task, const std::string& tag) const {
            auto it = mTags.find(id);
            return it != mTags.end() && it->second.count(tag);
        }
    };

} // namespace cse

#endif // TAGMANAGER_TAGMANAGER_H
