#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
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
static std::vector<cse::Item> itemList;
static double capacity = 0.0;
bool multipleRepeats = false;
bool weightless = false;
bool compare = false;
bool optimized = false;

static constexpr std::size_t CAPACITY_ARGLENGTH = 10;
static const std::string welcomeMessage =
    "Welcome to Knapsack Solver.\n"
    "This solver can process any text or CSV file containing items with names, "
    "weights, and values and return what the best lection of items are for the "
    "space you have.\n"
    "When using this solver, here are the options:\n";

static const std::string helpMessage =
    "Command:\n"
    "brute-force <filepath> -capacity=<value>\n\n"
    "All files must either be .csv, or .txt formatted in the CSV style.\n"
    "We also have a collection of arguments you can pass alongside that "
    "command if you want other results.\n"
    "-help (-h) = Prints this message\n"
    "-optimize (-o) = Turns on all optimization flags for the problem\n"
    "-compare (-c) = Solves the problem both unoptimized and optimized, then "
    "compares the results to see the speedup\n"
    "-no-weight (-w) = Removes weight considerations. The algorithm will only "
    "consider the number of items in respect to the capacity\n"
    "-repeats (-r) = Allows for items to be used multiple times in the "
    "solution\n";

/**
 * @brief - helps split strings
 * @param str - the string
 * @param delimiter - what to split the string by
 * @return vector of the split string
 */
std::vector<std::string> split(const std::string &str, char delimiter) {
  std::vector<std::string> sections;
  std::stringstream ss(str);
  std::string token;

  while (std::getline(ss, token, delimiter)) {
    sections.push_back(token);
  }

  return sections;
}

// Utility function to measure execution time
template <typename Func>
double measureTime(Func &&func) {
  auto start = std::chrono::high_resolution_clock::now();
  func();
  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration<double, std::milli>(end - start).count();
}

template <typename T>
void PrintVector(std::vector<T> vector) {
  std::cout << std::endl;
  for (const auto &item : vector) {
    std::cout << item << ",";
  }
  std::cout << std::endl;
}

std::vector<cse::Item> ConstructItems(std::string filename) {
  std::vector<cse::Item> Items{};

  assert((filename.contains(".txt") || filename.contains(".csv")) &&
         "This file is not one of the supported types (.txt or .csv)");
  std::ifstream TextFile(filename);
  std::string line;
  std::getline(
      TextFile,
      line);  // TODO: Check column headers and adjust where the values are
  while (std::getline(TextFile, line)) {
    std::vector<std::string> itemData = split(line, ',');
    cse::Item item(itemData[0], std::stod(itemData[1]), std::stod(itemData[2]));
    Items.push_back(item);
  }

  return Items;
}

void Compare() {
  cse::BruteForceOptimizer optimizer;
  optimizer.SetItems(itemList);
  optimizer.SetCapacity(capacity);
  optimizer.SetOptimizer(false);
  double unoptimizedTime =
      measureTime([&]() { optimizer.FindOptimalSolution(); });
  auto unoptimizedScore = optimizer.FindOptimalSolution().first;

  optimizer.SetOptimizer(true);

  std::pair<double, std::vector<cse::Item>> solutionPair;

  double optimizedTime =
      measureTime([&]() { optimizer.FindOptimalSolution(); });
  auto optimizedScore = optimizer.FindOptimalSolution().first;

  std::cout << "Unoptimized Time: " << unoptimizedTime << std::endl;
  std::cout << "Optimized Time: " << optimizedTime << std::endl;
  std::cout << "Speedup: "
            << ((unoptimizedTime - optimizedTime) / unoptimizedTime) * 100
            << "%\n";
}

void BruteForceUnoptimized() {
  cse::BruteForceOptimizer optimizer;

  optimizer.SetItems(itemList);
  optimizer.SetCapacity(capacity);
  optimizer.SetOptimizer(optimized);

  if (compare) {
    Compare();
  }

  auto solutionPair = optimizer.FindOptimalSolution();
  std::cout << "Optimal Value: " << solutionPair.first << std::endl;
  std::cout << "Item Set: " << std::endl;
  PrintVector(solutionPair.second);

  std::cout << "Run Unoptimized" << std::endl;
}

void BruteForceOptimized() {
  std::cout << "Run Optimized" << std::endl;
  cse::BruteForceOptimizer knapsackProblemSolver;
  knapsackProblemSolver.SetItems(itemList);
  knapsackProblemSolver.SetCapacity(capacity);
  knapsackProblemSolver.SetOptimizer(true);
  auto result = knapsackProblemSolver.FindOptimalSolution();
  std::cout << "Optimal Value: " << result.first << std::endl;
  std::cout << "Item Set: " << std::endl;
  for (auto item : result.second) {
    std::cout << item.name << " " << item.value << " " << item.weight
              << std::endl;
  }
}

/**
 * For Commands where weights should have no influence, set to a weight of 1 for
 * 1 'item'
 */
void AdjustWeights() {
  for (auto item : itemList) {
    item.weight = 1.0;
  }
}

/**
 * @brief creates the arg manager from a vector of strings - converts them into
 * c strings and then passes that to the arg manager. the original args vector
 * must outlive the argmanager, which should be fine for our code.abort
 * @param args the vector of strings
 * @return an argManager object
 */
cse::ArgManager createArgManager(std::vector<std::string> &args) {
  std::vector<char *> argV(args.size());
  for (std::size_t i = 0; i < args.size(); ++i) {
    // argV[i] = strdup(args[i].c_str()); // duplicates a new string. Must use
    // because c_str returns a const string
    argV[i] = args[i].data();
  }
  cse::ArgManager mgr(args.size(), argV.data());
  return mgr;
}

void AdjustRepeats() {
  std::vector<cse::Item> initItems = itemList;
  for (auto item : initItems) {
    int maxItemAmount = capacity / item.weight;
    if (maxItemAmount > 0) {
      // std::cout << maxItemAmount <<std::endl;
      std::vector<cse::Item> extraItems(--maxItemAmount, item);
      itemList.insert(itemList.end(), extraItems.begin(), extraItems.end());
    }
  }
}

void HelpOutput() { std::cout << helpMessage; }

void PrintTerminal() { std::cout << "\033[32m\nBellman-Application> \033[0m"; }

std::string RedError(std::string &&message) {
  return "\033[31m" + message + "\033[0m";
}

int main() {
  std::cout << welcomeMessage << helpMessage;
  PrintTerminal();
  cse::CommandLine mainCommand;
  mainCommand.addCommand(
      "brute-force", BruteForceUnoptimized,
      "Find the optimal solution for a list of items without optimization.");

  std::string input;
  while (std::getline(std::cin, input)) {
    // split input
    //  command -> text file seperated by a space
    auto arguments = split(input, ' ');
    if (!arguments.size()) {
      PrintTerminal();
      continue;
    }

    auto argMgr = createArgManager(arguments);

    if (argMgr.HasArg("-help") || argMgr.HasArg("-h")) {
      // do something to optimize
      HelpOutput();
      PrintTerminal();
      continue;
    }

    if (arguments.size() < 2) {
      std::cout << RedError(
                       "**Please specify a filename as the second argument.")
                << std::endl;
      PrintTerminal();
      continue;
    }

    if (!arguments[1].contains(".txt") && !arguments[1].contains(".csv")) {
      std::cout << RedError(
          "**The file must be of a valid type (.txt or .csv)");
      PrintTerminal();
      continue;
    }

    std::string valString;
    std::string toFind = "-capacity=";
    auto capacityArg = std::find_if(arguments.begin(), arguments.end(), [=](auto str) {
      return str.find(toFind) != std::string::npos;
    });
    if (capacityArg == arguments.end()) {
      std::cout << RedError("**Specify capacity=<capacity>.") << std::endl;
      continue;
    }
    valString = capacityArg->substr(CAPACITY_ARGLENGTH, capacityArg->length());
      

    itemList = ConstructItems(filename);

    // assign global variables
    try {
      capacity = std::stod(valString);
    } catch (const std::invalid_argument &e) {
      std::cout << RedError("**Specify capacity <capacity> as a numeric value.")
                << std::endl;
      PrintTerminal();
      continue;
    }

    if (argMgr.HasArg("optimized") || argMgr.HasArg("-o")) {
      // do something to optimize
      BruteForceOptimized();
    }
    if (argMgr.HasArg("-compare") || argMgr.HasArg("-c")) {
      compare = false;
    }
    if (argMgr.HasArg("-no-weight") || argMgr.HasArg("-w")) {
      weightless = true;
      AdjustWeights();
    }
    if (argMgr.HasArg("-repeats") || argMgr.HasArg("-r")) {
      multipleRepeats = true;
      AdjustRepeats();
    }
    mainCommand.executeCommand(arguments[0]);
    PrintTerminal();
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