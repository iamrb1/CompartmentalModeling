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

TEST_CASE("Ivan's WordLang Tests", "[WordLang]") {
  std::string input = "LIST list1 = LOAD \“top_5000_words_database\”\n\
  LIST list2 = LOAD \“top_1000_common_worlde_words_database\”\n\
  LIST combined = list1 | list2\n\
  SAVE combined \"my_custom_list\"\n\
  LENGTH 5\n\
  GET(“a___e”)\n\
  CONTAINS_ANY(\“qwrty\”)
  CONTAINS_ALL(“jh”)\"\
  PRINT *\
  RESET_LAST\ "

  std::string output_result = "\n\n\n\n
  Number of Words To Search: 1200\n\n\
  Number of Words To Search: 30\n\
  Number of Words To Search: 6\n\
  Number of Words To Search: 3\n\
  [ajhqe, ajhre, ahjte]\n\
  Number of Words To Search: 6";

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


TEST_CASE("Orhan's WordLang Tests", "[WordLangO]") {
    std::string input = "List list1 = LOAD Ofile1\n\
    List list2 = LOAD Ofile2\n\
    List combined = COMBINE list1 list2\n\
    ADD combined \"car, printer, green, paper\"\n\
    LENGTH = 5\n\
    CONTAINS \"ae\"\n\
    GET(a___e)\n\
    PRINT\n";

    std::string output_result = "\n\n\n\n"
                                "Number of Words to search: 12\n"
                                "Number of Words to search: 4\n"
                                "Number of Words to search: 1\n"
                                "[apple]\n";

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