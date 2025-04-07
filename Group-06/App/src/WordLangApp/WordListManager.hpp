#ifndef CSE_WORDLISTMANAGER_HPP_
#define CSE_WORDLISTMANAGER_HPP_

#include <cassert>
#include <cstring>
#include <iomanip>
#include <iterator>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include "../../../StringSet/StringSet.hpp"
#include "../../../StaticString/StaticString.hpp"
#include "FileSource.hpp"
namespace cse {
 
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

  // Combined function to combine group of sets this function can be variadic templated since we do not know the number of list to combine or can take a name to assign and vector of lists to combine

  // Difference function similar to combined

  // Intersection function similar to combined

  // Copy copys an existing list to a new list and creats it

  // Create list this is a helper

  // Set current

  // Add adds the words to the set

  // Save saves the list into a file if exist overwrite if not creates it.

  // Length function retricst the length of words to search

  // Contains any updates the current list to the restriction

  // Contains all updates the curernt list to the restriction

  // Not Contains update the current list to the restriction

  // Get searches based on pattern load found words into current set to print if asked 

  // Print prints the current setif number specified that number of variable if not all 
  bool print(int number = 0, bool isAll = false);
  
  // Reset takes a listname existing and loads the file associated to it to the original 

  // Reset last remove last applied pattern

  // Wordle search based on this input narrow the set based on input.

  private:
    // A struct has copy of map StringSet to track changes if reset last called this is replaced with current changes to retrive.
  
    // A map of StringSet represents different files
    std::unordered_map<std::string, cse::StringSet<cse::StaticString<20>>> mWordLists;

    // current list
    std::string mCurrentList;

    // A stringSet represents current Get command
    
    // Length restriction 

    // A map of listnames and associated filenames, if reset called resets based on this information. Combine list can not be resetted. We may change it later
 };
}

#endif  // CSE_WORDLISTMANAGER_HPP_