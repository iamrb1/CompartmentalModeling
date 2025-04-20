#include <optional>
#include <string>
#include <vector>

#include "../Classes/BruteForceOptimizer.hpp"

namespace cse {
struct PresetMessages {
  const std::string welcomeMessage =
      "Welcome to Knapsack Solver.\n"
      "This solver can process any text or CSV file containing items with "
      "names, "
      "weights, and values and return what the best lection of items are for "
      "the "
      "space you have.\n"
      "When using this solver, here are the command options:\n";

  const std::string commandListMessage =
      "\033[34mhelp\033[0m - Prints a list of all available commands\n"
      "\033[34mset-capacity \033[32m<value>\033[34m\033[0m - Sets a default "
      "capacity for future brute-force commands to use (unless specified "
      "otherwise)\n"
      "\033[34mbrute-force \033[32m<filepath>\033[34m "
      "-capacity=\033[32m<value>\033[0m - This will run the Knapsack Solver on "
      "the provided data file and return the results.\n"
      "\033[34mquit\033[0m - Closes the application. Other completions include "
      "\'q\', \'Q\', and \'exit\'\n";

  const std::string capacityHelp =
      "\033[34mset-capacity \033[32m<value>\033[34m\033[0m\n"
      "\033[32m-help\033[0m (\033[32m-h\033[0m) = Prints this message\n";

  const std::string optimizerHelpMessage =
      "\033[34mbrute-force \033[32m<filepath>\033[34m "
      "-capacity=\033[32m<value>\033[0m\n\n"
      "All files must either be .csv, or .txt formatted in the CSV style.\n"
      "We also have a collection of arguments you can pass alongside that "
      "command if you want other results.\n"
      "\033[32m-help\033[0m (\033[32m-h\033[0m) = Prints this message\n"
      "\033[32m-optimize\033[0m (\033[32m-o\033[0m) = Turns on all "
      "optimization "
      "flags for the problem\n"
      "\033[32m-compare\033[0m (\033[32m-c\033[0m) = Solves the problem both "
      "unoptimized and optimized, then "
      "compares the results to see the speedup\n"
      "\033[32m-no-weight\033[0m (\033[32m-w\033[0m) = Removes weight "
      "considerations. The algorithm will only "
      "consider the number of items in respect to the capacity\n"
      "\033[32m-repeats\033[0m (\033[32m-r\033[0m) = Allows for items to be "
      "used "
      "multiple times in the "
      "solution\n";
};

struct OptimizerSettings {
  std::string filename = "";
  std::vector<cse::Item> itemList = {};
  double capacity = 0.0;
  bool multipleRepeats = false;
  bool weightless = false;
  bool compare = false;
  bool optimized = false;
  bool timeSearch = false;
  std::optional<double> defaultCapacity = std::nullopt;
};
}  // namespace cse