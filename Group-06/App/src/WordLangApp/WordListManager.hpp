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
 /**
  * @brief WordListManager handles the backend of WordLang. 
  * 
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


     // Difference function similar to combined, listname, vector of listnames reutnrs a bool
     bool difference(const std::string& newListName, const std::vector<std::string>& listsToDiff);


     // Intersection function similar to combined, listname, vector of listnames reutnrs a bool
     bool intersection(const std::string& newListName, const std::vector<std::string>& listsToIntersect);


     // Copy copys an existing list to a new list and creats it, listname, listname to copy rturn bool
     bool copy(const std::string& newListName, const std::string& copyList);


     // Create list this is a helper

  // -Emil
  /**
   * @brief Sets the current list to the specified list name.
   * @param listName The name of the list to set as current.
   * @return true If the list was set successfully false otherwise.
   */
  bool setCurrent(cse::StringSet<cse::StaticString<20>> currentSet = cse::StringSet<cse::StaticString<20>>()) {
    if(currentSet == cse::StringSet<cse::StaticString<20>>()) return false; // Update this once we can track the most recent file loaded
    mCurrentSet = currentSet;
    return true;
  }
  // Add adds the words to the set, listname, space separated words to add returns bool

  // Save saves the list into a file if exist overwrite if not creates it. Listname return bool

  /** 
   * @brief Sets the length restriction for the current list.
   * @param number The length restriction to set. Default is 0, which means no restriction.
   * @return int The length restriction set.
  */
   int setLengthRestriction(int lengthRestriction = 0) {

    mlengthRestriction = lengthRestriction;
    return lengthRestriction;
   }

  // Contains any updates the current list to the restriction, string collection of letters to include some return bool

  // Contains all updates the curernt list to the restriction, string collection of letters to include all return bool

  // Not Contains update the current list to the restriction, string collection of letters to exclude return bool

  // Get searches based on pattern load found words into current set to print if asked, string pattern to restric return bool

  /**
   * @brief Prints the current values
   * 
   * @param number Int number of value to print, by default 0
   * @param isAll Bool flag to identify if user wants all inputs
   * @return true If print is successful
   * @return false If print is unsuccessful
   */
  bool print(int number = 0, bool isAll = false);
  
  // Reset takes a listname existing and loads the file associated to it to the original, listname resets back to originial(can be done only list loaded from files)

  // Reset last remove last applied pattern, remove last applied change return bool

  // Wordle search based on this input narrow the set based on input.

  private:
    // A struct has copy of map StringSet to track changes if reset last called this is replaced with current changes to retrive.
  
    // A map of StringSet represents different files
    std::unordered_map<std::string, cse::StringSet<cse::StaticString<20>>> mWordLists;

    // current list
    std::string mCurrentList;

    // A stringSet represents current Get command
    cse::StringSet<cse::StaticString<20>> mCurrentSet;
    // Length restriction 
    int mlengthRestriction;
    // A map of listnames and associated filenames, if reset called resets based on this information. Combine list can not be resetted. We may change it later
 };
}

#endif  // CSE_WORDLISTMANAGER_HPP_