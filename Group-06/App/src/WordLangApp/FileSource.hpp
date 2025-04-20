/**
 * @file FileSource.hpp
 * @author Mehmet Efe Caylan
 * @author Will Crawford
 * @author Ivan Bega
 * @author Orhan Aydin
 * @author Emil Rahn Siegel
 * @brief FileSource header file.
 * @version 0.1
 * @date 2025-04-19
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "../../../StringSet/StringSet.hpp"
#include "../../../StaticString/StaticString.hpp"
#include <iostream>
#include <string>

#ifndef CSE_FILE_SOURCE_HPP
#define CSE_FILE_SOURCE_HPP
/**
 * @brief A namespace that will save
 * and load words from and to a file
 * 
 */
namespace FileSource {
    /**
     * @brief Loads words from a file into a StringSet
     * 
     * @param filename Name of a file to load words from
     * @return cse::StringSet set of words loaded from a file
     */
    cse::StringSet<cse::StaticString<30>> load_file(const std::string& filename);

    /**
     * @brief Saves words into a file from a list
     * 
     * @param filename Filename the set will be saved into
     * @param set The set that will be saved
     * @return True if successful, false otherwise
     */
    bool save_file(const std::string& filename, cse::StringSet<cse::StaticString<30>> set);
};

#endif // CSE_FILE_SOURCE_HPP