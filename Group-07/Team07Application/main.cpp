// Anand
#include "../Team07Library/DataFileManager.hpp"
#include <filesystem>
#include <fstream>

string getProjectRoot() {
  std::filesystem::path currentPath = std::filesystem::current_path();
  while (!currentPath.empty() &&
         currentPath.filename() != "CSE498-Spring2025") {
    currentPath = currentPath.parent_path();
  }
  return currentPath.string();
}

int main() {
  cse::DataFileManager dfm;
  string projectRoot = getProjectRoot();
  string filePath = projectRoot + "/Group-07/Data/config.json";
  dfm.openJSON(filePath);
  dfm.closeJSON();
}
