#ifndef CSE_WORDLISTMANAGER_HPP_
#define CSE_WORDLISTMANAGER_HPP_

#include <cassert>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <fstream>
#include "../../../StringSet/StringSet.hpp"
#include "../../../StaticString/StaticString.hpp"
#include "FileSource.hpp"

namespace cse {

/**
 * @brief WordListManager handles the backend of WordLang. 
 */
class WordListManager {
public:
    /**
     * @brief Initializes a list of words from a file
     * 
     * @param listName Name of user's list
     * @param fileName Filename
     * @return true List loaded successfully
     * @return false List has not been loaded (file not found)
     */
    bool load_list(const std::string& listName, const std::string& fileName);

    // Combined function to combine group of sets, string listname, vector of lists to combine return bool
    bool combine(const std::string& newListName, const std::vector<std::string>& listsToCombine);

    // Difference function similar to combined, listname, vector of listnames returns a bool
    bool difference(const std::string& newListName, const std::vector<std::string>& listsToDiff);

    // Intersection function similar to combined, listname, vector of listnames returns a bool
    bool intersection(const std::string& newListName, const std::vector<std::string>& listsToIntersect);

    // Copy copies an existing list to a new list and creates it, listname, listname to copy return bool
    bool copy(const std::string& newListName, const std::string& copyList);

    /**
     * @brief Sets the current list to the specified list name.
     * @param listName The name of the list to set as current.
     * @return true If the list was set successfully false otherwise.
     */
    bool setCurrent(cse::StringSet<cse::StaticString<20>> currentSet = cse::StringSet<cse::StaticString<20>>());

    // Add adds the words to the set, listname, space-separated words to add returns bool
    bool add(const std::string& listName, const std::string& wordsToAdd);

    // Save saves the list into a file if it exists overwrite if not creates it. Listname return bool
    bool save(const std::string& listName);

    // Contains any updates the current list to the restriction, string collection of letters to include some return bool
    bool ContainsAny(const std::string &lettersToCheck);

    // Contains all updates the curernt list to the restriction, string collection of letters to include all return bool
    bool ContainsAll(const std::string &lettersToCheck);

    /** 
     * @brief Sets the length restriction for the current list.
     * @param number The length restriction to set. Default is 0, which means no restriction.
     * @return int The length restriction set.
     */
    int setLengthRestriction(int lengthRestriction = 0);

    /**
     * @brief Prints the current values
     * 
     * @param number Int number of values to print, by default 0
     * @param isAll Bool flag to identify if user wants all inputs
     * @return true If print is successful
     * @return false If print is unsuccessful
     */
    bool print(int number = 0, bool isAll = false);

private:
    // A map of StringSet represents different files
    std::unordered_map<std::string, cse::StringSet<cse::StaticString<20>>> mWordLists;

    // Current list
    std::string mCurrentList;

    // A StringSet represents current Get command
    cse::StringSet<cse::StaticString<20>> mCurrentSet;

    // Length restriction 
    int mlengthRestriction;
};

}  // namespace cse

#endif  // CSE_WORDLISTMANAGER_HPP_