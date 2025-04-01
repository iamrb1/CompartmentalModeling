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
  // std::string input = "LIST list1 = LOAD \"File1\"\nLIST list2 = LOAD \"File2\"\n\
  // LIST combined = COMBINE list1 list2\n\
  // LIST result = DIFFERANCE combined\n\
  // SET_CURRENT result\n\
  // LENGTH = 5\n\
  // GET “_o__s”\n\
  // PRINT 2\n\
  // PRINT ALL\n\
  // NOT_CONTAINS “wet”\n\
  // PRINT ALL\n";

  std::string input = R"(LIST list1 = LOAD "File1"
  LIST list2 = LOAD "File2"
  LIST combined = COMBINED list1 list2
  LIST result = DIFFERANCE combined
  SET_CURRENT result
  LENGTH = 5
  GET "_o__s"
  PRINT 2
  PRINT ALL
  NOT_CONTAINS "wet"
  PRINT ALL
  )";
  

  // std::string output_result = "\n\n\n\n\n\
  // Number of Words To Search: 5\n\n\
  // [boats, books]\n\
  // [where, toast, boost, books, boats]\n\n\
  // [books]\n";

  std::string output_result = R"(



  Number of Words To Search: 5
  
  [boats, books]
  [where, toast, boost, books, boats]
  
  [books]
  )";
  

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
  // std::string input = "LIST list1 = LOAD \"top_5000_words_database\"\n\
  // LIST list2 = LOAD \"top_1000_common_worlde_words_database\"\n\
  // LIST combined = list1 | list2\n\
  // SAVE combined \"my_custom_list\"\n\
  // LENGTH 5\n\
  // GET \"a___e\"\n\
  // CONTAINS_ANY \"qwrty\"
  // CONTAINS_ALL \"jh\"
  // PRINT ALL\
  // RESET_LAST "
  std::string input = R"(LIST list1 = LOAD "top_5000_words_database"
  LIST list2 = LOAD "top_1000_common_worlde_words_database"
  LIST combined = COMBINED list1 list2
  SAVE combined "my_custom_list"
  LENGTH 5
  GET "a___e"
  CONTAINS_ANY "qwrty"
  CONTAINS_ALL "jh"
  PRINT ALL
  RESET_LAST)";
  
  

  // std::string output_result = "\n\n\n\n
  // Number of Words To Search: 1200\n\n\
  // Number of Words To Search: 30\n\
  // Number of Words To Search: 6\n\
  // Number of Words To Search: 3\n\
  // [ajhqe, ajhre, ahjte]\n\
  // Number of Words To Search: 6";

  std::string output_result = R"(


  Number of Words To Search: 1200
  
  Number of Words To Search: 30
  Number of Words To Search: 6
  Number of Words To Search: 3
  [ajhqe, ajhre, ahjte]
  Number of Words To Search: 6)";

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
    // std::string input = "LIST list1 = LOAD Ofile1\n\
    // LIST list2 = LOAD Ofile2\n\
    // LIST combined = COMBINE list1 list2\n\
    // ADD combined \"car, printer, green, paper\"\n\
    // LENGTH = 5\n\
    // CONTAINS_ALL \"ae\"\n\
    // GET(a___e)\n\
    // PRINT\n";

    std::string input = R"(LIST list1 = LOAD "Ofile1"
    LIST list2 = LOAD "Ofile2"
    LIST combined = COMBINED list1 list2
    ADD combined "car printer green paper"
    LENGTH = 5
    CONTAINS_ALL "ae"
    GET "a___e"
    PRINT ALL
    )";
    

    // std::string output_result = "\n\n\n\n"
    //                             "Number of Words to search: 12\n"
    //                             "Number of Words to search: 4\n"
    //                             "Number of Words to search: 1\n"
    //                             "[apple]\n";
    std::string output_result = R"(



    Number of Words to search: 12
    Number of Words to search: 4
    Number of Words to search: 1
    [apple]
    )";
    

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


TEST_CASE("Intersection and Filter Test", "[WordLangIntersect]") {
  std::string input = R"(LIST a = LOAD "intersect_file1.txt"
  LIST b = LOAD "intersect_file2.txt"
  LIST c = INTERSECTION a b
  SET_CURRENT c
  NOT_CONTAINS "n"
  PRINT ALL
  )";

  std::string output_result = R"(


  Number of Words To Search: 3
  [fable, label, table]
  )";

  cse::WordLang wordLang;

  std::istringstream iss(input);
  std::istringstream issResult(output_result);
  std::string line, resultLine;

  while (std::getline(iss, line) && std::getline(issResult, resultLine)) {
      std::string output = wordLang.parse(line);
      REQUIRE(output == resultLine);
  }
}
