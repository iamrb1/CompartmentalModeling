#include <iostream>
#include "WordLangApp/WordLang.hpp"

int main() {
    std::cout << "START:\n";
    cse::WordLang wordLang;
    //wordLang.start();
    // wordLang.parse("LIST list1 = LOAD \"top_5000\"");
    // wordLang.parse("PRINT 10");
    // wordLang.parse("LIST list1 = COMBINED list2");
    // wordLang.parse("LIST list1 = DIFFERENCE list2");
    // wordLang.parse("LIST list1 = INTERSECTION list2");
    // wordLang.parse("LIST list1 = COPY list2");

    // wordLang.parse("LIST list1 = LOAD");
    // wordLang.parse("LIST list1 = COMBINED");
    // wordLang.parse("LIST list1 = DIFFERENCE");
    // wordLang.parse("LIST list1 = INTERSECTION");
    // wordLang.parse("LIST list1 = COPY");

    // wordLang.parse("LIST list1 = LOAD COPY");
    // wordLang.parse("LIST list1 = COMBINED \"string\"");
    // wordLang.parse("LIST list1 = DIFFERENCE \"string\" \"string2\"");
    // wordLang.parse("LIST list1 = INTERSECTION \"string\" \"string2\"");
    // wordLang.parse("LIST list1 = COPY \"string\"");

    // wordLang.parse("LIST list1 = LOAD list2");
    // wordLang.parse("LIST list1 = COMBINED list2 \"string\"");
    // wordLang.parse("LIST list1 = DIFFERENCE list2 \"string\" \"string2\"");
    // wordLang.parse("LIST list1 = INTERSECTION list2 \"string\" \"string2\"");
    // wordLang.parse("LIST list1 = COPY list2 \"string\"");

    // wordLang.parse("SET_CURRENT list1");
    // wordLang.parse("SET_CURRENT list1 list2");
    // wordLang.parse("SET_CURRENT list1 list2 list3");
    // wordLang.parse("SET_CURRENT \"list1\"");
    // wordLang.parse("SET_CURRENT COPY");
    // wordLang.parse("SET_CURRENT");
    // wordLang.parse("SET_CURRENT list1 list2 list3 \"list4\"");
    // wordLang.parse("SET_CURRENT list1 list2 list3 COPY");

    // wordLang.parse("ADD list1 \"word add to list those\"");
    // wordLang.parse("ADD list1 \"word\"");
    // wordLang.parse("ADD list1 words to add into list");
    // wordLang.parse("ADD");
    // wordLang.parse("ADD \"list1\" \"word add to list those\"");
    // wordLang.parse("ADD list1 COPY");

    // wordLang.parse("SAVE list1 \"file1\"");
    // wordLang.parse("SAVE list1 \"file2\"");
    // wordLang.parse("SAVE list1 file1");
    // wordLang.parse("SAVE");
    // wordLang.parse("SAVE \"list1\" \"file1\"");
    // wordLang.parse("SAVE list1 COPY");

    // wordLang.parse("LENGTH = 5");
    // wordLang.parse("LENGTH = 0");
    // wordLang.parse("LENGTH = 20");
    // wordLang.parse("LENGTH = *");
    // wordLang.parse("LENGTH");
    // wordLang.parse("LENGTH = string");
    // wordLang.parse("LENGTH = \"WRONG\"");
    // wordLang.parse("LENGTH = COPY");

    // wordLang.parse("CONTAINS_ANY \"ASd\"");
    // wordLang.parse("CONTAINS_ANY \"A\"");
    // wordLang.parse("CONTAINS_ANY ASd");
    // wordLang.parse("CONTAINS_ANY");
    // wordLang.parse("CONTAINS_ANY ALL");
    // wordLang.parse("CONTAINS_ANY *");
    // wordLang.parse("CONTAINS_ANY 5");
    // wordLang.parse("CONTAINS_ANY \"567990\"");

    std::cout << "Finished parsing. Press Enter to exit.\n";
    std::cin.get();

}