/**
 * @file FileSource.cpp
 * @author Mehmet Efe Caylan
 * @author Will Crawford
 * @author Ivan Bega
 * @author Orhan Aydin
 * @author Emil Rahn Siegel
 * @brief FileSource implementation file.
 * @version 0.1
 * @date 2025-04-19
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "FileSource.hpp"

#include <fstream>

#include "../../../StaticString/StaticString.hpp"

cse::StringSet<cse::StaticString<30>> FileSource::load_file(const std::string& filename) {
  std::ifstream file(mRelativePath + filename);
  cse::StringSet<cse::StaticString<30>> set;
  std::string line;

  while (std::getline(file, line)) {
    cse::StaticString<30> word = line;
    set.insert(word);
  }

  return set;
}

bool FileSource::save_file(const std::string& filename,
               cse::StringSet<cse::StaticString<30>> set) {
  std::ofstream file(mRelativePath + filename);
  for (const auto& word : set) {
    file << word << "\n";
  }

  file.close();
  return true;
}