#include "WordListManager.hpp"

bool cse::WordListManager::load_list(const std::string& listName, const std::string& fileName) {
    cse::StringSet<cse::StaticString<20>> set = FileSource::load_file(fileName);

    std::cout << "Loaded " << "\"" << fileName << "\". Word count in a list: " << set.size() << "\n";
    mWordLists[listName] = std::move(set);
    mCurrentList = listName;
    return true;

}