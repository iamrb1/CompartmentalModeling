#include <iostream>
#include "WordLangApp/WordLang.hpp"

int main() {
    std::cout << "START:\n";
    cse::WordLang wordLang;
    //wordLang.start();
    
    //wordLang.parse("LIST list1 = LOAD \"file1.txt\"");
    std::vector<std::string> input = {
        "LIST list1 = LOAD \"wordle_file.txt\"\n",
        "SET_CURRENT list1\n",
        "WORDLE(\"toast\", \"ygggg\")\n"
      };
    
      std::vector<std::string> output_result = {
        "Loaded \"wordle_file.txt\". Word count in a list: 7\n",
        "",
        "[coast, boast, roast]\n"
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
        
        std::cout << "Input:    " << input[i];
        std::cout << "Expected: " << expected;
        std::cout << "Actual:   " << actualOutput;
        std::cout << ((actualOutput == expected) ? "[\033[32mPASS\033[0m]\n\n" : "[\033[31mFAIL\033[0m]\n\n");
    }


    // /// ANOTHER TEST
    // wordLang.parse("LIST list1 = LOAD \"top_1000_wordle.txt\"");
    // wordLang.parse("LIST list2 = LOAD \"top_5000.txt\"");
    // wordLang.parse("LIST list3 = COMBINED list1 list2");
    // // wordLang.parse("LIST list3 = LOAD \"random.txt\"");
    // // wordLang.parse("LIST list4 = LOAD \"100000.txt\"");
    // // wordLang.parse("LIST list5 = COMBINED list1 list2 list3 list4");
    
    
    // //wordLang.parse("LIST list1 = LOAD \"wordle1.txt\"");
    // wordLang.parse("PRINT 20");
    // wordLang.parse("LENGTH = 5");
    // wordLang.parse("PRINT 20");
    // wordLang.parse("WORDLE (\"whale\",\"bbyyb\")");
    // //wordLang.parse("PRINT 10");
    // wordLang.parse("WORDLE (\"solar\",\"bgygy\")");
    //wordLang.parse("PRINT 10");


    // TEST111111
    // wordLang.parse("LIST list1 = LOAD \"top_1000_wordle\"");
    // wordLang.parse("LIST list2 = LOAD \"top_5000\"");
    // wordLang.parse("LIST list3 = COMBINED list1 list2");

    // wordLang.parse("PRINT 20");
    // wordLang.parse("LENGTH = 5");
    // wordLang.parse("PRINT 20");
    // wordLang.parse("LENGTH = *");
    // wordLang.parse("PRINT 20");
    // wordLang.parse("LENGTH = 10");
    // wordLang.parse("PRINT 20");
    // TEST111111 END



}