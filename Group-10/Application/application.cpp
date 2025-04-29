#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../../Group-06/ArgManager/ArgManager.cpp"
#include "../../Group-06/CommandLine/CommandLine.cpp"
#include "../Classes/BruteForceOptimizer.hpp"
#include "../Classes/ComboManager.hpp"
#include "../Classes/StaticVector.hpp"
#include "ApplicationTools.hpp"

static cse::OptimizerSettings settings;
static const cse::PresetMessages messages;
static constexpr std::size_t CAPACITY_ARGLENGTH = 10;
static constexpr double THRESHOLD_TO_SECONDS = 500;
static constexpr double MILLISECONDS_IN_SECONDS = 1000;
static constexpr double SPEEDUP_ADJUSTMENT_CONSTANT = 100;

/**
 * Utility Functions:
 *  - ResetGlobalVariables - clears data for nnext command
 *  - split - used for separating strings according to delimiters
 *  - MeasureTime - used for speed/time checks
 *  - printVector - basic function to handle the printing of a vector to the
 * console
 *  - PrintTerminal - prints a basic terminal to the screen for user experience
 *  - RedError - Handles the red coloring for all error messages
 *  - EndProgram - Helper function to terminate the application
 *  - CreateArgManager - Interface function for the ArgManager class constructor
 */

void ResetGlobalVariables() {
  settings.filename = "";
  settings.itemList = {};
  settings.capacity = (settings.defaultCapacity.has_value())
                          ? settings.defaultCapacity.value()
                          : 0.0;
  settings.multipleRepeats = false;
  settings.weightless = false;
  settings.compare = false;
  settings.optimized = false;
  settings.timeSearch = false;
}

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
double MeasureTime(Func &&func) {
  auto start = std::chrono::high_resolution_clock::now();
  func();
  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration<double, std::milli>(end - start).count();
}

template <typename T>
void PrintVector(std::vector<T> vector) {
  std::cout << std::endl;
  for (const auto &item : vector) {
    std::cout << item << "," << std::endl;
  }
  std::cout << std::endl;
}

void PrintTerminal() { std::cout << "\033[32m\nBellman-Application> \033[0m"; }

std::string RedError(std::string &&message) {
  return "\033[31m" + message + "\033[0m\n";
}

[[noreturn]] void EndProgram() { std::exit(0); }

void PrintOptimizerResults(std::pair<double, std::vector<cse::Item>> solution) {
  std::cout << "Optimal Value Calculated: " << solution.first << std::endl;
  std::unordered_map<std::string, int> chosenItems;
  double capacityUsed = 0.0;

  for (const auto &item : solution.second) {
    if (chosenItems.contains(item.name)) {
      chosenItems.at(item.name) += 1;
    } else {
      chosenItems.insert({item.name, 1});
    }
    capacityUsed += item.weight;
  }

  std::cout << "Total Capacity used: (" << capacityUsed << '/'
            << settings.capacity << ")\nThe Items selected for the solution:\n";
  auto itemIter = chosenItems.begin();
  while (itemIter != chosenItems.end()) {
    auto item = (*itemIter).first;
    auto itemCount = (*itemIter).second;
    std::cout << item;
    if (itemCount > 1) {
      std::cout << '(' << itemCount << "x)";
    }
    std::cout << std::endl;
    ++itemIter;
  }
  std::cout << std::endl;
}

void PrintTiming(double timingInMilliseconds) {
  if (timingInMilliseconds >= THRESHOLD_TO_SECONDS) {
    std::cout << std::setprecision(4) << "Total time to compute: "
              << timingInMilliseconds / MILLISECONDS_IN_SECONDS << " seconds\n"
              << std::endl;
  } else {
    std::cout << std::setprecision(4)
              << "Total time to compute: " << timingInMilliseconds
              << " milliseconds\n" << std::endl;
  }
}

/**
 * @brief creates the arg manager from a vector of strings - converts them
 * into c strings and then passes that to the arg manager. the original args
 * vector must outlive the argmanager, which should be fine for our code.abort
 * @param args the vector of strings
 * @return an argManager object
 */
cse::ArgManager CreateArgManager(std::vector<std::string> &args) {
  std::vector<char *> argV(args.size());
  for (std::size_t i = 0; i < args.size(); ++i) {
    // argV[i] = strdup(args[i].c_str()); // duplicates a new string. Must use
    // because c_str returns a const string
    argV[i] = args[i].data();
  }
  cse::ArgManager mgr(args.size(), argV.data());
  return mgr;
}

bool isDigit(const std::string& str) {
  std::istringstream iss(str);
  float f;
  iss >> std::noskipws >> f;
  return iss.eof() && !iss.fail();
}

std::vector<cse::Item> ConstructItems(std::ifstream &textFile) {
  std::vector<cse::Item> Items;
  std::string line;
  std::getline(textFile, line);
  int lineNumber = 2;
  while (std::getline(textFile, line)) {
    std::vector<std::string> itemData = split(line, ',');
    if (itemData.size() != 3) {
      throw std::length_error("CSV parse error on line " + std::to_string(lineNumber) + " expected 3 fields, got: " +
        std::to_string(itemData.size()));
    }
    if (!isDigit(itemData[1])) {
      throw std::invalid_argument("CSV parse error on line " + std::to_string(lineNumber) + " invalid argument.");
    }
    if (!isDigit(itemData[2])) {
      throw std::invalid_argument("CSV parse error on line " + std::to_string(lineNumber) + " invalid argument.");
    }
    
    cse::Item item(itemData[0], std::stod(itemData[1]), std::stod(itemData[2]));
    Items.push_back(item);
    ++lineNumber;
  }

  return Items;
}

void CallBruteForceOptimizer() {
  cse::BruteForceOptimizer optimizer;
  optimizer.SetItems(settings.itemList);
  optimizer.SetCapacity(settings.capacity);
  optimizer.SetRepeating(settings.multipleRepeats);

  double unoptimizedTime = 0.0;
  double optimizedTime = 0.0;

  if (!settings.optimized || settings.compare) {
    // Unoptimized brute force
    optimizer.SetOptimizer(false);
    std::cout << "Computing Unoptimized Search...\n";
    unoptimizedTime = MeasureTime([&]() {
      auto solutionPair = optimizer.FindOptimalSolution();
      PrintOptimizerResults(solutionPair);
    });
    if (settings.timeSearch || settings.compare) PrintTiming(unoptimizedTime);
  }

  if (settings.optimized || settings.compare) {
    // Optimized brute force
    optimizer.SetOptimizer(true);
    std::cout << "Computing Optimized Search...\n";
    optimizedTime = MeasureTime([&]() {
      auto solutionPair = optimizer.FindOptimalSolution();
      PrintOptimizerResults(solutionPair);
    });
    if (settings.timeSearch || settings.compare) PrintTiming(optimizedTime);
  }

  if (settings.compare) {
    // Print comparison results
    std::cout << "Speedup: "
              << ((unoptimizedTime / optimizedTime) *
                  SPEEDUP_ADJUSTMENT_CONSTANT) -
                     SPEEDUP_ADJUSTMENT_CONSTANT
              << "%\n";
  }
}

/**
 * For Commands where weights should have no influence, set to a weight of 1
 * for 1 'item'
 */
void AdjustWeights() {
  for (auto &item : settings.itemList) {
    item.weight = 1.0;
  }
}

int application(std::istream &in) {
  std::cout << messages.welcomeMessage << messages.commandListMessage;
  PrintTerminal();
  cse::CommandLine mainCommand;
  mainCommand.addCommand(
      "brute-force", CallBruteForceOptimizer,
      "Find the optimal solution for a list of items without optimization.");

  std::string input;
  while (std::getline(in, input)) {
    ResetGlobalVariables();
    // split input
    //  command -> text file seperated by a space
    auto arguments = split(input, ' ');
    auto argMgr = CreateArgManager(arguments);
    if (!arguments.size()) {
      PrintTerminal();
      continue;
    } else if ((arguments.at(0) == "q" || arguments.at(0) == "Q" ||
                arguments.at(0) == "quit" || arguments.at(0) == "exit")) {
      if (arguments.size() > 1) {
        std::cout << RedError(
            "**Error: the quit command cannot be run with additional "
            "arguments.");
        PrintTerminal();
        continue;
      } else {
        EndProgram();
      }
    }

    else if (arguments.at(0) == "help") {
      std::cout << "All commands:\n" << messages.commandListMessage;
    }

    else if (arguments[0] == "set-capacity") {
      if (arguments.size() == 1) {
        std::cout << RedError(
            "**Must specify a default capacity\n (use -h flag for more "
            "information)");
      } else if (arguments.size() > 2) {
        std::cout << RedError(
            "**Too many arguments provided for set-capacity\n (use -h flag for "
            "more information)");
      } else {
        try {
          if (argMgr.HasArg("-help") || argMgr.HasArg("-h")) {
            // do something to optimize
            std::cout << messages.capacityHelp;
            PrintTerminal();
            continue;
          }
          double givenCapacity = std::stod(arguments.at(1));
          settings.defaultCapacity = givenCapacity;
          settings.capacity = givenCapacity;
          std::cout << settings.defaultCapacity.value()
                    << " set as default capacity.\n";
        } catch (const std::exception &e) {
          std::cout << RedError("**Invalid value given for the capacity");
        }
      }
    }

    else if (arguments[0] == "show-capacity") {
      (settings.defaultCapacity.has_value())
          ? std::cout << "Default value set to: "
                      << settings.defaultCapacity.value()
          : std::cout << "No default value set.";
      std::cout << '\n';
    }

    else if (arguments[0] == "brute-force") {
      if (argMgr.HasArg("-help") || argMgr.HasArg("-h")) {
        // do something to optimize
        std::cout << messages.optimizerHelpMessage;
        PrintTerminal();
        continue;
      }

      if (arguments.size() < 2) {
        std::cout << RedError(
            "**Please specify a filename as the second argument.");
        PrintTerminal();
        continue;
      }
      settings.filename = arguments[1];

      if (!settings.filename.ends_with(".txt") &&
          !settings.filename.ends_with(".csv")) {
        std::cout << RedError(
            "**The file must be of a valid type (.txt or .csv)");
        PrintTerminal();
        continue;
      }
      std::ifstream textFile(settings.filename);
      if (textFile.is_open()) {
        try {
          settings.itemList = ConstructItems(textFile);
        } catch (std::length_error e) {
          std::cout << RedError(e.what());
          PrintTerminal();
          continue;
        } catch (std::invalid_argument e) {
          std::cout << RedError(e.what());
          PrintTerminal();
          continue;
        }
        
      } else {
        std::cout << RedError("** \""+settings.filename+"\" is not a valid file path");
        PrintTerminal();
        continue;
      }
      std::string valString;
      std::string toFind = "-capacity=";
      auto capacityArg = std::find_if(
          arguments.begin(), arguments.end(),
          [=](auto str) { return str.find(toFind) != std::string::npos; });

      // If -capacity=<value> is not present
      if (capacityArg == arguments.end()) {
        // no default capacity either, throw error
        if (!settings.defaultCapacity.has_value()) {
          std::cout << RedError(
              "**Specify capacity=\033[32m<capacity>\033[0m.");
          PrintTerminal();
          continue;
        }
        // Default capacity exists, assign value
        else {
          settings.capacity = settings.defaultCapacity.value();
        }
      } else {
        valString =
            capacityArg->substr(CAPACITY_ARGLENGTH, capacityArg->length());

        // assign global variables
        try {
          settings.capacity = std::stod(valString);
        } catch (const std::invalid_argument &e) {
          std::cout << RedError("**Capacity must be a numeric value.");
          PrintTerminal();
          continue;
        }
      }

      if (argMgr.HasArg("optimized") || argMgr.HasArg("-o")) {
        // do something to optimize
        settings.optimized = true;
      }
      if (argMgr.HasArg("-compare") || argMgr.HasArg("-c")) {
        settings.compare = true;
        settings.timeSearch = true;
      }
      if (argMgr.HasArg("-no-weight") || argMgr.HasArg("-w")) {
        settings.weightless = true;
        AdjustWeights();
      }
      if (argMgr.HasArg("-repeats") || argMgr.HasArg("-r")) {
        settings.multipleRepeats = true;
      }
      if (argMgr.HasArg("-t") || argMgr.HasArg("-T")) {
        settings.timeSearch = true;
      }
      mainCommand.executeCommand(arguments[0]);
    }

    else {
      std::cout << RedError(
          "**Command not recognized.\n\nUse command \"help\" to see a list "
          "of "
          "available commands.");
    }
    PrintTerminal();
  }

  return 0;
}