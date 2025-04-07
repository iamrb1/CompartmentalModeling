#include "WordLang.hpp"
#include "../../../StringSet/StringSet.hpp"
#include "../../../StaticString/StaticString.hpp"
#include "../../../ErrorManager/ErrorManager.hpp"

#include "FileSource.hpp"
#include "lexer.hpp"
#include <istream>
#include <vector>

void cse::WordLang::start() {
    //Initialize commandLine

    // Call a function that takes commandline and assigns functions and configure it

    // Assign WordListManager this handles all the backend keeping track of lists
    // Current lists, takes input returns results.

    // In loop until broken by user,
    // get input from the user feed the input into Parse function in WordLang
    // WordListManager and CommandLine should be member variable assigned at the 
    // Construction which allows Parse to access it whenever needed.

    // In the loop call parse assign to a string or a cout returned value if has
    // If input EXIT break the program and close

    std::cout << "Welcome to WordLang! Type your query below:\n";

    mIsActive = true;
    std::string input;

    //cse::StringSet<cse::StaticString<20>> set = FileSource::load_file("top_1000_worlde.txt");
    //std::cout << "Words loaded: " << set.size() << "\n";
    while (mIsActive) {
        std::cout << ">>> ";
        std::getline(std::cin, input);

        if (input == "EXIT") {
            mIsActive = false;
            break;
        }
        
        parse(input);

        std::cout << "You wrote: " << input << std::endl;
    }
}

cse::WordLang::WordLang() {

}

cse::WordLang::~WordLang() {
    
}

void cse::WordLang::parse(const std::string& input) {
    /* Each parse function should check syntax, required variables, if the given 
        List name exists or file can open etc.

        If no error, calls WordListManager to do the operations and couts if necessary 
        no return required in this case since ther might be type issues.
    */
    mTokenManager.Load(input);

    // for (auto t : tokens) {
    //     std::cout << "Lexeme: " << t.lexeme << ", ID: " << t.id << ", ID_Name: " << emplex::Lexer::TokenName(t.id) << "\n";
    // }

    switch (mTokenManager.Peek()) {
        using namespace emplex;
        case Lexer::ID_LIST: {
            parseList();
            break;
        }
        case Lexer::ID_SET_CURRENT: {
        break;
        }
        case Lexer::ID_ADD: {
        break;
        }
        case Lexer::ID_SAVE: {
        break;
        }
        case Lexer::ID_LENGTH: {
        break;
        }
        case Lexer::ID_CONTAINS_ANY: {
        break;
        }
        case Lexer::ID_CONTAINS_ALL: {
        break;
        }
        case Lexer::ID_NOT_CONTAINS: {
        break;
        }
        case Lexer::ID_GET: {
        break;
        }
        case Lexer::ID_PRINT: {
            parsePrint();
            break;
        }
        case Lexer::ID_RESET: {
        break;
        }
        case Lexer::ID_RESET_LAST: {
        break;
        }
        case Lexer::ID_WORDLE: {
        break;
        }
        default:{
        break;
        }
    }
}

void cse::WordLang::parseList() {
    mTokenManager.Use();

    auto listname = mTokenManager.Use();

    // Check if a listname provided
    if(listname == mTokenManager.eof_token || listname != emplex::Lexer::ID_LISTNAME) {
        // Print syntax error 
        mErrorManager.printInfo("Incorrect Syntax: Missing listname identifier.");
        return;
    }

    // Check if = is used 
    if(mTokenManager.Use_if(232) == mTokenManager.eof_token ) {
        // Print syntax error 
        mErrorManager.printInfo("Incorrect Syntax: Missing assign(=) operator.");
        return;
    }

    auto list_identifier = mTokenManager.Use();

    if(list_identifier.id == 253) {          // Load the list
        auto filename = mTokenManager.Use();

        if(filename == mTokenManager.eof_token || filename != emplex::Lexer::ID_STRING) { // Check file name
            // Missing file name to open
            mErrorManager.printInfo("Incorrect Syntax: Missing filename to open.");
            return;
        }
        // Syntax is correct, call WordListManager to handle. WIth file name and list name to load
        std::string cut_name = filename.lexeme.substr(1,filename.lexeme.length() - 2);
        if (!mWordListManager.load_list(listname.lexeme, cut_name)) {
            mErrorManager.printInfo("Incorrect Syntax: File can not be loaded.");
            return;
        }

    } else if (list_identifier.id == 252) {  // Combine the list
        std::vector<std::string> lists_to_combine = parseMultipleLists();

        // Error occured, syntax is wrong 
        if(lists_to_combine.size() == 0) {
            return;
        }
        // Call WordListManager to handle with listname, lists_to_combine to combine

    } else if (list_identifier.id == 251) {  // Difference the lists
        std::vector<std::string> lists_to_difference = parseMultipleLists();

        // Error occured, syntax is wrong 
        if(lists_to_difference.size() == 0) {
            return;
        }
        // Call WordListManager to handle with listname, lists_to_difference to difference

    } else if (list_identifier.id == 250) {  // Intersection the lists
        std::vector<std::string> lists_to_intersection = parseMultipleLists();

        // Error occured, syntax is wrong 
        if(lists_to_intersection.size() == 0) {
            return;
        }
        // Call WordListManager to handle with listname, lists_to_intersection to intersection

    } else {
        // Syntax Error 
        mErrorManager.printInfo("Incorrect Syntax: Use on invalid syntax.");
        return;
    }
}

std::vector<std::string> cse::WordLang::parseMultipleLists() {
    std::vector<std::string> lists_to_combine;

    while(mTokenManager.Peek() != mTokenManager.eof_token) {
        if(mTokenManager.Peek() == emplex::Lexer::ID_LISTNAME) {
            auto list = mTokenManager.Use();
            lists_to_combine.push_back(list.lexeme); // Add list name to collection
        } else {
            // Syntax Error 
            mErrorManager.printInfo("Incorrect Syntax: Incorrect identifier type, must be an exisiting listname.");
            lists_to_combine.clear(); 
            return lists_to_combine;
        }
    }

    return lists_to_combine;
}

void cse::WordLang::parsePrint() {
    mTokenManager.Use();

    auto identifier = mTokenManager.Use();

    if(identifier.id == 236) { // PRINT ALL
        mWordListManager.print(0,true);

    } else if(identifier.id == 234) { // PRINT X number
        mWordListManager.print(std::stoi(identifier.lexeme));

    } else {
        mErrorManager.printInfo("Incorrect Syntax: Incorrect identifier type, must be a number or \"ALL\"");
        return;
    }
}


// #include "WordLang.hpp"
// #include "../../../StringSet/StringSet.hpp"
// #include "../../../StaticString/StaticString.hpp"
// #include "../../../CommandLine/CommandLine.cpp"
// #include "../../../ErrorManager/ErrorManager.hpp"
// #include "FileSource.hpp"
// #include "WordListManager.hpp"
// //#include <ranges>
// //#include <algorithm>
// //#include <iostream>
// //#include <string>

// void WordLang::start() {
//     //Initialize commandLine
//     cse::CommandLine cl;
//     cse::ErrorManager errorManager;
//     cse::WordListManager wordManager;
//     // Call a function that takes commandline and assigns functions and configure it

//     // Assign WordListManager this handles all the backend keeping track of lists
//     // Current lists, takes input returns results.

//     // In loop until broken by user,
//     // get input from the user feed the input into Parse function in WordLang
//     // WordListManager and CommandLine should be member variable assigned at the 
//     // Construction which allows Parse to access it whenever needed.

//     // In the loop call parse assign to a string or a cout returned value if has
//     // If input EXIT break the program and close

//     std::cout << "Welcome to WordLang! Type your query below:\n";

//     mIsActive = true;
//     std::string input;

//     //cse::StringSet<cse::StaticString<20>> set = FileSource::load_file("top_5000.txt");
//     //std::cout << "Words loaded: " << set.size() << "\n";
//     while (mIsActive) {
//         std::cout << ">>> ";
//         std::getline(std::cin, input);

//         auto words = cl.tokenize_line(input);
        
//         if (words[0] == "LIST") {
//             if (words.size() != 5 || words[2] != "=" || words[3] != "LOAD") {
//                 errorManager.printInfo("Usage: LIST <name> = LOAD <filename>");
//                 continue;
//             }
//             std::string listname = words[1];
//             std::string filename = words[4];
//             // LIST name = load filename
//             wordManager.load_list(words[1], words[4]);
//         }
//         if (words[0] == "EXIT") {
//             mIsActive = false;
//         }
//     }
// }

// WordLang::WordLang() {

// }

// WordLang::~WordLang() {
    
// }


// void WordLang::Parse(const std::string& input) {
//     // In a switch tokenize the inputs to see which commands are called

//     // Call command specific parse function
//     /* Each parse function should check syntax, required variables, if the given 
//        List name exists or file can open etc.

//        If no error, calls WordListManager to do the operations and couts if necessary 
//        no return required in this case since ther might be type issues.

       
//     */
// }