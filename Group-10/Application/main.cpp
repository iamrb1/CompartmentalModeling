#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "../../Group-06/ArgManager/ArgManager.h"
#include "../../Group-06/CommandLine/CommandLine.cpp"
#include "../Classes/BruteForceOptimizer.hpp"
#include "../Classes/ComboManager.hpp"
#include "../Classes/StaticVector.hpp"

void BruteForceUnoptimized() { std::cout << "Run Unoptimized" << std::endl; }

std::vector<cse::Item> ConstructItems(std::string){ 
    std::cout << "Constructing Items" << std::endl;
    std::vector<cse::Item> Items {};
    
    /**
    Open file
    Iterate through csv file
    Create Item
    Enter into vector of items   
    **/ 

    return Items;
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

int main() {
  cse::CommandLine mainCommand;
  mainCommand.addCommand(
      "brute-force", BruteForceUnoptimized,
      "Find the optimal solution for a list of items without optimization.");

  std::string input;
  while (std::getline(std::cin, input)) {
    //split input
    // command -> text file seperated by a space
    auto arguments = split(input, ' ');
    //std::cout << arguments[1] << std::endl;
    mainCommand.executeCommand(arguments[0]);
  }
  return 0;
}

/**
    Plans:
    - Add a file name input for item lists
        - Flags: 
            Optimized
            Compare - unoptimized vs optimized
            Weightless - ignore weights
            Multiple repeats
            Required Item (?)
            Cache(?) - Save option

    Ideas:
    - Add ComboManager as useable command
        - Works with permutation or combo
    - Allow user to give a list of names and then choose preset way of calculating weight and value
        - Choose length of name or ASCII value as weight or value
    - STAAATIC VECTOR
        - Emphasis on static vector
    - Return a report text file
    - Return top x results
    - Caching file names and resulting values
        - MemoFunction and or MemoryFactory

    Issues:
    - Submitting repeat file name with different file contents
**/