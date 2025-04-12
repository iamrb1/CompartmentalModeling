#include "WordListManager.hpp"

bool cse::WordListManager::load_list(const std::string& listName, const std::string& fileName) {

    cse::StringSet<cse::StaticString<20>> set = FileSource::load_file(fileName);
    if(set.size() == 0) {
        return false;
    }

    std::cout << "Loaded " << "\"" << fileName << "\". Word count in a list: " << set.size() << "\n";
    mWordLists[listName] = std::move(set);
    mCurrentList = listName;
    return true;
}


bool cse::WordListManager::print(int number, bool isAll) {
    if (mWordLists.find(mCurrentList) == mWordLists.end()) {
        // Error occured
        return false;
    }

    const auto& list = mWordLists[mCurrentList];
    int count = 0;

    for (const auto& word : list) {
        if (!isAll && count >= number) break;
        std::cout << word << ", ";
        ++count;
    }
    std::cout << "\n";
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
    if(listsToCombine.size() < 2)
    {
        return false;
    }
    cse::StringSet<cse::StaticString<20>> result;

    for(const auto& list : listsToCombine)
    {
        if(mWordLists.find(list) == mWordLists.end())
        {
            return false;
        }

        result = result.Union(mWordLists[list]);
    }
    mWordLists[newListName] = result;
    mCurrentList = newListName;
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
    if(listsToDiff.size() < 2)
    {
        return false;
    }

    const auto& firstList = listsToDiff[0];
    if(mWordLists.find(firstList) == mWordLists.end())
    {
        return false;
    }
    cse::StringSet<cse::StaticString<20>> result = mWordLists[firstList];

    for(std::size_t i = 1; i < listsToDiff.size(); i++)
    {
        const auto& listName = listsToDiff[i];
        if(mWordLists.find(listName) == mWordLists.end())
        {
            return false;
        }
        result = result.difference(mWordLists[listName]);
    }
    mWordLists[newListName] = result;
    mCurrentList = newListName;
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
    if(listsToIntersect.size() < 2)
    {
        return false;
    }
    const auto& firstList = listsToIntersect[0];
    if(mWordLists.find(firstList) == mWordLists.end())
    {
        return false;
    }
    cse::StringSet<cse::StaticString<20>> result = mWordLists[firstList];

    for(std::size_t i = 1; i < listsToIntersect.size(); i++)
    {
        const auto& listName = listsToIntersect[i];
        if(mWordLists.find(listName) == mWordLists.end())
        {
            return false;
        }
        result = result.intersection(mWordLists[listName]);
    }
    mWordLists[newListName] = result;
    mCurrentList = newListName;
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
    if(mWordLists.find(copyList) == mWordLists.end())
    {
        return false;
    }
    mWordLists[newListName] = mWordLists[copyList];
    mCurrentList = newListName;
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

int cse::WordListManager::setLengthRestriction(int lengthRestriction) {
    mlengthRestriction = lengthRestriction;
    return lengthRestriction;
}
