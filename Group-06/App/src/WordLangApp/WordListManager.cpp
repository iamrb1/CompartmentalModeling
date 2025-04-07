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