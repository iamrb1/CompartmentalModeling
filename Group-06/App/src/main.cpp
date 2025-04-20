#include <iostream>
#include "WordLangApp/WordLang.hpp"

int main() {
    std::cout << "START:\n";
    cse::WordLang wordLang;
    wordLang.start();

   
    // std::vector<std::string> input = {
    //     "LIST list1 = LOAD \"top_1000_common_worlde_words_database\"\n",
    //     "LIST list2 = LOAD \"wordleTestList.txt\"\n",
    //     "LIST combined = COMBINED list1 list2\n",
    //     "WORDLE (\"whale\",\"bbbbb\")\n",
    //     "WORDLE (\"pilot\",\"bybgb\")\n",
    //     "WORDLE (\"union\",\"bgygb\")\n",
    //     "WORDLE (\"infos\",\"ggbgb\")\n"
    //   };
      
    //   std::vector<std::string> output_result = {
    //     "Loaded \"top_1000_common_worlde_words_database\". Word count in a list: 1000\n",
    //     "Loaded \"wordleTestList.txt\". Word count in a list: 4369\n",
    //     "Number of words to search: 4863\n",
    //     "[point, firms, onion, costs, rocks, sunny, corps, donor, irony, midst, intro]\n",
    //     "[inion, inbox, incog, union, onion, infos, brios]\n",
    //     "[union, onion]\n",
    //     "[undue]\n"
    //   };

    // for (size_t i = 0; i < input.size(); ++i) {
    //     std::ostringstream oss;
    //     std::streambuf* oldCoutBuf = std::cout.rdbuf();
    //     std::cout.rdbuf(oss.rdbuf());
    
    //     wordLang.parse(input[i]);
    
    //     std::cout.rdbuf(oldCoutBuf);
    
    //     std::string actualOutput = oss.str();
    //     std::string expected = output_result[i];

    //     // If DEBUG_MODE flag set, print out input output and Pass or Fail to test
        
    //     std::cout << "Input:    " << input[i];
    //     std::cout << "Expected: " << expected;
    //     std::cout << "Actual:   " << actualOutput;
    //     std::cout << ((actualOutput == expected) ? "[\033[32mPASS\033[0m]\n\n" : "[\033[31mFAIL\033[0m]\n\n");
    // }
}