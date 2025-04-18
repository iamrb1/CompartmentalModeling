#include "WordListManager.hpp"
#include "FileSource.hpp"

cse::WordListManager::WordListManager(cse::ErrorManager& errorManager) : mErrorManager(errorManager) {

}

bool cse::WordListManager::loadList(const std::string& listName, const std::string& fileName) {

    if(mWordLists.find(listName) != mWordLists.end())
    {
        mErrorManager.printInfo("Invalid List Name: \"" + listName + "\" already exists.");
        return false;
    }

    cse::StringSet<cse::StaticString<30>> set = FileSource::load_file(fileName);
    if(set.size() == 0) {
        mErrorManager.printInfo("Error : File can not be loaded.");
        return false;
    }

    std::cout << "Loaded " << "\"" << fileName << "\". Word count in a list: " << set.size() << "\n";
    mCurrentSet = set;
    mWordLists[listName] = std::move(set);

    mCurrentLists.clear();
    mCurrentLists.push_back(listName);

    mFileLists[listName] = fileName; // store filename of a listname to load it again in the future
    return true;
}


bool cse::WordListManager::print(int number, bool isAll) {
    if (mCurrentLists.size() == 0) {
        mErrorManager.printInfo("Error : There is no file exist to be printed.");
        return false;
    }

    int count = 0;
    int limit = isAll ? static_cast<int>(mCurrentSet.size() - 1) : number;

    if(!isAll && static_cast<int>(mCurrentSet.size()) <= number) {
        limit = static_cast<int>(mCurrentSet.size() - 1);
    }

    std::cout << "[";

    for (const auto& word : mCurrentSet) {
        if (!isAll && count > number) break;

        std::cout << word;
        ++count;

        if(count <= limit) {
            std::cout << ", ";
        }
    }
    
    std::cout << "]\n";
    return true;
}


/**
 * @brief Combines multiple lists into one list
 * @param newListName New created list
 * @param listsToCombine List names that needs to be combined
 * @return True if lists exist and successful, false if less than 2 lists provided and one of the lists or both doesn't exist
 */
bool cse::WordListManager::combine(const std::string &newListName, const std::vector<std::string> &listsToCombine)
{
    if(mWordLists.find(newListName) != mWordLists.end())
    {
        mErrorManager.printInfo("Invalid List Name: \"" + newListName + "\" already exists.");
        return false;
    }
    cse::StringSet<cse::StaticString<30>> result;

    for(const auto& list : listsToCombine)
    {
        if(mWordLists.find(list) == mWordLists.end())
        {
            mErrorManager.printInfo("Invalid List Name: List \"" + list + "\" does not exist.");
            return false;
        }

        result = result.Union(mWordLists[list]);
    }

    mCurrentSet = result;
    std::cout << "Number of words to search: " << result.size() << std::endl;

    mWordLists[newListName] = std::move(result);

    mCurrentLists.clear();
    mCurrentLists.push_back(newListName);
    
    return true;
}



/**
 * @brief Difference between first and other lists
 * @param newListName New list based on the difference
 * @param listsToDiff List names that we need for difference
 * @return True if lists exist and successful, false if less than 2 lists and any of them does not exist
 */
bool cse::WordListManager::difference(const std::string &newListName, const std::vector<std::string> &listsToDiff)
{
    if(mWordLists.find(newListName) != mWordLists.end())
    {
        mErrorManager.printInfo("Invalid list Name: \"" + newListName + "\" already exists.");
        return false;
    }

    const auto& firstList = listsToDiff[0];
    if(mWordLists.find(firstList) == mWordLists.end())
    {
        mErrorManager.printInfo("Invalid List Name: List \"" + firstList + "\" does not exist.");
        return false;
    }
    cse::StringSet<cse::StaticString<30>> result = mWordLists[firstList];

    for(std::size_t i = 1; i < listsToDiff.size(); i++)
    {
        const auto& listName = listsToDiff[i];
        if(mWordLists.find(listName) == mWordLists.end())
        {
            mErrorManager.printInfo("Invalid List Name: List \"" + listName + "\" does not exist.");
            return false;
        }
        result = result.difference(mWordLists[listName]);
    }   

    mCurrentSet = result;

    std::cout << "Number of words to search: " << result.size() << std::endl;
    mWordLists[newListName] = std::move(result);

    mCurrentLists.clear();
    mCurrentLists.push_back(newListName);
   
    return true;
}



/**
 * @brief Intersection of lists
 * @param newListName New created list based on intersection
 * @param listsToIntersect List names that we need for intersection
 * @return True if lists exist and successful, false if less than 2 lists and any of them does not exist
 */
bool cse::WordListManager::intersection(const std::string &newListName, const std::vector<std::string> &listsToIntersect)
{
    if(mWordLists.find(newListName) != mWordLists.end())
    {
        mErrorManager.printInfo("Invalid List Name: \"" + newListName + "\" already exists.");
        return false;
    }

    const auto& firstList = listsToIntersect[0];

    if(mWordLists.find(firstList) == mWordLists.end())
    {
        mErrorManager.printInfo("Invalid List Name: List \"" + firstList + "\" does not exist.");
        return false;
    }
    cse::StringSet<cse::StaticString<30>> result = mWordLists[firstList];

    for(std::size_t i = 1; i < listsToIntersect.size(); i++)
    {
        const auto& listName = listsToIntersect[i];
        if(mWordLists.find(listName) == mWordLists.end())
        {
            mErrorManager.printInfo("Invalid list Name: List \"" + listName + "\" does not exist.");
            return false;
        }
        result = result.intersection(mWordLists[listName]);
    }

    mCurrentSet = result;
    std::cout << "Number of words to search: " << result.size() << std::endl;

    mWordLists[newListName] = std::move(result);

    mCurrentLists.clear();
    mCurrentLists.push_back(newListName);
    return true;
}



/**
 * @brief Creates a copy of a list
 * @param newListName New list we created
 * @param copyList List name we need to copy
 * @return True if copyList exists and successful, false if copyList does not exist
 */
bool cse::WordListManager::copy(const std::string &newListName, const std::string &copyList)
{
    if(mWordLists.find(newListName) != mWordLists.end())
    {
        mErrorManager.printInfo("Invalid List Name: \"" + newListName + "\" already exists.");
        return false;
    }

    if(mWordLists.find(copyList) == mWordLists.end())
    {
        mErrorManager.printInfo("Invalid List Name: List \"" + copyList + "\" does not exist.");
        return false;
    }

    mWordLists[newListName] = mWordLists[copyList];
    mCurrentSet = mWordLists[newListName];

    mCurrentLists.clear();
    mCurrentLists.push_back(newListName);
    return true;
}


/**
 * @brief Sets the current list to the specified list names.
 * @param listNames A vector of list names to set as the current list.
 * @return true If the lists were set successfully, false otherwise.
 */
bool cse::WordListManager::setCurrent(const std::vector<std::string>& listNames) {
    cse::StringSet<cse::StaticString<30>> combinedSet;

    for (const auto& listName : listNames) {
        if (mWordLists.find(listName) == mWordLists.end()) {
            return false;
        }
        combinedSet = combinedSet.Union(mWordLists[listName]);
    }

    mCurrentSet = combinedSet;
    mCurrentLists = listNames; // Update the current list names
    return true;
}

/**
 * @brief Adds words to the specified list.
 * @param listName The name of the list to add words to.
 * @param wordsToAdd The words to add, separated by spaces.
 * @return true If the words were added successfully, false otherwise.
 */
bool cse::WordListManager::add(const std::string& listName, const std::string& wordsToAdd) {
    if (mWordLists.find(listName) == mWordLists.end()) {
        return false;
    }
    auto& list = mWordLists[listName];
    std::istringstream iss(wordsToAdd);
    std::string word;
    while (iss >> word) {
        list.insert(word);
    }
    return true;
}
/**
 * @brief Saves the specified list to a file.
 * @param fileName The name of the file to save the list into.
 * @param listName The name of the list to save.
 * @return true If the list was saved successfully, false otherwise.
 */
bool cse::WordListManager::save(const std::string& fileName, const std::string& listName) {
    if (mWordLists.find(listName) == mWordLists.end()) {
        return false;
    }

    const auto& list = mWordLists[listName];
    if (FileSource::save_file(fileName, list)) {
        return true;
    } else {
        return false;
    }
}

void cse::WordListManager::reset(const std::string& listname) {
    if (listname == "") {
        mErrorManager.printInfo("Succesfully reset all current lists to the original state.\n");
        for (const auto& listname : mCurrentLists) {
            mWordLists[listname] = FileSource::load_file(mFileLists[listname]);
            std::cout << "  " << listname << ": " << mWordLists[listname].size() << " words\n";
        }
        std::cout << "\n";
        return;
    }

    if (mWordLists.find(listname) == mWordLists.end()) {
        mErrorManager.printInfo("List " + listname + " does not exist");
        return;
    }

    mWordLists[listname] = FileSource::load_file(mFileLists[listname]);
    mErrorManager.printInfo("Successfully reset " + listname + " to the original state with " + std::to_string(mWordLists[listname].size()) + " words.");    
    
}
/**
 * @brief Sets the length restriction for words in the list.
 * @param lengthRestriction The length restriction to set.
 * @return true Operation was successful
 * @return false Error with lengthRestriction value
 */
bool cse::WordListManager::setLengthRestriction(const std::string& lengthRestriction) {
    // bool hasNumber = false;
    // bool hasStar = false;

    // // Check if the input contains a number
    // try {
    //     int num = std::stoi(lengthRestriction);
    //     hasNumber = true;
    //     if(num > -1) mlengthRestriction = num;
    //     else return false;
    // } catch (const std::invalid_argument&) {
    //     // Not a number, continue checking for '*'
    // }

    // // Check if the input contains a star
    // if (lengthRestriction.find('*') != std::string::npos) {
    //     hasStar = true;
    //     mlengthRestriction = -1;
    // }

    // // If both a number and a star are present, return false
    // if (hasNumber && hasStar) {
    //     return false;
    // }

    // // If neither a number nor a star is present, return false
    // if (!hasNumber && !hasStar) {
    //     return false;
    // }

    // return true;

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
        std::cout << "Words before filter: " << count_before << ", after filter: " << count_after << "\n";
        // Update mCurrentSet too.
        mCurrentSet = std::move(result);
        return true;
    }

    // if we have a star - restore all lists
    for (const auto& listname : mCurrentLists) {
        mWordLists[listname] = FileSource::load_file(mFileLists[listname]);
        std::cout << "Restored all Lists" << "\n";
    }
    return true;
}

/**
 * @brief updates the current list to the restriction, string collection of letters to include some return bool.
 * @param lettersToCheck The letters to check in the word list.
 * @return The updated list.
 */
bool cse::WordListManager::ContainsAny(const std::string &lettersToCheck)
{
  if (lettersToCheck.length() == 0) {
    mErrorManager.printInfo("Incorrect Syntax: no letters given.");
  }
  if (mCurrentSet.size() == 0) {
    return false;
  }

  cse::StringSet<cse::StaticString<30>> result = mCurrentSet.search(".*[" +lettersToCheck + "].*");
  mCurrentSet = result;

  if(mPrintNumberOfWords) {
    std::cout << "Number of words to search: " << mCurrentSet.size() << std::endl;
  }

  return result.size() > 0;
}

/**
 * @brief updates the curernt list to the restriction, string collection of letters to include all return bool.
 * @param lettersToCheck The letters to check in the word list.
 * @return The updated list.
 */
bool cse::WordListManager::ContainsAll(const std::string &lettersToCheck)
{
    if (lettersToCheck.length() == 0) {
      mErrorManager.printInfo("Incorrect Syntax: no letters given.");
    }
    if (mCurrentSet.size() == 0) {
      return false;
    }

    std::string lettersToCheckNew = "^";
    for (size_t i = 0; i < lettersToCheck.length(); i++) {   
        std::string tempString = "(?=.*";
        tempString += lettersToCheck[i];
        tempString += ")";
        lettersToCheckNew += tempString;

    }
    lettersToCheckNew += ".*";

    cse::StringSet<cse::StaticString<30>> result = mCurrentSet.search(lettersToCheckNew);
    mCurrentSet = result;

    if(mPrintNumberOfWords) {
        std::cout << "Number of words to search: " << mCurrentSet.size() << std::endl;
    }
    return result.size() > 0;
}

/**
 * @brief Not Contains update the current list to the restriction, string collection of letters to exclude return bool
 * @param lettersToCheck The letters to check in the word list.
 * @return The updated list.
 */
bool cse::WordListManager::NotContains(const std::string &lettersToCheck)
{
    //If there aren't any patterns being checked, print an error message
    if (lettersToCheck.length() == 0) {
      mErrorManager.printInfo("Incorrect Syntax: no letters to check for provided.");
    }
    //Don't perform any opperations if there are no current lists
    if (mCurrentSet.size() == 0) {
      return false;
    }

    cse::StringSet<cse::StaticString<30>> result = mCurrentSet.search("^[^" +lettersToCheck + "]*$");
    mCurrentSet = result;
    
    if(mPrintNumberOfWords) {
        std::cout << "Number of words to search: " << mCurrentSet.size() << std::endl;
    }
    return result.size() > 0;
}

/**
 * @brief Get searches based on pattern load found words into current set to print if asked, string pattern to restric return bool
 * @param patternToCheck the pattern to check
 * @return The updated list
 */
bool cse::WordListManager::Get(const std::string &patternToCheck)
{
    //If there aren't any patterns being checked, print an error message
    if (patternToCheck.length() == 0) {
      mErrorManager.printInfo("Incorrect Syntax: no letters to check for provided");
    }

    //Don't perform any opperations if there are no current lists
    if (mCurrentSet.size() == 0) {
      return false;
    }

    std::string patternToCheckNew = "";
    for (size_t i = 0; i < patternToCheck.length(); i++) {
      if (patternToCheck[i] != '_' && patternToCheck[i] != '*') {
        patternToCheckNew += patternToCheck[i];
      } else if (patternToCheck[i] == '_') {
        patternToCheckNew += ".";
      } else if (patternToCheck[i] == '*') {
        patternToCheckNew += ".*";
      }
    }
    cse::StringSet<cse::StaticString<30>> result = mCurrentSet.search(patternToCheckNew);
    mCurrentSet = result;
    if(mPrintNumberOfWords) {
        std::cout << "Number of words to search: " << mCurrentSet.size() << std::endl;
    }
    
    return result.size() > 0;
}


bool cse::WordListManager::wordle(const std::string& word, const std::string& result) {
    
    if(word.size() != result.size()) {
        mErrorManager.printInfo("Error : Word and matched pattern size must be same.");
        return false;
    }

    if(word.size() != 5 || result.size() != 5) {
        mErrorManager.printInfo("Error : Word and matched pattern size must be equal to 5.");
        return false;
    }

    std::unordered_set<char> lettersToNotContainsSet;  // Letters marked 'b' in the result.
    std::unordered_set<char> lettersToContainSet;      // Letters marked 'g' or 'y' means those letters must be in the word.
    std::string patternOfGreen = "";        // Letters marked 'g' where that letter exactly must be in that place.
    std::unordered_map<char, size_t> lettersIndexes; // Holds the character marked yellow and its index.


    for(size_t i=0; i<result.length(); i++) {
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
    }

    mPrintNumberOfWords = false;

    std::string lettersToNotContains(lettersToNotContainsSet.begin(), lettersToNotContainsSet.end());
    std::string lettersToContain(lettersToContainSet.begin(), lettersToContainSet.end());

    // Now we remove all the letters marked 'b' from the wordle in our search set
    if(!lettersToNotContains.empty()) {
        NotContains(lettersToNotContains);
    }
    
    // We make sure only the words that contains letter marked green or yellow in the search set
    if(!lettersToContain.empty()) {
        ContainsAll(lettersToContain);
    }
    // Filter set with a green in position with get
    Get(patternOfGreen);

    mPrintNumberOfWords = true;

    // Create a result set that where green mark is in the same index for all words and words doesn't contain black
    // marked letters, also, yellow marked letters are not in the wrong indexes.
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


    // Might sort or request 
    
    // Update mCurrentSet with the new filtered list.
    mCurrentSet.clear();
    mCurrentSet = possibleOptions;

    // Display the top 10 or fewer matching words. Might do with random in stringset for printing,
    print(10);

    return true;
}