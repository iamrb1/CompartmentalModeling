#include "WordListManager.hpp"

cse::WordListManager::WordListManager(cse::ErrorManager& errorManager) : mErrorManager(errorManager) {

}

bool cse::WordListManager::loadList(const std::string& listName, const std::string& fileName) {

    if(mWordLists.find(listName) != mWordLists.end())
    {
        mErrorManager.printInfo("Invalid List Name: \"" + listName + "\" already exists.");
        return false;
    }

    cse::StringSet<cse::StaticString<20>> set = FileSource::load_file(fileName);
    if(set.size() == 0) {
        mErrorManager.printInfo("Error : File can not be loaded.");
        return false;
    }

    std::cout << "Loaded " << "\"" << fileName << "\". Word count in a list: " << set.size() << "\n";
    mCurrentSet = set;
    mWordLists[listName] = std::move(set);

    mCurrentLists.clear();
    mCurrentLists.push_back(listName);
    // TODO Decide if we want to store mCurrentLists as values or names and what we want to mCurrentSet
    return true;
}


bool cse::WordListManager::print(int number, bool isAll) {
    if (mCurrentLists.size() == 0) {
        mErrorManager.printInfo("Error : There is no file exist to be printed.");
        return false;
    }

    int count = 0;
    int limit = isAll ? static_cast<int>(mCurrentSet.size()) : number;

    std::cout << "[";

    for (const auto& word : mCurrentSet) {
        if (!isAll && count > number) break;

        std::cout << word;
        ++count;

        if(count < limit) {
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
        mErrorManager.printInfo("Invalid File Name: \"" + newListName + "\" already exists.");
        return false;
    }
    cse::StringSet<cse::StaticString<20>> result;

    for(const auto& list : listsToCombine)
    {
        if(mWordLists.find(list) == mWordLists.end())
        {
            mErrorManager.printInfo("Invalid File Name: File \"" + list + "\" does not exist.");
            return false;
        }

        result = result.Union(mWordLists[list]);
    }
    mWordLists[newListName] = result;

    mCurrentLists.clear();
    mCurrentLists.push_back(newListName);
    std::cout << "Number of words to search: " << result.size() << std::endl;
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
        mErrorManager.printInfo("Invalid File Name: \"" + newListName + "\" already exists.");
        return false;
    }

    const auto& firstList = listsToDiff[0];
    if(mWordLists.find(firstList) == mWordLists.end())
    {
        mErrorManager.printInfo("Invalid File Name: File \"" + firstList + "\" does not exist.");
        return false;
    }
    cse::StringSet<cse::StaticString<20>> result = mWordLists[firstList];

    for(std::size_t i = 1; i < listsToDiff.size(); i++)
    {
        const auto& listName = listsToDiff[i];
        if(mWordLists.find(listName) == mWordLists.end())
        {
            mErrorManager.printInfo("Invalid File Name: File \"" + listName + "\" does not exist.");
            return false;
        }
        result = result.difference(mWordLists[listName]);
    }
    mWordLists[newListName] = result;

    mCurrentLists.clear();
    mCurrentLists.push_back(newListName);
    std::cout << "Number of words to search: " << result.size() << std::endl;
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
        mErrorManager.printInfo("Invalid File Name: \"" + newListName + "\" already exists.");
        return false;
    }

    const auto& firstList = listsToIntersect[0];

    if(mWordLists.find(firstList) == mWordLists.end())
    {
        mErrorManager.printInfo("Invalid File Name: File \"" + firstList + "\" does not exist.");
        return false;
    }
    cse::StringSet<cse::StaticString<20>> result = mWordLists[firstList];

    for(std::size_t i = 1; i < listsToIntersect.size(); i++)
    {
        const auto& listName = listsToIntersect[i];
        if(mWordLists.find(listName) == mWordLists.end())
        {
            mErrorManager.printInfo("Invalid File Name: File \"" + listName + "\" does not exist.");
            return false;
        }
        result = result.intersection(mWordLists[listName]);
    }
    mWordLists[newListName] = result;

    mCurrentLists.clear();
    mCurrentLists.push_back(newListName);
    std::cout << "Number of words to search: " << result.size() << std::endl;
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
        mErrorManager.printInfo("Invalid File Name: \"" + newListName + "\" already exists.");
        return false;
    }

    if(mWordLists.find(copyList) == mWordLists.end())
    {
        mErrorManager.printInfo("Invalid File Name: File \"" + copyList + "\" does not exist.");
        return false;
    }
    mWordLists[newListName] = mWordLists[copyList];
    mCurrentLists.clear();
    mCurrentLists.push_back(newListName);
    return true;
}


/**
 * @brief Sets the current list to the specified list name.
 * @param listName The name of the list to set as current.
 * @return true If the list was set successfully, false otherwise.
 */
bool cse::WordListManager::setCurrent(cse::StringSet<cse::StaticString<20>> currentSet) { 
    if (currentSet == cse::StringSet<cse::StaticString<20>>()) {
        return false;
    }
    mCurrentSet = currentSet;
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
 * @brief Saves the current list to a file.
 * @param listName The name of the list to save.
 * @return true If the list was saved successfully, false otherwise.
 */
bool cse::WordListManager::save(const std::string& listName) {
    if (mWordLists.find(listName) == mWordLists.end()) {
        return false;
    }
    auto& list = mWordLists[listName];
    std::ofstream file(listName + ".txt");
    if (!file.is_open()) {
        return false;
    }
    for (const auto& word : list) {
        file << word << "\n";
    }
    file.close();
    return true;
}
/**
 * @brief Sets the length restriction for words in the list.
 * @param lengthRestriction The length restriction to set.
 * @return true Operation was successful
 * @return false Error with lengthRestriction value
 */
bool cse::WordListManager::setLengthRestriction(const std::string& lengthRestriction) {
    int num = 0;
    try {
        num = std::stoi(lengthRestriction);
        if (num <= 0) return false;
    }
    catch (...) {
        return false;
    }
    // Add current lists to the current set
    for (const auto& listname : mCurrentLists) {
        // Ivan: Sorry, I haven't figured out how to properly copy StringSet ??
        cse::StringSet<StaticString<20>> current_set = mWordLists[listname];
        auto vec = current_set.to_vector();
        mCurrentSet.insert(vec);
    }

    // Filter the whole set by length
    mCurrentSet.size_filter(num);
    std::cout << "Current number of words: " << mCurrentSet.size() << "\n";

    return true;

    // Ivan: I don't understand what these two lines were suppose to do
    //mlengthRestriction = lengthRestriction;
    //return lengthRestriction;
}

/**
 * @brief updates the current list to the restriction, string collection of letters to include some return bool.
 * @param lettersToCheck The letters to check in the word list.
 * @return The updated list.
 */
bool cse::WordListManager::ContainsAny(const std::string &lettersToCheck)
{
  StringSet result = mCurrentSet.search("[" +lettersToCheck + "]");
  mCurrentSet = result;
  return result.size() > 0;
}

/**
 * @brief updates the curernt list to the restriction, string collection of letters to include all return bool.
 * @param lettersToCheck The letters to check in the word list.
 * @return The updated list.
 */
bool cse::WordListManager::ContainsAll(const std::string &lettersToCheck)
{
    std::string lettersToCheckNew = "";
    for (size_t i = 0; i < lettersToCheck.length(); i++)
    {
        lettersToCheckNew += lettersToCheck[i];
        if (i < lettersToCheck.length() - 1)
        {
            lettersToCheckNew += "+";
        }
    }
    StringSet result = mCurrentSet.search(lettersToCheckNew);
    mCurrentSet = result;
    return result.size() > 0;
}

/**
 * @brief Not Contains update the current list to the restriction, string collection of letters to exclude return bool
 * @param lettersToCheck The letters to check in the word list.
 * @return The updated list.
 */
bool cse::WordListManager::NotContains(const std::string &lettersToCheck)
{
  StringSet result = mCurrentSet.search("[^" +lettersToCheck + "]");
  mCurrentSet = result;
  return result.size() > 0;
}

/**
 * @brief Get searches based on pattern load found words into current set to print if asked, string pattern to restric return bool
 * @param patternToCheck the pattern to check
 * @return The updated list
 */
bool cse::WordListManager::Get(const std::string &patternToCheck)
{
    std::string patternToCheckNew = "";
    for (size_t i = 0; i < patternToCheck.length(); i++)
    {
      //lettersToCheckNew += lettersToCheck;
      if (patternToCheck[i] != '_')
      {
        patternToCheckNew += patternToCheck[i];
      }
    }
    StringSet result = mCurrentSet.search(patternToCheckNew);
    mCurrentSet = result;
    return result.size() > 0;
}
