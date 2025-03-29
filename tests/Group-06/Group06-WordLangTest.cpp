#include <string>
#include <iostream>
#include <istream>
#include "../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../Group-06/StringSet/StringSet.hpp"
#include "../../Group-06/StaticString/StaticString.hpp"
#include "../../Group-06/ArgManager/ArgManager.h"
#include "../../Group-06/CommandLine/CommandLine.cpp"
#include "../../Group-06/ErrorManager/ErrorManager.hpp"
#include "../../Group-06/WordLang/WordLang.hpp"

using namespace cse;

TEST_CASE("WordLang Tests", "[WordLang]") {
  std::string input = "List list1 = load File1\nList list2 = load File2\n\
  List combined = COMBINE list1 list2\n\
  List result = DIFFERANCE combined\n\
  Current_List = result\n\
  LENGTH = 5\n\
  GET “?o??s”\n\
  PRINT 2\n\
  PRINT ALL\n\
  NOT_CONTAINS “wet”\n\
  PRINT ALL\n";

  std::string output_result = "\n\n\n\n\n\
  Number of Words To Search: 5\n\n\
  [boats, books]\n\
  [where, toast, boost, books, boats]\n\n\
  [books]\n";

  cse::WordLang wordLang;

  std::istringstream iss(input);
  std::istringstream issResult(output_result);

  std::string line;
  std::string resultLine;

  while(std::getline(iss, line) && std::getline(issResult, resultLine)) {
    std::string output = wordLang.parse(line);
    REQUIRE(output == resultLine);
  }

}