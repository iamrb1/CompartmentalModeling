#include "FileSource.hpp"
#include "../../../StaticString/StaticString.hpp"
#include <fstream>
cse::StringSet<cse::StaticString<20>> FileSource::load_file(const std::string& filename) {
    std::ifstream file("../database/" + filename);
    cse::StringSet<cse::StaticString<20>> set;
    std::string line;

    while (std::getline(file, line)) {
        cse::StaticString<20> word = line;
        set.insert(word);
    }

    return set;
}