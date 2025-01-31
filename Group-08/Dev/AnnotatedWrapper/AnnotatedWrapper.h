#pragma once

#include <iostream>
#include <vector>
#include <string>

class AnnotatedWrapper {
protected:
    // Vector of tags associated with the object
    std::vector<std::string> mTags;

public:
    // Adds a tag to the object
    void AddTag(std::string tag);

    // Removes a tag from the object
    void RemoveTag(std::string tag);

    // Remove all tags from the object
    void RemoveAllTags();

    // Gets all tags associated with the object
    std::vector<std::string> GetAllTags();

    // Verifies if a tag is assoicated with the object
    bool VerifyTag(std::string tag);

};