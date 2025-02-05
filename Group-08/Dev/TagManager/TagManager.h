//
// Created by Kelsi Elliott on 2/3/25.
//

#ifndef TAGMANAGER_TAGMANAGER_H
#define TAGMANAGER_TAGMANAGER_H

#include <string>
#include <unordered_map>
#include <unordered_set>

class TagManager {
private:
    // Maps from entry -> set of tags, and from tag -> set of entries
    std::unordered_map<std::string, std::unordered_set<std::string> > entry_to_tags;
    std::unordered_map<std::string, std::unordered_set<std::string> > tag_to_entries;

public:
    // 1. Adds a tag to the entry
    void add_tag(const std::string& entry, const std::string& tag);

    // 2. Removes a tag from the entry
    void remove_tag(const std::string& entry, const std::string& tag);

    // 3. Retrieves all tags associated with an entry
    std::unordered_set<std::string> get_tags(const std::string& entry);

    // 4. Retrieves all entries associated with a specific tag
    std::unordered_set<std::string> get_entries(const std::string& tag);

    // 5. Clears all tags for a specific entry
    void clear_entry_tags(const std::string& entry);

    // 6. Clears all entries associated with a specific tag
    void clear_tag_entries(const std::string& tag);

    // 7. Checks if an entry has a specific tag
    bool has_tag(const std::string& entry, const std::string& tag);
};


#endif //TAGMANAGER_TAGMANAGER_H
