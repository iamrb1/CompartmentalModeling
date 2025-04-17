#include "FileSource.hpp"
#include "../../../StaticString/StaticString.hpp"
#include <fstream>

namespace FileSource {
    cse::StringSet<cse::StaticString<30>> load_file(const std::string& filename) {
        std::ifstream file("../database/" + filename);
        cse::StringSet<cse::StaticString<30>> set;
        std::string line;
    
        while (std::getline(file, line)) {
            cse::StaticString<30> word = line;
            set.insert(word);
        }
    
        return set;
    }

    bool save_file(const std::string& filename, cse::StringSet<cse::StaticString<30>> set) {
        std::ofstream file("../database/" + filename);
        for (const auto& word : set) {
            file << word << "\n";
        }

        file.close();
        return true;
    }
}