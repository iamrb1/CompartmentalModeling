/**
 * StringSet class created by Orhan Aydin
 */

#ifndef PROJECTCLONE_STRINGSET_H
#define PROJECTCLONE_STRINGSET_H

#include <unordered_set>
#include <string>
#include <vector>
#include <iostream>

namespace cse{
class StringSet {

public:

    void insert(const std::string &str);

    void erase(const std::string &str);

    void clear();

    size_t size() const;

    bool empty() const;

    StringSet Union(const StringSet &other) const;

    StringSet Intersection(const StringSet &other) const;

private:
    std::unordered_set <std::string> m_elements;

};
}

#endif //PROJECTCLONE_STRINGSET_H
