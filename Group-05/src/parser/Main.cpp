#include <iostream>
#include "Parser.h"

int main() {
  // Will turn this into catch2 tests later.
  // Right now decimal functionality is not implemented.
  // That would be the next step
  Parser parser;
  std::cout << (parser.Evaluate("{-2} + {2}") == 0) << "\n";
  std::cout << (parser.Evaluate("{-15} + {-15}") == -30) << "\n";
  std::cout << (parser.Evaluate("{15} + {15}") == 30) << "\n";
  std::cout << (parser.Evaluate("{-2} - {0}}") == -2) << "\n";
  std::cout << (parser.Evaluate("{15} - {-15}") == 30) << "\n";
  std::cout << (parser.Evaluate("{-15} - {15}") == -30) << "\n";
  std::cout << (parser.Evaluate("{30} - {15}") == 15) << "\n";
  std::cout << (parser.Evaluate("{-2} * {0}}") == 0) << "\n";
  std::cout << (parser.Evaluate("{-2} * {-2}") == 4) << "\n";
  std::cout << (parser.Evaluate("{2} * {2}") == 4) << "\n";
  std::cout << (parser.Evaluate("{-2} * {2}") == -4) << "\n";
  std::cout << (parser.Evaluate("{0} / {2}}") == 0) << "\n";
  std::cout << (parser.Evaluate("{-2} / {-2}") == 1) << "\n";
  std::cout << (parser.Evaluate("{2} / {2}") == 1) << "\n";
  std::cout << (parser.Evaluate("{-2} / {2}") == -1) << "\n";
  std::cout << (parser.Evaluate("{0.5} / {2}") == 0) << "\n";
  std::cout << (parser.Evaluate("{1} / {4}") == 0) << "\n";
  return 0;
}
