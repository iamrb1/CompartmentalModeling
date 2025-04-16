#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "../../Group-06/ArgManager/ArgManager.cpp"
#include "../../Group-06/CommandLine/CommandLine.cpp"
#include "../Classes/BruteForceOptimizer.hpp"
#include "../Classes/ComboManager.hpp"
#include "../Classes/StaticVector.hpp"


// global variables because the command line takes no parameters i guess
std::string filename;
double capacity;


// global variables because the command line takes no parameters i guess
std::string filename;
double capacity;

static std::vector<cse::Item> itemList;
static double capacity = 0.0;

std::vector<std::string> split(const std::string &str, char delimiter) {
  std::vector<std::string> sections;
  std::stringstream ss(str);
  std::string token;

  while (std::getline(ss, token, delimiter)) {
    sections.push_back(token);
  }

  return sections;
}

template <typename T>
void PrintVector(std::vector<T> vector) {
  std::cout << std::endl;
  for (const auto &item : vector) {
    std::cout << item << ",";
  }
  std::cout << std::endl;
}

void PrintBruteForceResults(std::pair<double, std::vector<cse::Item>> results,
                            bool optimized = false) {
  std::cout << "Knapsack Solver Results\n"
            << (optimized)
      ? "<<Optimized>>"
      : "<<Unoptimized>>\n";
  std::cout << "Best Score Recorded: " << results.first << '\n';
  std::cout << "Selection of Items:";
  PrintVector(results.second);
}

void BruteForceUnoptimized() {
  std::cout << "Run Unoptimized" << std::endl;
  cse::BruteForceOptimizer knapsackProblemSolver;
  knapsackProblemSolver.SetItems(itemList);
  knapsackProblemSolver.SetCapacity(capacity);
  auto result = knapsackProblemSolver.FindOptimalSolution();
  PrintBruteForceResults(result);
}

void BruteForceOptimized() {
  std::cout << "Run Optimized" << std::endl;
  cse::BruteForceOptimizer knapsackProblemSolver;
  knapsackProblemSolver.SetItems(itemList);
  knapsackProblemSolver.SetCapacity(capacity);
  knapsackProblemSolver.SetOptimizer(true);
  auto result = knapsackProblemSolver.FindOptimalSolution();
  PrintBruteForceResults(result);
}

std::vector<cse::Item> ConstructItems(std::string filename) {
  std::cout << "Constructing Items" << std::endl;
  std::vector<cse::Item> Items{};

  /**
  Open file
  Iterate through csv file
  Create Item
  Enter into vector of items
  **/
  assert(filename.contains('.txt') ||
         filename.contains('.csv') &&
             "This file is not one of the supported types (.txt or .csv)");
  std::ifstream TextFile(filename);
  std::string line;
  while (std::getline(TextFile, line)) {
    std::vector<std::string> itemData = split(line, ',');
    cse::Item item(itemData[0], std::stod(itemData[1]), std::stod(itemData[2]));
    Items.push_back(item);
    std::cout << "Item: " << item.name << " Weight: " << item.weight
              << " Value: " << item.value << std::endl;
  }

  return Items;
}

void BruteForceUnoptimized() { 
  auto items = ConstructItems(filename);
  cse::BruteForceOptimizer optimizer;
  optimizer.SetItems(items);
  optimizer.SetCapacity(capacity);
  auto solutionPair = optimizer.FindOptimalSolution();
  std::cout << "Optimal Value: " << solutionPair.first << std::endl;
  std::cout << "Item Set: " << std::endl;
  for (auto item : solutionPair.second) {
    std::cout << item.name << " " << item.value << " " << item.weight << std::endl;
  }

  std::cout << "Run Unoptimized" << std::endl; 
}


/**
 * @brief creates the arg manager from a vector of strings - converts them into c strings and then passes that to 
 * the arg manager. the original args vector must outlive the argmanager, which should be fine for our code.abort
 * @param args the vector of strings
 * @return an argManager object
 */
cse::ArgManager createArgManager(std::vector<std::string>& args) {
  std::vector<char *> argV(args.size());
  for (int i = 0; i < args.size(); ++i) {
    //argV[i] = strdup(args[i].c_str()); // duplicates a new string. Must use because c_str returns a const string
    argV[i] = args[i].data();
  }
  cse::ArgManager mgr(args.size(),  argV.data());
  return mgr;
}


int main() {
  cse::CommandLine mainCommand;
  mainCommand.addCommand(
      "brute-force", BruteForceUnoptimized,
      "Find the optimal solution for a list of items without optimization.");

  std::string input;
  while (std::getline(std::cin, input)) {
    // split input
    //  command -> text file seperated by a space
    auto arguments = split(input, ' ');

    auto argMgr = createArgManager(arguments);

    // assign global variables
    capacity = std::stod(argMgr.GetSingleOption("capacity"));
    std::cout << capacity << std::endl;

    filename = argMgr.GetSingleOption("filename");

    if (argMgr.HasArg("optimized")) {
      // do something to optimize
      std::cout << "Is optimized." << std::endl;
    }
    if (argMgr.HasArg("compare")) {

    }
    if (argMgr.HasArg("weightless")) {

    }
    if (argMgr.HasArg("multiple-repeats")) {

    }

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
    - Allow user to give a list of names and then choose preset way of
calculating weight and value
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