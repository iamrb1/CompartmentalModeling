#include <iostream>
#include "WordLangApp/WordLang.hpp"

int main() {
    std::cout << "START:\n";
    cse::WordLang wordLang;
    //wordLang.start();
    wordLang.parse("LIST list1 = LOAD \"top_5000\"");
    wordLang.parse("PRINT 10");
    std::cout << "Finished parsing. Press Enter to exit.\n";
    std::cin.get();

}