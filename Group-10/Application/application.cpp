#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "ApplicationTools.hpp"
#include "ArgManager.cpp"
#include "BruteForceOptimizer.hpp"
#include "ComboManager.hpp"
#include "CommandLine.cpp"
#include "StaticVector.hpp"
namespace PresetMessages = cse::PresetMessages;

static cse::OptimizerSettings settings;
static constexpr std::size_t CAPACITY_ARGLENGTH = 10;
static constexpr double THRESHOLD_TO_SECONDS = 500;
static constexpr double MILLISECONDS_IN_SECONDS = 1000;
static constexpr double SPEEDUP_ADJUSTMENT_CONSTANT = 100;

/**
 * Returns all General Settings to their default state
 * - Only exception is if defaultCapacity is set, as that needs to persist
 */
void ResetGlobalVariables() {
  std::optional<double> currDefault = settings.defaultCapacity;
  settings = cse::OptimizerSettings();
  settings.defaultCapacity = currDefault;
  if (currDefault.has_value()) {
    settings.capacity = *currDefault;
  }
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

/**
 * @brief records the time taken for a function to execute
 *
 * @return double with the amount of time in milliseconds
 */
template <typename Func>
double MeasureTime(Func &&func) {
  auto start = std::chrono::high_resolution_clock::now();
  func();
  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration<double, std::milli>(end - start).count();
}

/**
 *  Basic output used to simulate a terminal interface
 */
void PrintTerminal() {
  std::cout << cse::Color::GREEN << "\nBellman-Application> "
            << cse::Color::RESET;
}

/**
 * Helper function for outputting errors
 */
std::string RedError(std::string &&message) {
  return cse::Color::RED + message + cse::Color::RESET;
}

/**
 * Terminates the program
 */
[[noreturn]] void EndProgram() { std::exit(0); }

/**
 * Prints the solution of any Optimizer run
 * @param optimalScore The score the optimizer returns
 * @param optimalSolution a Vector containing the items used in the score
 */
void PrintOptimizerResults(double &optimalScore,
                           std::vector<cse::Item> &optimalSolution) {
  std::cout << "Optimal Value Calculated: " << optimalScore << std::endl;
  std::map<std::string, int> chosenItems;
  double capacityUsed = 0.0;

  // Combines repeated items and keeps a count for readability
  for (const auto &item : optimalSolution) {
    if (chosenItems.contains(item.name)) {
      chosenItems.at(item.name) += 1;
    } else {
      chosenItems.insert({item.name, 1});
    }
    capacityUsed += item.weight;
  }

  std::cout << "Total Capacity used: (" << capacityUsed << '/'
            << settings.capacity << ")\nThe Items selected for the solution:\n";

  for (const auto &pair : chosenItems) {
    auto [name, count] = pair;
    std::cout << name;
    if (count > 1) {
      std::cout << '(' << count << "x)";
    }
    if (pair != *(chosenItems.rbegin())) {
      std::cout << ", ";
    }
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
              << " milliseconds\n"
              << std::endl;
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

/**
 * Constructs items based on the input of a file
 *
 * @param textFile filestream to read in items
 * @return A vector of constructed items
 */
std::vector<cse::Item> ConstructItems(std::ifstream &textFile) {
  std::vector<cse::Item> Items;
  std::string line;
  std::getline(textFile, line);
  int lineNumber = 2;
  while (std::getline(textFile, line)) {
    std::vector<std::string> itemData = split(line, ',');
    if (itemData.size() != 3) {
      throw std::length_error(
          "CSV parse error on line " + std::to_string(lineNumber) +
          " expected 3 fields, got: " + std::to_string(itemData.size()));
    }
    try {
      auto weight = std::stod(itemData[1]);
      auto value = std::stod(itemData[2]);
      try {
        cse::Item item(itemData[0], weight, value);
        Items.push_back(item);

      } catch (std::invalid_argument &e) {
        throw std::invalid_argument(
            "Item input error on line " + std::to_string(lineNumber) +
            " invalid input, weights and values must be positive.");
      }
      ++lineNumber;
    } catch (std::invalid_argument &e) {
      throw std::invalid_argument("CSV parse error on line " +
                                  std::to_string(lineNumber) +
                                  " invalid argument.");
    }
  }

  return Items;
}

/**
 * Driver function for the brute-force command
 * Uses the global settings to program BruteForceOptimizer and retrieve the
 * output
 */
void CallBruteForceOptimizer() {
  cse::BruteForceOptimizer optimizer;
  optimizer.SetItems(settings.itemList);
  optimizer.SetCapacity(settings.capacity);
  optimizer.SetRepeating(settings.multipleRepeats);

  double unoptimizedTime = 0.0;
  double optimizedTime = 0.0;
  std::pair<double, std::vector<cse::Item>> solutionPair;

  if (!settings.optimized || settings.compare) {
    // Unoptimized brute force
    optimizer.SetOptimizer(false);
    std::cout << "Computing Unoptimized Search...\n";
    unoptimizedTime =
        MeasureTime([&]() { solutionPair = optimizer.FindOptimalSolution(); });
    PrintOptimizerResults(solutionPair.first, solutionPair.second);
    if (settings.timeSearch || settings.compare) PrintTiming(unoptimizedTime);
  }

  if (settings.optimized || settings.compare) {
    // Optimized brute force
    optimizer.SetOptimizer(true);
    std::cout << "Computing Optimized Search...\n";
    optimizedTime =
        MeasureTime([&]() { solutionPair = optimizer.FindOptimalSolution(); });
    PrintOptimizerResults(solutionPair.first, solutionPair.second);
    if (settings.timeSearch || settings.compare) PrintTiming(optimizedTime);
  }

  if (settings.compare) {
    // Print comparison results
    assert(optimizedTime != 0 &&
           "Optimizer time has evaluated to 0, speedup is extreme.");
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

/**
 * Main Application Function
 * Handles all command inputs and reroutes as appropriate
 */
int application(std::istream &in) {
  std::cout << PresetMessages::welcomeMessage
            << PresetMessages::commandListMessage;
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
    }
    const std::string &commandName = arguments[0];

    if ((commandName == "q" || commandName == "Q" || commandName == "quit" ||
         commandName == "exit")) {
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

    else if (commandName == "help") {
      std::cout << "All commands:\n" << PresetMessages::commandListMessage;
    }

    else if (commandName == "set-capacity") {
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
            std::cout << PresetMessages::capacityHelp;
            PrintTerminal();
            continue;
          }
          double givenCapacity = std::stod(arguments.at(1));
          settings.defaultCapacity = givenCapacity;
          settings.capacity = givenCapacity;
          std::cout << settings.defaultCapacity.value()
                    << " set as default capacity.\n";
        } catch (const std::invalid_argument &e) {
          std::cout << RedError("**Invalid value given for the capacity");
        }
      }
    }

    else if (commandName == "show-capacity") {
      if (settings.defaultCapacity.has_value()) {
        std::cout << "Default value set to: " << *settings.defaultCapacity;
      } else {
        std::cout << "No default value set.";
      }
      std::cout << '\n';
    }

    else if (commandName == "brute-force") {
      if (argMgr.HasArg("-help") || argMgr.HasArg("-h")) {
        // do something to optimize
        std::cout << PresetMessages::optimizerHelpMessage;
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
        } catch (std::length_error &e) {
          std::cout << RedError(e.what());
          PrintTerminal();
          continue;
        } catch (std::invalid_argument &e) {
          std::cout << RedError(e.what());
          PrintTerminal();
          continue;
        }

      } else {
        std::cout << RedError("** \"" + settings.filename +
                              "\" is not a valid file path");
        PrintTerminal();
        continue;
      }
      std::string valString;
      std::string toFind = "-capacity=";
      auto capacityArg =
          std::find_if(arguments.begin(), arguments.end(),
                       [=](auto &str) { return str.starts_with(toFind); });

      // If -capacity=<value> is not present
      if (capacityArg == arguments.end()) {
        // no default capacity either, throw error
        if (!settings.defaultCapacity.has_value()) {
          std::cout << RedError("**Specify -capacity=" + cse::Color::GREEN +
                                "<capacity>");
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
      mainCommand.executeCommand(commandName);
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