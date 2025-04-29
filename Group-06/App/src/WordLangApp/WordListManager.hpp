/**
 * @file WordListManager.hpp
 * @author Mehmet Efe Caylan
 * @author Will Crawford
 * @author Ivan Bega
 * @author Orhan Aydin
 * @author Emil Rahn Siegel
 * @brief WordListManager header file.
 * @version 0.1
 * @date 2025-04-19
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef CSE_WORDLISTMANAGER_HPP_
#define CSE_WORDLISTMANAGER_HPP_

#include <cassert>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "../../../ErrorManager/ErrorManager.hpp"
#include "../../../StaticString/StaticString.hpp"
#include "../../../StringSet/StringSet.hpp"
#include "FileSource.hpp"

namespace cse {

/**
 * @brief WordListManager handles the backend of WordLang.
 */
class WordListManager {
 public:
  /**
   * @brief Construct a new Word List Manager object
   *
   * @param errorManager
   */
  WordListManager(ErrorManager& errorManager);

  /**
   * @brief Initializes a list of words from a file
   *
   * Loads the words from the given file to the LIST and creats the list with
   * given listname.
   *
   * @attention The file must be located under database directory to be able to
   * opened. Also, the words in the file must be line separated, each line
   * contains only and only one word without a trailing comman or space.
   *
   * @param listName Name of user's list
   * @param fileName Filename filename to be loaded into list
   * @return true List loaded successfully
   * @return false List has not been loaded (file not found)
   */
  bool loadList(const std::string& listName, const std::string& fileName);

  /**
   * @brief Combines multiple lists into one list
   * @param newListName New created list
   * @param listsToCombine List names that needs to be combined
   * @return True and combines listsToCombine if newListName does not already
   * exist and all listsToCombine exist, false otherwise
   */
  bool combine(const std::string& newListName,
               const std::vector<std::string>& listsToCombine) noexcept;

  /**
   * @brief Difference between first and other lists
   * @param newListName New list based on the difference
   * @param listsToDiff List names that we need for difference
   * @return True and computes the difference of listsToDiff if newListName does
   * not already exist and all listToDiff exist, false otherwise
   */
  bool difference(const std::string& newListName,
                  const std::vector<std::string>& listsToDiff) noexcept;

  /**
   * @brief Intersection of lists
   * @param newListName New created list based on intersection
   * @param listsToIntersect List names that we need for intersection
   * @return True and computes the intersection of listsToIntersect if
   * newListName does not already exist and all listsToIntersect exist, false
   * otherwise
   */
  bool intersection(const std::string& newListName,
                    const std::vector<std::string>& listsToIntersect) noexcept;

  /**
   * @brief Creates a copy of a list
   * @param newListName New list we created
   * @param copyList List name we need to copy
   * @return True and copies copyList to newListName if newListName does not
   * already exist and copyList exists, false otherwise
   */
  bool copy(const std::string& newListName, const std::string& copyList) noexcept;

  /**
   * @brief Sets the current list to the specified list names.
   *
   * User can set one or multiple lists as current, and the searches will
   * be based on these list as well as all the filter operations unless
   * new lists are specified.
   *
   * When LIST operation is invoked current lists are resetted and current
   * working list is set to LIST created or loaded currently.
   *
   * @attention Every time SET_CURRENT called with lists current lists are
   * emptied out and assigned to provided lists.
   *
   *
   * @param listNames A vector of list names to set as the current lists.
   * @return true If the list was set successfully, false otherwise.
   */
  bool setCurrent(const std::vector<std::string>& listNames) noexcept;

  /**
   * @brief Adds words to the specified list.
   * @param listName The name of the list to add words to.
   * @param wordsToAdd The words to add, separated by spaces.
   * @return true If the words were added successfully, false otherwise.
   */
  bool add(const std::string& listName, const std::string& wordsToAdd) noexcept;

  /**
   * @brief Saves the specified list to a file.
   * @param fileName The name of the file to save the list into.
   * @param listName The name of the list to save.
   * @return true If the list was saved successfully, false otherwise.
   */
  bool save(const std::string& fileName, const std::string& listName) noexcept;

  /**
   * @brief Updates the current list to the restriction
   *
   * To use contains any provide a string of letters which you do want
   * them to be included in the word.
   *
   * Example:
   * CONTAINS_ANY "aet"  this should filter all the current lists in a way that
   * each word in them must have at least on of these letters one or more times.
   *
   * @param lettersToCheck The letters to check in the word list.
   * @return True if operation is successful, false otherwise.
   */
  bool ContainsAny(const std::string& lettersToCheck) noexcept;

  /**
   * @brief Updates the curernt list to the restriction
   *
   * To use contains all provide a string of letters which you do want
   * them to be included in the word.
   *
   * Example:
   * CONTAINS_ALL "aet"  this should filter all the current lists in a way that
   * each word in them must have all of these letters in them one or more times.
   *
   * @param lettersToCheck The letters to check in the word list.
   * @return True if operation is successful, false otherwise.
   */
  bool ContainsAll(const std::string& lettersToCheck) noexcept;

  /**
   * @brief Not Contains update the current list to the restriction
   *
   * To use not contains provide a string of letters which you do not want
   * them to be included in the word.
   *
   * Example:
   * NOT_CONTAINS "aet"  this should filter all the current lists in a way that
   * each word in them must not have any of these letters.
   *
   * @param lettersToCheck The letters to check in the word list.
   * @return True if operation is successful, false otherwise.
   */
  bool NotContains(const std::string& lettersToCheck) noexcept;

  /**
   * @brief Get searches based on pattern load found words into current set to
   * print if asked
   *
   * To use get you need to provide a pattern to be searched.
   * '_' is used to indicate a single character,
   * '*' is used to indicate zero or more any character.
   *
   * Example:
   * GET "_oo*"  should filter words that has 'o' at the second and third index
   * and any charaters after that zero or more.
   * [book, books, foot, football, boots, cool, tooth] etc.
   *
   * @param patternToCheck The pattern to check
   * @return True if operation is successful, false otherwise.
   */
  bool Get(const std::string& patternToCheck) noexcept;

  /**
   * @brief Resets the lists into original state
   *
   * Resets the lists to the original state when they are loaded. 
   * If a specific listname is provided, only resets that list to state where 
   * it is first loaded from the file.
   * If no listname is provided, it resets all lists in the current working 
   * lists.
   * 
   * @param listName
   */
  void reset(const std::string& listName = "");

  /**
   * @brief Sets the length restriction for words in the list.
   * @param lengthRestriction The length restriction to set.
   * @return true Operation was successful
   * @return false Error with lengthRestriction value
   */
  bool setLengthRestriction(const std::string& lengthRestriction) noexcept;

  /**
   * @brief Prints the current values
   *
   * Prints the number of values from the current working lists. If 'ALL' is
   * used rather than a number, print will print all the values.
   *
   * @param number Int number of values to print, by default 0
   * @param isAll Bool flag to identify if user wants all inputs
   * @return true If print is successful
   * @return false If print is unsuccessful
   */
  bool print(int number = 0, bool isAll = false);

  /**
   * @brief Gets the top 10 possible options for the Wordle game
   *
   * User provides the word that has inputted into Wordle and the results
   * recieved from it. If the respective letter is marked black(not found) put
   * 'b' , yellow (in the word but wrong position) 'y', green (correct match)
   * 'g'.
   *
   * Based on these parameters, WORDLE will return you the best 10 options in
   * the database you can input for word of the day in wordle.
   *
   * Example usage:
   *
   * In wordle, we wrote PILOT and in result P='b', I='y', L='b', O='g', T='b'
   * which means 'O' is in the correct position and 'I' is in the word but in
   * wrong position.
   *
   * We ran the command:
   * WORDLE ("pilot","bybgb")
   * Output= [union, infos, inbox, ...]
   *
   * @param word Word that inputted into Wordle
   * @param result Result indications of the word from Wordle
   * @return true If operation is successful
   * @return false otherwise.
   */
  bool wordle(const std::string& word, const std::string& result) noexcept;

 private:
  /// @brief A map of StringSet represents different files
  std::unordered_map<std::string, cse::StringSet<cse::StaticString<30>>>
      mWordLists;

  /// @brief A map that represents listname(key) and its according
  /// filename(value)
  std::unordered_map<std::string, std::string> mFileLists;

  /// @brief Current vector of lists
  std::vector<std::string> mCurrentLists;

  /// @brief A StringSet represents current Get command
  cse::StringSet<cse::StaticString<30>> mCurrentSet;

  /// @brief Length restriction
  int mlengthRestriction;

  /// @brief ErrorManager that handles error prompting to the user
  cse::ErrorManager& mErrorManager;

  /// @brief Boolean used to indicate program state for word count printing
  bool mPrintNumberOfWords = true;
};

}  // namespace cse

#endif  // CSE_WORDLISTMANAGER_HPP_