#ifndef TAGMANAGER_TAGMANAGER_H
#define TAGMANAGER_TAGMANAGER_H

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
    void AddTag(const string& task, const string& tag);

    // 2. Removes a tag from the task
    void RemoveTag(const string& task, const string& tag);

    // 3. Retrieves all tags associated with an task
    std::unordered_set<string> GetTags(const string& task) const;

    // 4. Retrieves all entries associated with a specific tag
    std::unordered_set<string> GetTaskTags(const string& tag) const;

    // 5. Clears all tags for a specific task
    void ClearTagsForTask(const string& task);

    // 6. Clears all entries associated with a specific tag
    void ClearTags(const string& tag);

    // 7. Checks if an task has a specific tag
    bool HasTag(const string& task, const string& tag) const;
};

#endif //TAGMANAGER_TAGMANAGER_H

}   // namespace cse