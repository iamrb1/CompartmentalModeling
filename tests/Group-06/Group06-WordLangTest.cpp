#include <string>
#include <iostream>
#include <istream>
#include <filesystem>
#include "../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../Group-06/StringSet/StringSet.hpp"
#include "../../Group-06/StaticString/StaticString.hpp"
#include "../../Group-06/ArgManager/ArgManager.hpp"
#include "../../Group-06/CommandLine/CommandLine.cpp"
#include "../../Group-06/ErrorManager/ErrorManager.hpp"
#include "../../Group-06/App/src/WordLangApp/WordLang.hpp"

using namespace cse;

const static bool DEBUG_MODE = false;

TEST_CASE("Fundamental WordLang Tests", "[WordLang]") {
  SECTION("Instance of the class is created successfuly") {

    cse::WordLang wordLang; // if we get past this line then we're good
    REQUIRE(true);
  }

  SECTION("Correct input line is parsed without throwing an exception") {
    cse::WordLang wordLang;
    REQUIRE_NOTHROW(wordLang.parse("PRINT ALL"));
  }

  SECTION("Incorrect input line is parsed without throwing an exception") {
    cse::WordLang wordLang;
    REQUIRE_NOTHROW(wordLang.parse("GARBAGE"));
  }
}

TEST_CASE("Reset command", "[Reset]") {
  //std::filesystem::path current = std::filesystem::current_path();
  //std::filesystem::path target = current.parent_path().parent_path() / "Group-06" / "App" / "src";
  //std::filesystem::current_path(target);
  std::stringstream buffer;
  std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

  cse::WordLang wordLang;

  SECTION("Reset with one list") {
    wordLang.parse("LIST l = LOAD \"100000.txt\n");
    wordLang.parse("LENGTH = 5\n");
    buffer.str("");
    wordLang.parse("RESET\n");

    REQUIRE(buffer.str() == "[Info]: Succesfully reset all current lists to the original state.\n\n\n");

  }

  std::cout.rdbuf(oldCout);
}


TEST_CASE("Length restriction Tests", "[WordLang]") {
  // changing path to text files from "database" folder in "App"
  std::filesystem::path current = std::filesystem::current_path();
  std::filesystem::path target = current.parent_path().parent_path() / "Group-06" / "App" / "src";
  std::filesystem::current_path(target);
  std::stringstream buffer;
  std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

  cse::WordLang wordLang;
  

  SECTION("Length of 5") {
    wordLang.parse("LIST list1 = LOAD \"top_5000\"\n");
    wordLang.parse("LENGTH = 5\n");

    REQUIRE(buffer.str() == "Loaded \"top_5000\". Word count in a list: 4354\nWords before filter: 4354, after filter: 688\n");
  }
  
  std::cout.rdbuf(oldCout);

}

TEST_CASE("WordLang Tests", "[WordLang]") {

  cse::WordLang wordLang;

  std::vector<std::string> input = {
    "LIST list1 = LOAD \"File1\"\n",
    "LIST list2 = LOAD \"File2\"\n",
    "LIST combined = COMBINED list1 list2\n",
    "LIST result = DIFFERENCE combined\n",
    "SET_CURRENT result\n",
    "LENGTH = 5\n",
    "GET \"_o__s\"\n",
    "PRINT 2\n",
    "PRINT ALL\n",
    "NOT_CONTAINS \"wet\"\n",
    "PRINT ALL\n"
  };
    
  std::vector<std::string> output_result = {
    "\n",
    "\n",
    "\n",
    "\n",
    "Number of Words To Search: 5\n",
    "\n",
    "[boats, books]\n",
    "[where, toast, boost, books, boats]\n",
    "\n",
    "[books]\n"
  };

  for (size_t i = 0; i < input.size(); ++i) {
    std::ostringstream oss;
    std::streambuf* oldCoutBuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());

    wordLang.parse(input[i]);

    std::cout.rdbuf(oldCoutBuf);

    std::string actualOutput = oss.str();
    std::string expected = output_result[i];

    // If DEBUG_MODE flag set, print out input output and Pass or Fail to test
    if(DEBUG_MODE) {
      std::cout << "Input:    " << input[i];
      std::cout << "Expected: " << expected;
      std::cout << "Actual:   " << actualOutput;
      std::cout << ((actualOutput == expected) ? "[\033[32mPASS\033[0m]\n\n" : "[\033[31mFAIL\033[0m]\n\n");
    }
    
    CHECK(actualOutput == expected);
  }

}

TEST_CASE("WordLang Tests", "[WordLang]") {

  cse::WordLang wordLang;

  std::vector<std::string> input = {
    "LIST list1 = LOAD \"top_5000_words_database\"\n",
    "LIST list2 = LOAD \"top_1000_common_worlde_words_database\"\n",
    "LIST combined = COMBINED list1 list2\n",
    "SAVE combined \"my_custom_list\"\n",
    "LENGTH = 5\n",
    "GET \"a___e\"\n",
    "CONTAINS_ANY \"qwrty\"\n",
    "CONTAINS_ALL \"jh\"\n",
    "PRINT ALL\n",
    "RESET_LAST\n"
  };
    
  std::vector<std::string> output_result = {
    "\n",
    "\n",
    "Number of Words To Search: 1200\n",
    "\n",
    "Number of Words To Search: 30\n",
    "Number of Words To Search: 6\n",
    "Number of Words To Search: 3\n",
    "[ajhqe, ajhre, ahjte]\n",
    "Number of Words To Search: 6\n"
  };

  for (size_t i = 0; i < input.size(); ++i) {
    std::ostringstream oss;
    std::streambuf* oldCoutBuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());

    wordLang.parse(input[i]);

    std::cout.rdbuf(oldCoutBuf);

    std::string actualOutput = oss.str();
    std::string expected = output_result[i];

    // If DEBUG_MODE flag set, print out input output and Pass or Fail to test
    if(DEBUG_MODE) {
      std::cout << "Input:    " << input[i];
      std::cout << "Expected: " << expected;
      std::cout << "Actual:   " << actualOutput;
      std::cout << ((actualOutput == expected) ? "[\033[32mPASS\033[0m]\n\n" : "[\033[31mFAIL\033[0m]\n\n");
    }
    
    CHECK(actualOutput == expected);
  }

}


TEST_CASE("WordLang Tests", "[WordLang]") {

  cse::WordLang wordLang;

  std::vector<std::string> input = {
    "LIST list1 = LOAD \"Ofile1\"\n",
    "LIST list2 = LOAD \"Ofile2\"\n",
    "LIST combined = COMBINED list1 list2\n",
    "ADD combined \"car printer green paper\"\n",
    "LENGTH = 5\n",
    "CONTAINS_ALL \"ae\"\n",
    "GET \"a___e\"\n",
    "PRINT ALL\n"
  };
    
  std::vector<std::string> output_result = {
    "\n",
    "\n",
    "\n",
    "\n",
    "Number of Words to search: 12\n",
    "Number of Words To Search: 4\n",
    "Number of Words To Search: 1\n",
    "[apple]\n"
  };

  for (size_t i = 0; i < input.size(); ++i) {
    std::ostringstream oss;
    std::streambuf* oldCoutBuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());

    wordLang.parse(input[i]);

    std::cout.rdbuf(oldCoutBuf);

    std::string actualOutput = oss.str();
    std::string expected = output_result[i];

    // If DEBUG_MODE flag set, print out input output and Pass or Fail to test
    if(DEBUG_MODE) {
      std::cout << "Input:    " << input[i];
      std::cout << "Expected: " << expected;
      std::cout << "Actual:   " << actualOutput;
      std::cout << ((actualOutput == expected) ? "[\033[32mPASS\033[0m]\n\n" : "[\033[31mFAIL\033[0m]\n\n");
    }
    
    CHECK(actualOutput == expected);
  }
}


TEST_CASE("Intersection and Filter Tests", "[WordLang]") {

  cse::WordLang wordLang;

  std::vector<std::string> input = {
    "LIST a = LOAD \"intersect_file1.txt\"\n",
    "LIST b = LOAD  \"intersect_file2.txt\"\n",
    "LIST c = INTERSECTION a b\n",
    "SET_CURRENT c\n",
    "NOT_CONTAINS \"n\"\n",
    "PRINT ALL\n"
  };
    
  std::vector<std::string> output_result = {
    "\n",
    "\n",
    "\n",
    "Number of Words To Search: 4\n",
    "Number of Words To Search: 4\n",
    "[fable, label, table, stable]\n"
  };

  for (size_t i = 0; i < input.size(); ++i) {
    std::ostringstream oss;
    std::streambuf* oldCoutBuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());

    wordLang.parse(input[i]);

    std::cout.rdbuf(oldCoutBuf);

    std::string actualOutput = oss.str();
    std::string expected = output_result[i];

    // If DEBUG_MODE flag set, print out input output and Pass or Fail to test
    if(DEBUG_MODE) {
      std::cout << "Input:    " << input[i];
      std::cout << "Expected: " << expected;
      std::cout << "Actual:   " << actualOutput;
      std::cout << ((actualOutput == expected) ? "[\033[32mPASS\033[0m]\n\n" : "[\033[31mFAIL\033[0m]\n\n");
    }
    
    CHECK(actualOutput == expected);
  }
}

TEST_CASE("Copy and Reset Tests", "[WordLang]") {

  cse::WordLang wordLang;

  std::vector<std::string> input = {
    "LIST original = LOAD \"reset_test.txt\"\n",
    "LIST backup = COPY original\n",
    "SET_CURRENT original\n",
    "LENGTH = 5\n",
    "CONTAINS_ALL \"ae\"\n",
    "GET \"_a__e\"\n",
    "PRINT ALL\n",
    "RESET original\n",
    "PRINT ALL"
  };
    
  std::vector<std::string> output_result = {
    "\n",
    "\n",
    "\n",
    "Number of Words To Search: 5\n",
    "Number of Words To Search: 4\n",
    "Number of Words To Search: 1\n",
    "[rathe]\n",
    "Number of Words To Search: 5\n",
    "[heart, earth, hater, rathe, eater]\n"
  };

  for (size_t i = 0; i < input.size(); ++i) {
    std::ostringstream oss;
    std::streambuf* oldCoutBuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());

    wordLang.parse(input[i]);

    std::cout.rdbuf(oldCoutBuf);

    std::string actualOutput = oss.str();
    std::string expected = output_result[i];

    // If DEBUG_MODE flag set, print out input output and Pass or Fail to test
    if(DEBUG_MODE) {
      std::cout << "Input:    " << input[i];
      std::cout << "Expected: " << expected;
      std::cout << "Actual:   " << actualOutput;
      std::cout << ((actualOutput == expected) ? "[\033[32mPASS\033[0m]\n\n" : "[\033[31mFAIL\033[0m]\n\n");
    }
    
    CHECK(actualOutput == expected);
  }
}

TEST_CASE("Wordle Command Tests", "[WordLang]") {
  cse::WordLang wordLang;

  std::vector<std::string> input = {
    "LIST list1 = LOAD \"wordle_file.txt\"\n",
    "SET_CURRENT list1\n",
    "WORDLE(\"toast\", \"GBBBG\")\n",
    "PRINT ALL\n"
  };
    
  std::vector<std::string> output_result = {
    "\n",
    "\n",
    "\n",
    "Top 10 Wordle Suggestions:\n[coast, boast, roast]\n"
  };


  for (size_t i = 0; i < input.size(); ++i) {
    std::ostringstream oss;
    std::streambuf* oldCoutBuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());

    wordLang.parse(input[i]);

    std::cout.rdbuf(oldCoutBuf);

    std::string actualOutput = oss.str();
    std::string expected = output_result[i];

    // If DEBUG_MODE flag set, print out input output and Pass or Fail to test
    if(DEBUG_MODE) {
      std::cout << "Input:    " << input[i];
      std::cout << "Expected: " << expected;
      std::cout << "Actual:   " << actualOutput;
      std::cout << ((actualOutput == expected) ? "[\033[32mPASS\033[0m]\n\n" : "[\033[31mFAIL\033[0m]\n\n");
    }
    
    CHECK(actualOutput == expected);
  }
}


TEST_CASE("WordLang Tests: Parsing", "[WordLang]") {

  SECTION("Parse LIST and LOAD command") {
    cse::WordLang wordLang;

    std::vector<std::string> input = {
      "LIST = LOAD \"File2\"\n",
      "LIST list2 LOAD \"File2\"\n",
      "LIST list2 = \"File2\"\n",
      "LIST list2 = load \"File2\"\n",
      "LIST list2 = LLOAD \"File2\"\n",
      "LIST list2 = LOAD \n",
      "LIST list2 = LOAD File2\n",
      "LIST list2 = LOAD \"File2\"\n"
    };
      
    std::vector<std::string> output_result = {
      "[Info]: Incorrect Syntax: Missing listname identifier.\n",
      "[Info]: Incorrect Syntax: Missing assign(=) operator.\n",
      "[Info]: Incorrect Syntax: Use of invalid syntax.\n",
      "[Info]: Incorrect Syntax: Use of invalid syntax.\n",
      "[Info]: Incorrect Syntax: Use of invalid syntax.\n",
      "[Info]: Incorrect Syntax: Missing filename to open.\n",
      "[Info]: Incorrect Syntax: Missing filename to open.\n",
      "[Info]: Incorrect Syntax: File can not be loaded.\n"
    };


    for (size_t i = 0; i < input.size(); ++i) {
        std::ostringstream oss;
        std::streambuf* oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());
    
        wordLang.parse(input[i]);
    
        std::cout.rdbuf(oldCoutBuf);
    
        std::string actualOutput = oss.str();
        std::string expected = output_result[i];

        // If DEBUG_MODE flag set, print out input output and Pass or Fail to test
        if(DEBUG_MODE) {
          std::cout << "Input:    " << input[i];
          std::cout << "Expected: " << expected;
          std::cout << "Actual:   " << actualOutput;
          std::cout << ((actualOutput == expected) ? "[\033[32mPASS\033[0m]\n\n" : "[\033[31mFAIL\033[0m]\n\n");
        }
        
        CHECK(actualOutput == expected);
    }
  };

  SECTION("Parse LIST COMBINE command") {
    cse::WordLang wordLang;

    std::vector<std::string> input = {
      "LIST list1 = \"File2\"\n",
      "LIST list1 = combined \"File2\"\n",
      "LIST list1 = COMBINED \n",
      "LIST list1 = COMBINED \"File2\"\n",
      "LIST list1 = COMBINED list2\n",
      "LIST list1 = COMBINED list2 list3 list4 \"list5\"\n",
    };
      
    std::vector<std::string> output_result = {
      "[Info]: Incorrect Syntax: Use of invalid syntax.\n",
      "[Info]: Incorrect Syntax: Use of invalid syntax.\n",
      "[Info]: Incorrect Syntax: There must be lists to be combined.\n",
      "[Info]: Incorrect Syntax: There must be lists to be combined.\n",
      "[Info]: Incorrect Syntax: There must be at least two lists to be combined.\n",
      "[Info]: Incorrect Syntax: There must be lists to be combined.\n"
    };

    for (size_t i = 0; i < input.size(); ++i) {
        std::ostringstream oss;
        std::streambuf* oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());
    
        wordLang.parse(input[i]);
    
        std::cout.rdbuf(oldCoutBuf);
    
        std::string actualOutput = oss.str();
        std::string expected = output_result[i];

        // If DEBUG_MODE flag set, print out input output and Pass or Fail to test
        if(DEBUG_MODE) {
          std::cout << "Input:    " << input[i];
          std::cout << "Expected: " << expected;
          std::cout << "Actual:   " << actualOutput;
          std::cout << ((actualOutput == expected) ? "[\033[32mPASS\033[0m]\n\n" : "[\033[31mFAIL\033[0m]\n\n");
        }
        
        CHECK(actualOutput == expected);
    }
  };

  SECTION("Parse LIST DIFFERENCE command") {
    cse::WordLang wordLang;

    std::vector<std::string> input = {
      "LIST list1 = \"File2\"\n",
      "LIST list1 = difference \"File2\"\n",
      "LIST list1 = DIFFERENCE \n",
      "LIST list1 = DIFFERENCE \"File2\"\n",
      "LIST list1 = DIFFERENCE list2\n",
      "LIST list1 = DIFFERENCE list2 list3 list4 \"list5\"\n",
    };
      
    std::vector<std::string> output_result = {
      "[Info]: Incorrect Syntax: Use of invalid syntax.\n",
      "[Info]: Incorrect Syntax: Use of invalid syntax.\n",
      "[Info]: Incorrect Syntax: There must be lists to find differences.\n",
      "[Info]: Incorrect Syntax: There must be lists to find differences.\n",
      "[Info]: Incorrect Syntax: There must be at least two lists to find differences.\n",
      "[Info]: Incorrect Syntax: There must be lists to find differences.\n"
    };

    for (size_t i = 0; i < input.size(); ++i) {
        std::ostringstream oss;
        std::streambuf* oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());
    
        wordLang.parse(input[i]);
    
        std::cout.rdbuf(oldCoutBuf);
    
        std::string actualOutput = oss.str();
        std::string expected = output_result[i];

        // If DEBUG_MODE flag set, print out input output and Pass or Fail to test
        if(DEBUG_MODE) {
          std::cout << "Input:    " << input[i];
          std::cout << "Expected: " << expected;
          std::cout << "Actual:   " << actualOutput;
          std::cout << ((actualOutput == expected) ? "[\033[32mPASS\033[0m]\n\n" : "[\033[31mFAIL\033[0m]\n\n");
        }
        
        CHECK(actualOutput == expected);
    }
  };

  SECTION("Parse LIST INTERSECTION command") {
    cse::WordLang wordLang;

    std::vector<std::string> input = {
      "LIST list1 = \"File2\"\n",
      "LIST list1 = intersection \"File2\"\n",
      "LIST list1 = INTERSECTION \n",
      "LIST list1 = INTERSECTION \"File2\"\n",
      "LIST list1 = INTERSECTION list2\n",
      "LIST list1 = INTERSECTION list2 list3 list4 \"list5\"\n",
    };
      
    std::vector<std::string> output_result = {
      "[Info]: Incorrect Syntax: Use of invalid syntax.\n",
      "[Info]: Incorrect Syntax: Use of invalid syntax.\n",
      "[Info]: Incorrect Syntax: There must be lists to find intersections.\n",
      "[Info]: Incorrect Syntax: There must be lists to find intersections.\n",
      "[Info]: Incorrect Syntax: There must be at least two lists to find intersections.\n",
      "[Info]: Incorrect Syntax: There must be lists to find intersections.\n"
    };

    for (size_t i = 0; i < input.size(); ++i) {
        std::ostringstream oss;
        std::streambuf* oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());
    
        wordLang.parse(input[i]);
    
        std::cout.rdbuf(oldCoutBuf);
    
        std::string actualOutput = oss.str();
        std::string expected = output_result[i];

        // If DEBUG_MODE flag set, print out input output and Pass or Fail to test
        if(DEBUG_MODE) {
          std::cout << "Input:    " << input[i];
          std::cout << "Expected: " << expected;
          std::cout << "Actual:   " << actualOutput;
          std::cout << ((actualOutput == expected) ? "[\033[32mPASS\033[0m]\n\n" : "[\033[31mFAIL\033[0m]\n\n");
        }
        
        CHECK(actualOutput == expected);
    }
  };

  SECTION("Parse LIST COPY command") {
    cse::WordLang wordLang;

    std::vector<std::string> input = {
      "LIST list1 = \"File2\"\n",
      "LIST list1 = copy list1\n",
      "LIST list1 = COPY \n",
      "LIST list1 = COPY \"File2\"\n"
    };
      
    std::vector<std::string> output_result = {
      "[Info]: Incorrect Syntax: Use of invalid syntax.\n",
      "[Info]: Incorrect Syntax: Use of invalid syntax.\n",
      "[Info]: Incorrect Syntax: Missing list name to copy.\n",
      "[Info]: Incorrect Syntax: Missing list name to copy.\n"
    };

    for (size_t i = 0; i < input.size(); ++i) {
        std::ostringstream oss;
        std::streambuf* oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());
    
        wordLang.parse(input[i]);
    
        std::cout.rdbuf(oldCoutBuf);
    
        std::string actualOutput = oss.str();
        std::string expected = output_result[i];

        // If DEBUG_MODE flag set, print out input output and Pass or Fail to test
        if(DEBUG_MODE) {
          std::cout << "Input:    " << input[i];
          std::cout << "Expected: " << expected;
          std::cout << "Actual:   " << actualOutput;
          std::cout << ((actualOutput == expected) ? "[\033[32mPASS\033[0m]\n\n" : "[\033[31mFAIL\033[0m]\n\n");
        }
        
        CHECK(actualOutput == expected);
    }
  };

  SECTION("Parse PRINT command") {
    cse::WordLang wordLang;

    std::vector<std::string> input = {
      "PRINT\n",
      "print\n",
      "PRINT all\n",
      "PRINT -1\n",
      "PRINT #\n"
    };
      
    std::vector<std::string> output_result = {
      "[Info]: Incorrect Syntax: Incorrect identifier type, must be a number or \"ALL\"\n",
      "[Info]: Incorrect Syntax: Check syntax, keyword is not found.\n",
      "[Info]: Incorrect Syntax: Incorrect identifier type, must be a number or \"ALL\"\n",
      "[Info]: Incorrect Syntax: Incorrect identifier type, must be a number or \"ALL\"\n",
      "[Info]: Incorrect Syntax: Incorrect identifier type, must be a number or \"ALL\"\n"
    };

    for (size_t i = 0; i < input.size(); ++i) {
        std::ostringstream oss;
        std::streambuf* oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());
    
        wordLang.parse(input[i]);
    
        std::cout.rdbuf(oldCoutBuf);
    
        std::string actualOutput = oss.str();
        std::string expected = output_result[i];

        // If DEBUG_MODE flag set, print out input output and Pass or Fail to test
        if(DEBUG_MODE) {
          std::cout << "Input:    " << input[i];
          std::cout << "Expected: " << expected;
          std::cout << "Actual:   " << actualOutput;
          std::cout << ((actualOutput == expected) ? "[\033[32mPASS\033[0m]\n\n" : "[\033[31mFAIL\033[0m]\n\n");
        }
        
        CHECK(actualOutput == expected);
    }
  };

  SECTION("Parse SET_CURRENT command") {
    cse::WordLang wordLang;

    std::vector<std::string> input = {
      "Set_Current\n",
      "SETCURRENT\n",
      "SET_CURRENT \"list1\"\n",
      "SET_CURRENT list1 list2 list3 \"list4\"\n"
    };
      
    std::vector<std::string> output_result = {
      "[Info]: Incorrect Syntax: Check syntax, keyword is not found.\n",
      "[Info]: Incorrect Syntax: Check syntax, keyword is not found.\n",
      "[Info]: Incorrect Syntax: There must be list type to set as current.\n",
      "[Info]: Incorrect Syntax: There must be list type to set as current.\n"
    };

    for (size_t i = 0; i < input.size(); ++i) {
        std::ostringstream oss;
        std::streambuf* oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());
    
        wordLang.parse(input[i]);
    
        std::cout.rdbuf(oldCoutBuf);
    
        std::string actualOutput = oss.str();
        std::string expected = output_result[i];

        // If DEBUG_MODE flag set, print out input output and Pass or Fail to test
        if(DEBUG_MODE) {
          std::cout << "Input:    " << input[i];
          std::cout << "Expected: " << expected;
          std::cout << "Actual:   " << actualOutput;
          std::cout << ((actualOutput == expected) ? "[\033[32mPASS\033[0m]\n\n" : "[\033[31mFAIL\033[0m]\n\n");
        }
        
        CHECK(actualOutput == expected);
    }
  };

  SECTION("Parse ADD command") {
    cse::WordLang wordLang;

    std::vector<std::string> input = {
      "add\n",
      "ADD \"test words list\"\n",
      "ADD list1 test words list\n",
      "ADD \"list1\" \"test words list\"\n"
    };
      
    std::vector<std::string> output_result = {
      "[Info]: Incorrect Syntax: Check syntax, keyword is not found.\n",
      "[Info]: Incorrect Syntax: Missing listname identifier.\n",
      "[Info]: Incorrect Syntax: Missing words to add.\n",
      "[Info]: Incorrect Syntax: Missing listname identifier.\n"
    };

    for (size_t i = 0; i < input.size(); ++i) {
        std::ostringstream oss;
        std::streambuf* oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());
    
        wordLang.parse(input[i]);
    
        std::cout.rdbuf(oldCoutBuf);
    
        std::string actualOutput = oss.str();
        std::string expected = output_result[i];

        // If DEBUG_MODE flag set, print out input output and Pass or Fail to test
        if(DEBUG_MODE) {
          std::cout << "Input:    " << input[i];
          std::cout << "Expected: " << expected;
          std::cout << "Actual:   " << actualOutput;
          std::cout << ((actualOutput == expected) ? "[\033[32mPASS\033[0m]\n\n" : "[\033[31mFAIL\033[0m]\n\n");
        }
        
        CHECK(actualOutput == expected);
    }
  };

  SECTION("Parse SAVE command") {
    cse::WordLang wordLang;

    std::vector<std::string> input = {
      "save\n",
      "SAVE \"file1\"\n",
      "SAVE list1 test\n",
      "SAVE \"list1\" \"file1\"\n"
    };
      
    std::vector<std::string> output_result = {
      "[Info]: Incorrect Syntax: Check syntax, keyword is not found.\n",
      "[Info]: Incorrect Syntax: Missing listname identifier.\n",
      "[Info]: Incorrect Syntax: Missing filename to save.\n",
      "[Info]: Incorrect Syntax: Missing listname identifier.\n"
    };

    for (size_t i = 0; i < input.size(); ++i) {
        std::ostringstream oss;
        std::streambuf* oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());
    
        wordLang.parse(input[i]);
    
        std::cout.rdbuf(oldCoutBuf);
    
        std::string actualOutput = oss.str();
        std::string expected = output_result[i];

        // If DEBUG_MODE flag set, print out input output and Pass or Fail to test
        if(DEBUG_MODE) {
          std::cout << "Input:    " << input[i];
          std::cout << "Expected: " << expected;
          std::cout << "Actual:   " << actualOutput;
          std::cout << ((actualOutput == expected) ? "[\033[32mPASS\033[0m]\n\n" : "[\033[31mFAIL\033[0m]\n\n");
        }
        
        CHECK(actualOutput == expected);
    }
  };

  SECTION("Parse LENGTH command") {
    cse::WordLang wordLang;

    std::vector<std::string> input = {
      "Length\n",
      "LENGTH \"file1\"\n",
      "LENGTH = list1\n",
      "LENGTH =\n",
      "LENGTH = \"10\"\n",
      "LENGTH = \"*\"\n",
      "LENGTH = -10\n"
    };
      
    std::vector<std::string> output_result = {
      "[Info]: Incorrect Syntax: Check syntax, keyword is not found.\n",
      "[Info]: Incorrect Syntax: Missing assign(=) operator.\n",
      "[Info]: Incorrect Syntax: Length value must be number or \"*\"\n",
      "[Info]: Incorrect Syntax: Length value must be number or \"*\"\n",
      "[Info]: Incorrect Syntax: Length value must be number or \"*\"\n",
      "[Info]: Incorrect Syntax: Length value must be number or \"*\"\n",
      "[Info]: Incorrect Syntax: Length value must be number or \"*\"\n"
    };

    for (size_t i = 0; i < input.size(); ++i) {
        std::ostringstream oss;
        std::streambuf* oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());
    
        wordLang.parse(input[i]);
    
        std::cout.rdbuf(oldCoutBuf);
    
        std::string actualOutput = oss.str();
        std::string expected = output_result[i];

        // If DEBUG_MODE flag set, print out input output and Pass or Fail to test
        if(DEBUG_MODE) {
          std::cout << "Input:    " << input[i];
          std::cout << "Expected: " << expected;
          std::cout << "Actual:   " << actualOutput;
          std::cout << ((actualOutput == expected) ? "[\033[32mPASS\033[0m]\n\n" : "[\033[31mFAIL\033[0m]\n\n");
        }
        
        CHECK(actualOutput == expected);
    }
  };

  SECTION("Parse CONTAINS_ANY command") {
    cse::WordLang wordLang;

    std::vector<std::string> input = {
      "Contains_any\n",
      "CONTAINS_ANY\n",
      "CONTAINS_ANY list1\n"
    };
      
    std::vector<std::string> output_result = {
      "[Info]: Incorrect Syntax: Check syntax, keyword is not found.\n",
      "[Info]: Incorrect Syntax: CONTAINS_ANY must have string of letters.\n",
      "[Info]: Incorrect Syntax: CONTAINS_ANY must have string of letters.\n"
    };

    for (size_t i = 0; i < input.size(); ++i) {
        std::ostringstream oss;
        std::streambuf* oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());
    
        wordLang.parse(input[i]);
    
        std::cout.rdbuf(oldCoutBuf);
    
        std::string actualOutput = oss.str();
        std::string expected = output_result[i];

        // If DEBUG_MODE flag set, print out input output and Pass or Fail to test
        if(DEBUG_MODE) {
          std::cout << "Input:    " << input[i];
          std::cout << "Expected: " << expected;
          std::cout << "Actual:   " << actualOutput;
          std::cout << ((actualOutput == expected) ? "[\033[32mPASS\033[0m]\n\n" : "[\033[31mFAIL\033[0m]\n\n");
        }
        
        CHECK(actualOutput == expected);
    }
  };

  SECTION("Parse CONTAINS_ALL command") {
    cse::WordLang wordLang;

    std::vector<std::string> input = {
      "Contains_all\n",
      "CONTAINS_ALL\n",
      "CONTAINS_ALL list1\n"
    };
      
    std::vector<std::string> output_result = {
      "[Info]: Incorrect Syntax: Check syntax, keyword is not found.\n",
      "[Info]: Incorrect Syntax: CONTAINS_ALL must have string of letters.\n",
      "[Info]: Incorrect Syntax: CONTAINS_ALL must have string of letters.\n"
    };

    for (size_t i = 0; i < input.size(); ++i) {
        std::ostringstream oss;
        std::streambuf* oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());
    
        wordLang.parse(input[i]);
    
        std::cout.rdbuf(oldCoutBuf);
    
        std::string actualOutput = oss.str();
        std::string expected = output_result[i];

        // If DEBUG_MODE flag set, print out input output and Pass or Fail to test
        if(DEBUG_MODE) {
          std::cout << "Input:    " << input[i];
          std::cout << "Expected: " << expected;
          std::cout << "Actual:   " << actualOutput;
          std::cout << ((actualOutput == expected) ? "[\033[32mPASS\033[0m]\n\n" : "[\033[31mFAIL\033[0m]\n\n");
        }
        
        CHECK(actualOutput == expected);
    }
  };

  SECTION("Parse NOT_CONTAINS command") {
    cse::WordLang wordLang;

    std::vector<std::string> input = {
      "Not_Contains\n",
      "NOT_CONTAINS\n",
      "NOT_CONTAINS list1\n"
    };
      
    std::vector<std::string> output_result = {
      "[Info]: Incorrect Syntax: Check syntax, keyword is not found.\n",
      "[Info]: Incorrect Syntax: NOT_CONTAINS must have string of letters.\n",
      "[Info]: Incorrect Syntax: NOT_CONTAINS must have string of letters.\n"
    };

    for (size_t i = 0; i < input.size(); ++i) {
        std::ostringstream oss;
        std::streambuf* oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());
    
        wordLang.parse(input[i]);
    
        std::cout.rdbuf(oldCoutBuf);
    
        std::string actualOutput = oss.str();
        std::string expected = output_result[i];

        // If DEBUG_MODE flag set, print out input output and Pass or Fail to test
        if(DEBUG_MODE) {
          std::cout << "Input:    " << input[i];
          std::cout << "Expected: " << expected;
          std::cout << "Actual:   " << actualOutput;
          std::cout << ((actualOutput == expected) ? "[\033[32mPASS\033[0m]\n\n" : "[\033[31mFAIL\033[0m]\n\n");
        }
        
        CHECK(actualOutput == expected);
    }
  };

  SECTION("Parse GET command") {
    cse::WordLang wordLang;

    std::vector<std::string> input = {
      "get\n",
      "GET\n",
      "GET __o_s\n",
      "GET pattern_*\n"
    };
      
    std::vector<std::string> output_result = {
      "[Info]: Incorrect Syntax: Check syntax, keyword is not found.\n",
      "[Info]: Incorrect Syntax: GET must have string of letters or wildcards.\n",
      "[Info]: Incorrect Syntax: GET must have string of letters or wildcards.\n",
      "[Info]: Incorrect Syntax: GET must have string of letters or wildcards.\n"
    };

    for (size_t i = 0; i < input.size(); ++i) {
        std::ostringstream oss;
        std::streambuf* oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());
    
        wordLang.parse(input[i]);
    
        std::cout.rdbuf(oldCoutBuf);
    
        std::string actualOutput = oss.str();
        std::string expected = output_result[i];

        // If DEBUG_MODE flag set, print out input output and Pass or Fail to test
        if(DEBUG_MODE) {
          std::cout << "Input:    " << input[i];
          std::cout << "Expected: " << expected;
          std::cout << "Actual:   " << actualOutput;
          std::cout << ((actualOutput == expected) ? "[\033[32mPASS\033[0m]\n\n" : "[\033[31mFAIL\033[0m]\n\n");
        }
        
        CHECK(actualOutput == expected);
    }
  };

  SECTION("Parse RESET command") {
    cse::WordLang wordLang;

    std::vector<std::string> input = {
      "reset\n",
      "RESET \"lsit1\"\n",
      "RESET LOAD\n",
      "RESET list2 \"list3\"\n"
    };
      
    std::vector<std::string> output_result = {
      "[Info]: Incorrect Syntax: Check syntax, keyword is not found.\n",
      "[Info]: Incorrect Syntax: encountered unknown symbols in RESET statement\n",
      "[Info]: Incorrect Syntax: encountered unknown symbols in RESET statement\n",
      "[Info]: List list2 does not exist\n"
    };

    for (size_t i = 0; i < input.size(); ++i) {
        std::ostringstream oss;
        std::streambuf* oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());
    
        wordLang.parse(input[i]);
    
        std::cout.rdbuf(oldCoutBuf);
    
        std::string actualOutput = oss.str();
        std::string expected = output_result[i];

        // If DEBUG_MODE flag set, print out input output and Pass or Fail to test
        if(DEBUG_MODE) {
          std::cout << "Input:    " << input[i];
          std::cout << "Expected: " << expected;
          std::cout << "Actual:   " << actualOutput;
          std::cout << ((actualOutput == expected) ? "[\033[32mPASS\033[0m]\n\n" : "[\033[31mFAIL\033[0m]\n\n");
        }
        
        CHECK(actualOutput == expected);
    }
  };

  SECTION("Parse RESET_LAST command") {
    cse::WordLang wordLang;

    std::vector<std::string> input = {
      "reset_last\n",
      "RESET_LAST file1\n"
    };
      
    std::vector<std::string> output_result = {
      "[Info]: Incorrect Syntax: Check syntax, keyword is not found.\n",
      "[Info]: Incorrect Syntax: Encountered unknown symbols after \"RESET_LAST\" token.\n"
    };

    for (size_t i = 0; i < input.size(); ++i) {
        std::ostringstream oss;
        std::streambuf* oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());
    
        wordLang.parse(input[i]);
    
        std::cout.rdbuf(oldCoutBuf);
    
        std::string actualOutput = oss.str();
        std::string expected = output_result[i];

        // If DEBUG_MODE flag set, print out input output and Pass or Fail to test
        if(DEBUG_MODE) {
          std::cout << "Input:    " << input[i];
          std::cout << "Expected: " << expected;
          std::cout << "Actual:   " << actualOutput;
          std::cout << ((actualOutput == expected) ? "[\033[32mPASS\033[0m]\n\n" : "[\033[31mFAIL\033[0m]\n\n");
        }
        
        CHECK(actualOutput == expected);
    }
  };

  SECTION("Parse WORDLE command") {
    cse::WordLang wordLang;

    std::vector<std::string> input = {
      "wordle (\"print\",\"bbgyb\")\n",
      "WORDLE \"print\",\"bbgyb\"\n",
      "WORDLE (print,\"bbgyb\")\n",
      "WORDLE (\"print\" \"bbgyb\")\n",
      "WORDLE (\"print\", bbgyb)\n",
      "WORDLE (\"print\",\"bbgyb\"\n",
      "WORDLE (\"print\",\"bbgyb\") LOAD\n"
    };

    std::vector<std::string> output_result = {
      "[Info]: Incorrect Syntax: Check syntax, keyword is not found.\n",
      "[Info]: Incorrect Syntax: Wordle command must have \'(\'.\n",
      "[Info]: Incorrect Syntax: Missing word inputted to the wordle.\n",
      "[Info]: Incorrect Syntax: Missing \',\' between two strings.\n",
      "[Info]: Incorrect Syntax: Missing result string from wordle.\n",
      "[Info]: Incorrect Syntax: Wordle command must have \')\' at the end.\n",
      "[Info]: Incorrect Syntax: Encountered unknown symbols after \"WORDLE\" token.\n"
    };

    for (size_t i = 0; i < input.size(); ++i) {
        std::ostringstream oss;
        std::streambuf* oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());

        wordLang.parse(input[i]);

        std::cout.rdbuf(oldCoutBuf);

        std::string actualOutput = oss.str();
        std::string expected = output_result[i];

        // If DEBUG_MODE flag set, print out input output and Pass or Fail to test
        if(DEBUG_MODE) {
          std::cout << "Input:    " << input[i];
          std::cout << "Expected: " << expected;
          std::cout << "Actual:   " << actualOutput;
          std::cout << ((actualOutput == expected) ? "[\033[32mPASS\033[0m]\n\n" : "[\033[31mFAIL\033[0m]\n\n");
        }

        CHECK(actualOutput == expected);
    }
  };  
}
TEST_CASE("setLengthRestriction Tests", "[WordListManager]") {
  cse::ErrorManager errorManager;
  cse::WordListManager manager(errorManager);

  SECTION("Valid length restriction") {
      REQUIRE(manager.setLengthRestriction("5") == true);
  }

  SECTION("Invalid length restriction (negative number)") {
      REQUIRE(manager.setLengthRestriction("-1") == false);
  }

  SECTION("Invalid length restriction (non-numeric)") {
      REQUIRE(manager.setLengthRestriction("abc") == false);
  }

  SECTION("Wildcard length restriction") {
      REQUIRE(manager.setLengthRestriction("*") == true);
  }
}

TEST_CASE("setCurrent Tests", "[WordListManager]") {
  /* Currently commented out as the load list function which is required for this test isn't finished yet
  cse::ErrorManager errorManager;
  cse::WordListManager manager(errorManager);

  manager.loadList("list1", "file1.txt");
  manager.loadList("list2", "file2.txt");

  SECTION("Set current with valid lists") {
      REQUIRE(manager.setCurrent({"list1", "list2"}) == true);
  }

  SECTION("Set current with non-existent list") {
      REQUIRE(manager.setCurrent({"list1", "nonexistent"}) == false);
  }

  SECTION("Set current with empty list") {
      REQUIRE(manager.setCurrent({}) == true);
  }*/
}

TEST_CASE("save Tests", "[WordListManager]") {
  /* Currently commented out as the load list function which is required for this test isn't finished yet
  cse::ErrorManager errorManager;
  cse::WordListManager manager(errorManager);

  manager.loadList("list1", "file1.txt");

  SECTION("Save existing list to a file") {
      REQUIRE(manager.save("output.txt", "list1") == true);
  }

  SECTION("Save non-existent list to a file") {
      REQUIRE(manager.save("output.txt", "nonexistent") == false);
  }*/ 
}

TEST_CASE("add Tests", "[WordListManager]") {
  /* Currently commented out as the load list function which is required for this test isn't finished yet
  cse::ErrorManager errorManager;
  cse::WordListManager manager(errorManager);

  manager.loadList("list1", "file1.txt");

  SECTION("Add words to an existing list") {
      REQUIRE(manager.add("list1", "word1 word2 word3") == true);
  }

  SECTION("Add words to a non-existent list") {
      REQUIRE(manager.add("nonexistent", "word1 word2 word3") == false);
  }

  SECTION("Add empty words to an existing list") {
      REQUIRE(manager.add("list1", "") == true);
  }
      */
}


TEST_CASE("LOAD and PRINT tests", "[WordLang]") {
  SECTION("LIST LOAD and PRINT command") {
    cse::WordLang wordLang;

    std::vector<std::string> input = {
      "LIST list1 = LOAD \"file1.txt\"\n",
      "LIST list2 = LOAD \"file2.txt\"\n",
      "PRINT ALL\n",
      "SET_CURRENT list1\n",
      "PRINT ALL\n",
      "PRINT 5\n"
    };
      
    std::vector<std::string> output_result = {
      "Loaded \"file1.txt\". Word count in a list: 11\n",
      "Loaded \"file2.txt\". Word count in a list: 11\n",
      "[hello, cost, host, best, cucumber, dump, kitchen, toast, chain, boats, key]\n",
      "",
      "[hello, cost, host, best, cucumber, dump, kitchen, toast, chain, boats, key]\n",
      "[hello, cost, host, best, cucumber, dump]\n"
    };


    for (size_t i = 0; i < input.size(); ++i) {
        std::ostringstream oss;
        std::streambuf* oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(oss.rdbuf());
    
        wordLang.parse(input[i]);
    
        std::cout.rdbuf(oldCoutBuf);
    
        std::string actualOutput = oss.str();
        std::string expected = output_result[i];

        // If DEBUG_MODE flag set, print out input output and Pass or Fail to test
        if(DEBUG_MODE) {
          std::cout << "Input:    " << input[i];
          std::cout << "Expected: " << expected;
          std::cout << "Actual:   " << actualOutput;
          std::cout << ((actualOutput == expected) ? "[\033[32mPASS\033[0m]\n\n" : "[\033[31mFAIL\033[0m]\n\n");
        }
        
        CHECK(actualOutput == expected);
    }
  };
}

/*
TEST_CASE("Combine lists and print result", "[Combine]") {
    cse::ErrorManager errorManager;
    cse::WordListManager manager(errorManager);
    cse::WordLang wordLang;

    wordLang.parse("LIST a = LOAD \"FileA.txt\"\n");
    wordLang.parse("LIST b = LOAD \"FileB.txt\"\n");
    wordLang.parse("LIST c = COMBINED a b\n");
    wordLang.parse("SET_CURRENT c\n");

    std::ostringstream oss;
    std::streambuf* oldCoutBuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());

    wordLang.parse("PRINT ALL\n");

    std::cout.rdbuf(oldCoutBuf);
    std::string output = oss.str();

    CHECK(output == "[table, computer, yellow, blue]\n");  // Replace with actual expected output
}

TEST_CASE("Difference between lists", "[Difference]") {
    cse::ErrorManager errorManager;
    cse::WordListManager manager(errorManager);
    cse::WordLang wordLang;

    wordLang.parse("LIST a = LOAD \"FileA.txt\"\n");
    wordLang.parse("LIST b = LOAD \"FileB.txt\"\n");
    wordLang.parse("LIST diff = DIFFERENCE a b\n");
    wordLang.parse("SET_CURRENT diff\n");

    std::ostringstream oss;
    std::streambuf* oldCoutBuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());

    wordLang.parse("PRINT ALL\n");

    std::cout.rdbuf(oldCoutBuf);
    std::string output = oss.str();

    CHECK(output == "[table, computer]\n");
}

TEST_CASE("Intersection of lists", "[Intersection]") {
    cse::ErrorManager errorManager;
    cse::WordListManager manager(errorManager);
    cse::WordLang wordLang;

    wordLang.parse("LIST a = LOAD \"FileA.txt\"\n");
    wordLang.parse("LIST b = LOAD \"FileC.txt\"\n");
    wordLang.parse("LIST common = INTERSECTION a b\n");
    wordLang.parse("SET_CURRENT common\n");

    std::ostringstream oss;
    std::streambuf* oldCoutBuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());

    wordLang.parse("PRINT ALL\n");

    std::cout.rdbuf(oldCoutBuf);
    std::string output = oss.str();

    CHECK(output == "[table]\n");
}

TEST_CASE("Copy list", "[Copy]") {

    cse::ErrorManager errorManager;
    cse::WordListManager manager(errorManager);
    cse::WordLang wordLang;

    wordLang.parse("LIST a = LOAD \"FileA.txt\"\n");
    wordLang.parse("LIST b = COPY a\n");
    wordLang.parse("SET_CURRENT b\n");

    std::ostringstream oss;
    std::streambuf* oldCoutBuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());

    wordLang.parse("PRINT ALL\n");

    std::cout.rdbuf(oldCoutBuf);
    std::string output = oss.str();

    CHECK(output == "[table, computer]\n");
}
*/