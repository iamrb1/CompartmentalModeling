#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#include "../../Group-06/ArgManager/ArgManager.cpp"
#include "../../Group-06/CommandLine/CommandLine.cpp"
#include "../Classes/BruteForceOptimizer.hpp"
#include "../Classes/ComboManager.hpp"
#include "../Classes/StaticVector.hpp"

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

template <typename T>
void PrintVector(std::vector<T> vector) {
    std::cout << std::endl;
    for (const auto& item : vector) {
        std::cout << item << ",";
    }
    std::cout << std::endl;
}

void BruteForceUnoptimized() { std::cout << "Run Unoptimized" << std::endl; }

std::vector<cse::Item> ConstructItems(std::string filename){ 
    std::cout << "Constructing Items" << std::endl;
    std::vector<cse::Item> Items {};
    
    /**
    Open file
    Iterate through csv file
    Create Item
    Enter into vector of items   
    **/ 
    std::ifstream TextFile(filename);
    std::string line;
    while (std::getline(TextFile, line)) {
        
        cse::Item item;
        std::vector<std::string> itemData = split(line, ',');
        item.name = itemData[0];
        item.weight = std::stod(itemData[1]);
        item.value = std::stod(itemData[2]);
        Items.push_back(item);
        std::cout << "Item: " << item.name << " Weight: " << item.weight << " Value: " << item.value << std::endl;
    }

    return Items;
}


int main() {
  cse::CommandLine mainCommand;
  mainCommand.addCommand(
      "brute-force", BruteForceUnoptimized,
      "Find the optimal solution for a list of items without optimization.");
  

//   mainCommand.addCommand(
//     "construct", BruteForceUnoptimized,
//     "Find the optimal solution for a list of items without optimization.");
  std::string input;
  while (std::getline(std::cin, input)) {
    //split input
    // command -> text file seperated by a space
    auto arguments = split(input, ' ');
    auto vectorList = ConstructItems(arguments[1]);
    mainCommand.executeCommand(arguments[0]);

    // use ArgManager
    char *argVal2 []= {(char *)"--TestGetter", (char *)"success.dat"};
    char *argVal[vectorList.size()];
    for (auto string : vectorList) {
        
    }
    int argC2 = 2;
    cse::ArgManager argManager(argC2, argVal2);
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

    g++ main.cpp -std=c++23 -g -pthread -Wall -Wextra -o main
**/