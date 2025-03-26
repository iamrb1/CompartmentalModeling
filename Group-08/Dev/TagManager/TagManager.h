#ifndef TAGMANAGER_TAGMANAGER_H
#define TAGMANAGER_TAGMANAGER_H

/**
 * @class   TagManager
 * @file    TagManager.hpp
 * @author  Kelsi Elliott
 * @brief   The TagManager class helps to "label" items with a two-way
 *          association to make looking up by "tag" or "task" simple
 */

#include <string>
#include <unordered_map>
#include <unordered_set>
using std::string;

namespace cse {

class TagManager {
private:
    // Maps from task -> set of tags, and from tag -> set of entries
    std::unordered_map<string, std::unordered_set<string> > mTaskToTag;
    std::unordered_map<string, std::unordered_set<string> > mTagToTask;

public:
    // Default constructor
    TagManager() = default;

    // Copy constructor
    TagManager(const TagManager& other) {
        // Copy both maps from the other TagManager1
        mTaskToTag = other.mTaskToTag;
        mTagToTask = other.mTagToTask;
    }

    // Copy assignment operator
    TagManager& operator=(const TagManager& other) {
        if (this != &other) {  // Check for self-assignment
            mTaskToTag = other.mTaskToTag;
            mTagToTask = other.mTagToTask;
        }
        return *this;
    }

    // Destructor
    ~TagManager() = default;

    // 1. Adds a tag to the task
    void addTag(const string& task, const string& tag);

    // 2. Removes a tag from the task
    void removeTag(const string& task, const string& tag);

    // 3. Retrieves all tags associated with an task
    std::unordered_set<string> getTags(const string& task) const;

    // 4. Retrieves all entries associated with a specific tag
    std::unordered_set<string> getTaskTags(const string& tag) const;

    // 5. Clears all tags for a specific task
    void clearTagsForTask(const string& task);

    // 6. Clears all entries associated with a specific tag
    void clearTags(const string& tag);

    // 7. Checks if an task has a specific tag
    bool hasTag(const string& task, const string& tag) const;
};

#endif //TAGMANAGER_TAGMANAGER_H

}   // namespace cse