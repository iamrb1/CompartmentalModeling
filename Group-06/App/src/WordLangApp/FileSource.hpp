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
#include <iostream>
#include <string>

#include "../../../StaticString/StaticString.hpp"
#include "../../../StringSet/StringSet.hpp"

#ifndef CSE_FILE_SOURCE_HPP
#define CSE_FILE_SOURCE_HPP
/**
 * @brief A class that will save
 * and load words from and to a file
 *
 */
class FileSource {
private:
/**
 * @brief relative path for word files
 * 
 */
    static inline std::string mRelativePath = "../database/";

public:

/**
 * @brief Set the Relative Path object
 * 
 * @param path Path to set
 */
static void set_relative_path(const std::string& path) {
    mRelativePath = path;
}
/**
 * @brief Loads words from a file into a StringSet
 *
 * @param filename Name of a file to load words from
 * @return cse::StringSet set of words loaded from a file
 */
static cse::StringSet<cse::StaticString<30>> load_file(const std::string& filename);

/**
 * @brief Saves words into a file from a list
 *
 * @param filename Filename the set will be saved into
 * @param set The set that will be saved
 * @return True if successful, false otherwise
 */
static bool save_file(const std::string& filename,
               cse::StringSet<cse::StaticString<30>> set);
};  // namespace FileSource

#endif  // CSE_FILE_SOURCE_HPP