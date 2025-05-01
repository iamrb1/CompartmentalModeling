/**
 * @file WordListManager.cpp
 * @author Mehmet Efe Caylan
 * @author Will Crawford
 * @author Ivan Bega
 * @author Orhan Aydin
 * @author Emil Rahn Siegel
 * @brief WordListManager implementation file.
 * @version 0.1
 * @date 2025-04-19
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "WordListManager.hpp"

#include "../../../StaticString/StaticString.hpp"
#include "../../../StringSet/StringSet.hpp"
#include "FileSource.hpp"

cse::WordListManager::WordListManager(cse::ErrorManager& errorManager)
    : mErrorManager(errorManager) {}

bool cse::WordListManager::loadList(const std::string& listName,
                                    const std::string& fileName) {
  if (mWordLists.find(listName) != mWordLists.end()) {
    mErrorManager.printInfo("Invalid List Name: \"" + listName +
                            "\" already exists.");
    return false;
  }

  cse::StringSet<cse::StaticString<30>> set = FileSource::load_file(fileName);
  if (set.size() == 0) {
    mErrorManager.printInfo("Error : File can not be loaded.");
    return false;
  }

  std::cout << "Loaded "
            << "\"" << fileName << "\". Word count in a list: " << set.size()
            << "\n";
  mCurrentSet = set;
  mWordLists[listName] = std::move(set);

  mCurrentLists.clear();  // Clear and set this list as current list ot work on
  mCurrentLists.push_back(listName);

  mFileLists[listName] =
      fileName;  // store filename of a listname to load it again in the future
  return true;
}

bool cse::WordListManager::print(int number, bool isAll) {
  if (mCurrentLists.size() == 0) {
    mErrorManager.printInfo("Error : There is no file exist to be printed.");
    return false;
  }

  int count = 0;
  int limit = isAll ? static_cast<int>(mCurrentSet.size() - 1) : number - 1;

  // Eliminate trailing comma at the end
  if (!isAll && static_cast<int>(mCurrentSet.size()) <= number) {
    limit = static_cast<int>(mCurrentSet.size() - 1);
  }

  std::cout << "[";

  for (const auto& word : mCurrentSet) {
    if (!isAll && count > limit) break;

    std::cout << word;
    ++count;

    if (count <= limit) {
      std::cout << ", ";
    }
  }

  std::cout << "]\n";
  return true;
}

bool cse::WordListManager::combine(
    const std::string& newListName,
    const std::vector<std::string>& listsToCombine) noexcept {
  // check if newListName is unique
  if (mWordLists.find(newListName) != mWordLists.end()) {
    mErrorManager.printInfo("Invalid List Name: \"" + newListName +
                            "\" already exists.");
    return false;
  }
  cse::StringSet<cse::StaticString<30>> result;

  // loop through listsTocombine and call Union on result
  for (const auto& list : listsToCombine) {
    if (mWordLists.find(list) == mWordLists.end()) {
      mErrorManager.printInfo("Invalid List Name: List \"" + list +
                              "\" does not exist.");
      return false;
    }

    for (const auto& word: mWordLists[list]) {
      result.insert(word);
    }
  }

  // update the current set
  mCurrentSet = result;
  std::cout << "Number of words to search: " << result.size() << std::endl;

  mWordLists[newListName] = std::move(result);

  // set the resulting list as current
  mCurrentLists.clear();
  mCurrentLists.push_back(newListName);

  return true;
}

bool cse::WordListManager::difference(
    const std::string& newListName,
    const std::vector<std::string>& listsToDiff) noexcept {
  // check if newListName is unique
  if (mWordLists.find(newListName) != mWordLists.end()) {
    mErrorManager.printInfo("Invalid list Name: \"" + newListName +
                            "\" already exists.");
    return false;
  }

  // check if all listnames that user provided us with exist
  const auto& firstList = listsToDiff[0];
  if (mWordLists.find(firstList) == mWordLists.end()) {
    mErrorManager.printInfo("Invalid List Name: List \"" + firstList +
                            "\" does not exist.");
    return false;
  }
  cse::StringSet<cse::StaticString<30>> result = mWordLists[firstList];

  // take difference from appropriate lists and store it in the result
  for (std::size_t i = 1; i < listsToDiff.size(); i++) {
    const auto& listName = listsToDiff[i];
    if (mWordLists.find(listName) == mWordLists.end()) {
      mErrorManager.printInfo("Invalid List Name: List \"" + listName +
                              "\" does not exist.");
      return false;
    }
    result = result.difference(mWordLists[listName]);
  }

  // update current list and display info to the user
  mCurrentSet = result;
  std::cout << "Number of words to search: " << result.size() << std::endl;
  mWordLists[newListName] = std::move(result);

  // update current list with the result
  mCurrentLists.clear();
  mCurrentLists.push_back(newListName);

  return true;
}

bool cse::WordListManager::intersection(
    const std::string& newListName,
    const std::vector<std::string>& listsToIntersect) noexcept {
  // check if newListName is unique
  if (mWordLists.find(newListName) != mWordLists.end()) {
    mErrorManager.printInfo("Invalid List Name: \"" + newListName +
                            "\" already exists.");
    return false;
  }

  const auto& firstList = listsToIntersect[0];
  // check if listsToIntersect exist
  if (mWordLists.find(firstList) == mWordLists.end()) {
    mErrorManager.printInfo("Invalid List Name: List \"" + firstList +
                            "\" does not exist.");
    return false;
  }
  cse::StringSet<cse::StaticString<30>> result = mWordLists[firstList];

  // intersect lists accordingly
  for (std::size_t i = 1; i < listsToIntersect.size(); i++) {
    const auto& listName = listsToIntersect[i];
    if (mWordLists.find(listName) == mWordLists.end()) {
      mErrorManager.printInfo("Invalid list Name: List \"" + listName +
                              "\" does not exist.");
      return false;
    }
    result = result.intersection(mWordLists[listName]);
  }

  // update current set and current lists
  mCurrentSet = result;
  std::cout << "Number of words to search: " << result.size() << std::endl;

  mWordLists[newListName] = std::move(result);

  mCurrentLists.clear();
  mCurrentLists.push_back(newListName);
  return true;
}

bool cse::WordListManager::copy(const std::string& newListName,
                                const std::string& copyList) noexcept {
  // check if newListName is unique
  if (mWordLists.find(newListName) != mWordLists.end()) {
    mErrorManager.printInfo("Invalid List Name: \"" + newListName +
                            "\" already exists.");
    return false;
  }

  // check if copyList is unique
  if (mWordLists.find(copyList) == mWordLists.end()) {
    mErrorManager.printInfo("Invalid List Name: List \"" + copyList +
                            "\" does not exist.");
    return false;
  }

  // perform copy and update current set/list
  mWordLists[newListName] = mWordLists[copyList];
  mCurrentSet = mWordLists[newListName];

  mCurrentLists.clear();
  mCurrentLists.push_back(newListName);
  return true;
}

bool cse::WordListManager::setCurrent(
    const std::vector<std::string>& listNames) noexcept {
  cse::StringSet<cse::StaticString<30>> combinedSet;

  for (const auto& listName : listNames) {
    if (mWordLists.find(listName) == mWordLists.end()) {
      mErrorManager.printInfo("List '" + listName + "' does not exist");
      return false;
    }
    combinedSet = combinedSet.Union(
        mWordLists[listName]);  // add all words from current lists together
  }

  mCurrentSet = std::move(combinedSet);
  mCurrentLists = listNames;  // Update the current list names
  return true;
}

bool cse::WordListManager::add(const std::string& listName,
                               const std::string& wordsToAdd) noexcept {
  // check if list exists
  if (mWordLists.find(listName) == mWordLists.end()) {
    mErrorManager.printInfo("List '" + listName + "' does not exist");
    return false;
  }
  auto& list = mWordLists[listName];
  std::istringstream iss(wordsToAdd);
  std::string word;
  // add words to list
  while (iss >> word) {
    list.insert(word);
  }
  mErrorManager.printInfo("Words added to list '" + listName + "'");
  return true;
}

bool cse::WordListManager::save(const std::string& fileName,
                                const std::string& listName) noexcept {
  // check if list exists
  if (mWordLists.find(listName) == mWordLists.end()) {
    mErrorManager.printInfo("List '" + listName + "' does not exist");
    return false;
  }

  const auto& list = mWordLists[listName];
  // save file
  if (FileSource::save_file(fileName, list)) {
    mErrorManager.printInfo("List '" + listName + "' saved to " + +"'" +
                            fileName + "'");
    return true;
  } else {
    mErrorManager.printInfo("List '" + listName + "' can not saved to " + +"'" +
      fileName + "'");
    return false;
  }
}

bool cse::WordListManager::setLengthRestriction(
    const std::string& lengthRestriction) noexcept {
  // we are guaranteed by the parser to have either a star
  // or a string that will be converted to a positive integer

  // if we have star - remove length restriction
  if (lengthRestriction != "*") {
    int num = std::stoi(lengthRestriction);
    int count_before = 0, count_after = 0;

    cse::StringSet<cse::StaticString<30>> result;

    for (const auto& listname : mCurrentLists) {
      count_before += mWordLists[listname].size();
      mWordLists[listname].size_filter(num);
      count_after += mWordLists[listname].size();
      result = result.Union(mWordLists[listname]);
    }

    if (mPrintNumberOfWords) {
      std::cout << "Words before filter: " << count_before
                << ", after filter: " << count_after << "\n";
    }
    // Update mCurrentSet too.
    mCurrentSet = std::move(result);
    return true;
  }

  // if we have a star - restore all lists
  for (const auto& listname : mCurrentLists) {
    
    mWordLists[listname] = FileSource::load_file(mFileLists[listname]);
  }
  return false;
}

void cse::WordListManager::reset(const std::string& listname) {
  // if no listname provided - reset all lists
  if (listname == "") {
    mErrorManager.printInfo(
        "Succesfully reset all current lists to the original state.\n");
    mCurrentSet.clear();
    for (const auto& listname : mCurrentLists) {
      mWordLists[listname] = FileSource::load_file(mFileLists[listname]);
      std::cout << "  " << listname << ": " << mWordLists[listname].size()
                << " words\n";
      mCurrentSet = mCurrentSet.Union(mWordLists[listname]);
    }
    std::cout << "\n";
    return;
  }

  // check if list name provided by the user exists
  if (mWordLists.find(listname) == mWordLists.end()) {
    mErrorManager.printInfo("List " + listname + " does not exist");
    return;
  }

  mWordLists[listname] = FileSource::load_file(mFileLists[listname]);

  mCurrentSet = mWordLists[listname];  // Set as current for the resetted list
  mCurrentLists.clear();
  mCurrentLists.push_back(listname);

  mErrorManager.printInfo(
      "Successfully reset " + listname + " to the original state with " +
      std::to_string(mWordLists[listname].size()) + " words.");
}

bool cse::WordListManager::ContainsAny(const std::string& lettersToCheck) noexcept {
  // If there aren't any letters being checked, print an error message
  if (lettersToCheck.length() == 0) {
    mErrorManager.printInfo("Incorrect Syntax: no letters given.");
    return false;
  }

  // Don't perform any opperations if there are no current lists
  if (mCurrentSet.size() == 0) {
    mErrorManager.printInfo(
        "Error: No lists exists or set as current list to filter.");
    return false;
  }

  cse::StringSet<cse::StaticString<30>> result;

  for (auto list : mCurrentLists) {
    // Update each set individually based on restriction.
    mWordLists[list] = mWordLists[list].search(".*[" + lettersToCheck + "].*");
    result = result.Union(mWordLists[list]);
  }

  mCurrentSet = result;

  if (mPrintNumberOfWords) {
    std::cout << "Number of words to search: " << mCurrentSet.size()
              << std::endl;
  }

  return result.size() > 0;
}

bool cse::WordListManager::ContainsAll(const std::string& lettersToCheck) noexcept {
  // If there aren't any letters being checked, print an error message
  if (lettersToCheck.length() == 0) {
    mErrorManager.printInfo("Incorrect Syntax: no letters given.");
    return false;
  }

  // Don't perform any opperations if there are no current lists
  if (mCurrentSet.size() == 0) {
    mErrorManager.printInfo(
        "Error: No lists exists or set as current list to filter.");
    return false;
  }

  std::string regexPattern = "^";
  for (size_t i = 0; i < lettersToCheck.length(); i++) {
    std::string tempString = "(?=.*";
    tempString += lettersToCheck[i];
    tempString += ")";
    regexPattern += tempString;
  }
  regexPattern += ".*";

  cse::StringSet<cse::StaticString<30>> result;

  for (auto list : mCurrentLists) {
    // Update each set individually based on restriction.
    mWordLists[list] = mWordLists[list].search(regexPattern);
    result = result.Union(mWordLists[list]);
  }

  mCurrentSet = result;

  if (mPrintNumberOfWords) {
    std::cout << "Number of words to search: " << mCurrentSet.size()
              << std::endl;
  }
  return result.size() > 0;
}

bool cse::WordListManager::NotContains(const std::string& lettersToCheck) noexcept {
  // If there aren't any letters being checked, print an error message
  if (lettersToCheck.length() == 0) {
    mErrorManager.printInfo(
        "Incorrect Syntax: no letters to check for provided.");
    return false;
  }
  // Don't perform any opperations if there are no current lists
  if (mCurrentSet.size() == 0) {
    mErrorManager.printInfo(
        "Error: No lists exists or set as current list to filter.");
    return false;
  }

  cse::StringSet<cse::StaticString<30>> result;

  for (auto list : mCurrentLists) {
    // Update each set individually based on restriction.
    mWordLists[list] = mWordLists[list].search("^[^" + lettersToCheck + "]*$");
    result = result.Union(mWordLists[list]);
  }

  mCurrentSet = result;

  if (mPrintNumberOfWords) {
    std::cout << "Number of words to search: " << mCurrentSet.size()
              << std::endl;
  }
  return result.size() > 0;
}

bool cse::WordListManager::Get(const std::string& patternToCheck) noexcept {
  // If there aren't any patterns being checked, print an error message
  if (patternToCheck.length() == 0) {
    mErrorManager.printInfo(
        "Incorrect Syntax: no patterns to check for provided.");
    return false;
  }

  // Don't perform any opperations if there are no current lists
  if (mCurrentSet.size() == 0) {
    mErrorManager.printInfo(
        "Error: No lists exists or set as current list to filter.");
    return false;
  }

  // Start Position of the pattern
  std::string regexPattern = "^";
  for (size_t i = 0; i < patternToCheck.length(); i++) {
    if (patternToCheck[i] != '_' && patternToCheck[i] != '*') {
      regexPattern += patternToCheck[i];
    } else if (patternToCheck[i] == '_') {
      regexPattern += ".";
    } else if (patternToCheck[i] == '*') {
      regexPattern += ".*";
    }
  }

  // End Position of the pattern
  regexPattern += "$";

  cse::StringSet<cse::StaticString<30>> result;

  for (auto list : mCurrentLists) {
    // Update each set individually based on restriction.
    mWordLists[list] = mWordLists[list].search(regexPattern);
    result = result.Union(mWordLists[list]);
  }

  mCurrentSet = result;

  if (mPrintNumberOfWords) {
    std::cout << "Number of words to search: " << mCurrentSet.size()
              << std::endl;
  }

  return result.size() > 0;
}

bool cse::WordListManager::wordle(const std::string& word,
                                  const std::string& result) noexcept {
  if (word.size() != result.size()) {
    mErrorManager.printInfo(
        "Error : Word and matched pattern size must be same.");
    return false;
  }

  // In wordle words must be length of 5
  if (word.size() != 5 || result.size() != 5) {
    mErrorManager.printInfo(
        "Error : Word and matched pattern size must be equal to 5.");
    return false;
  }

  // Letters marked 'g' where that letter exactly must be in that place.
  std::string patternOfGreen = "";  
  // Holds the character marked yellow and its index.
  std::unordered_map<char, size_t> lettersIndexes;  
  // Letters marked 'b' in the result.
  cse::StringSet<char> lettersToNotContainsSet;
  // Letters marked 'g' or 'y' means those letters must be in the word.
  cse::StringSet<char> lettersToContainSet;

  // Construct each restrictions to apply
  for (size_t i = 0; i < result.length(); i++) {
    if (result[i] == 'b') {
      lettersToNotContainsSet.insert(word[i]);
    } else if (result[i] == 'y' || result[i] == 'g') {
      lettersToContainSet.insert(word[i]);
    }

    if (result[i] == 'g') {
      patternOfGreen += word[i];
    } else {
      patternOfGreen += '_';
    }

    if (result[i] == 'y') {
      lettersIndexes[word[i]] = i;
    }

    if (result[i] != 'y' && result[i] != 'b' && result[i] != 'g') {
      mErrorManager.printInfo(
        "Error : Invalid letter identifier used for wordle. You must use one of the yellow 'y', black 'b', green 'g' identifiers.");
        return false;
    }
  }

  // Disable user notficiation prints for temporarily.
  mPrintNumberOfWords = false;

  // Eliminate any multiple occurance of words that marked 'b'
  lettersToNotContainsSet = lettersToNotContainsSet.difference(lettersToContainSet);

  std::string lettersToNotContains(lettersToNotContainsSet.begin(),
                                   lettersToNotContainsSet.end());
  std::string lettersToContain(lettersToContainSet.begin(),
                               lettersToContainSet.end());

  // CALL LENGTH = 5 by default
  setLengthRestriction("5");

  // Now we remove all the letters marked 'b' from the wordle in our search set
  if (!lettersToNotContains.empty()) {
    NotContains(lettersToNotContains);
  }

  // We make sure only the words that contains letter marked green or yellow in
  // the search set
  if (!lettersToContain.empty()) {
    ContainsAll(lettersToContain);
  }
  // Filter set with a green in position with get
  Get(patternOfGreen);

  mPrintNumberOfWords = true;

  // Create a result set that where green mark is in the same index for all
  // words and words doesn't contain black marked letters, also, yellow marked
  // letters are not in the wrong indexes.
  cse::StringSet<cse::StaticString<30>> possibleOptions;

  for (auto word : mCurrentSet) {
    bool valid = true;

    for (std::size_t i = 0; i < word.length(); i++) {
      char ch = word[i];

      if (lettersIndexes.contains(ch)) {
        // If any yellow letter is found in its forbidden index, word not valid
        if (lettersIndexes[ch] == i) {
          valid = false;
          break;
        }
      }
    }

    if (valid) {
      possibleOptions.insert(word);
    }
  }

  // Update mCurrentSet with the new filtered list.
  mCurrentSet.clear();
  mCurrentSet = possibleOptions;

  // Display the top 10 or fewer matching words.
  print(10);

  return true;
}