#include "../../../StringSet/StringSet.hpp"
#include "../../../StaticString/StaticString.hpp"
#include <iostream>
#include <string>

#ifndef CSE_FILE_SOURCE_HPP
#define CSE_FILE_SOURCE_HPP
/**
 * @brief A class that will save
 * and load words from and to a file
 * 
 */
class FileSource {
public:
    /**
     * @brief Loads words from a file into a StringSet
     * 
     * @param filename 
     * @return cse::StringSet 
     */
    static cse::StringSet<cse::StaticString<20>> load_file(const std::string& filename);
};

#endif