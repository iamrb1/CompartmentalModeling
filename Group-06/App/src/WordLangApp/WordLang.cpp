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
            parseSetCurrent();
            break;
        }
        case Lexer::ID_ADD: {
            parseAdd();
            break;
        }
        case Lexer::ID_SAVE: {
            parseSave();
            break;
        }
        case Lexer::ID_LENGTH: {
            parseLength();
            break;
        }
        case Lexer::ID_CONTAINS_ANY: {
            parseContainsAny();
            break;
        }
        case Lexer::ID_CONTAINS_ALL: {
            parseContainsAll();
            break;
        }
        case Lexer::ID_NOT_CONTAINS: {
            parseNotContains();
            break;
        }
        case Lexer::ID_GET: {
            parseGet();
            break;
        }
        case Lexer::ID_PRINT: {
            parsePrint();
            break;
        }
        case Lexer::ID_RESET: {
            parseReset();
            break;
        }
        case Lexer::ID_RESET_LAST: {
            parseResetLast();
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

    auto listIdentifier = mTokenManager.Use();

    if(listIdentifier.id == 253) {          // Load the list
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

    } else if (listIdentifier.id == 252) {  // Combine the list
        std::vector<std::string> listsToCombine = parseMultipleLists();

        // Error occured, syntax is wrong 
        if(listsToCombine.size() == 0) {
            mErrorManager.printInfo("Incorrect Syntax: There must be lists to be combined.");
            return;

        } else if(listsToCombine.size() == 1) {
            mErrorManager.printInfo("Incorrect Syntax: There must be at least two lists to be combined.");
            return;
        }
        // Call WordListManager to handle with listname, listsToCombine to combine
        // TODO

    } else if (listIdentifier.id == 251) {  // Difference the lists
        std::vector<std::string> listsToDifference = parseMultipleLists();

        // Error occured, syntax is wrong 
        if(listsToDifference.size() == 0) {
            mErrorManager.printInfo("Incorrect Syntax: There must be lists to find differences.");
            return;

        } else if(listsToDifference.size() == 1) {
            mErrorManager.printInfo("Incorrect Syntax: There must be at least two lists to find differences.");
            return;
        }
        // Call WordListManager to handle with listname, listsToDifference to difference
        // TODO

    } else if (listIdentifier.id == 250) {  // Intersection the lists
        std::vector<std::string> listsToIntersection = parseMultipleLists();

        // Error occured, syntax is wrong 
        if(listsToIntersection.size() == 0) {
            mErrorManager.printInfo("Incorrect Syntax: There must be lists to find intersections.");
            return;

        } else if(listsToIntersection.size() == 1) {
            mErrorManager.printInfo("Incorrect Syntax: There must be at least two lists to find intersections.");
            return;
        }
        // Call WordListManager to handle with listname, listsToIntersection to intersection
        // TODO

    } else if (listIdentifier.id == 249) {   // Copy the list
        auto listnameToCopy = mTokenManager.Use();

        if(listnameToCopy == mTokenManager.eof_token || listnameToCopy != emplex::Lexer::ID_LISTNAME) { 
            // Missing listname to copy
            mErrorManager.printInfo("Incorrect Syntax: Missing list name to copy.");
            return;
        }
        // Call WordListManager to handle with listname, listnameToCopy to copy
        // TODO

    } else {
        // Syntax Error 
        mErrorManager.printInfo("Incorrect Syntax: Use of invalid syntax.");
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
            // mErrorManager.printInfo("Incorrect Syntax: Incorrect identifier type, must be an exisiting listname.");
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

    } else if(identifier.id == 235) { // PRINT X number
        mWordListManager.print(std::stoi(identifier.lexeme));

    } else {
        mErrorManager.printInfo("Incorrect Syntax: Incorrect identifier type, must be a number or \"ALL\"");
        return;
    }
}

void cse::WordLang::parseSetCurrent() {
    mTokenManager.Use();

    std::vector<std::string> listsToSetCurrent = parseMultipleLists();

    // Error occured, syntax is wrong 
    if(listsToSetCurrent.size() == 0) {
        mErrorManager.printInfo("Incorrect Syntax: There must be list type to set as current.");
        return;
    }
    // Call WordListManager to handle with listsToSetCurrent
    // TODO
}

void cse::WordLang::parseAdd() {
    mTokenManager.Use();

    auto listname = mTokenManager.Use();

    // Check if a listname provided
    if(listname == mTokenManager.eof_token || listname != emplex::Lexer::ID_LISTNAME) {
        // Print syntax error 
        mErrorManager.printInfo("Incorrect Syntax: Missing listname identifier.");
        return;
    }

    auto words = mTokenManager.Use();
    if(words == mTokenManager.eof_token || words != emplex::Lexer::ID_STRING) { 
        // Missing file name to open
        mErrorManager.printInfo("Incorrect Syntax: Missing words to add.");
        return;
    }
    // Syntax is correct, call WordListManager to handle. WIth file name and list name to load
    std::string trimmedWords = words.lexeme.substr(1, words.lexeme.length() - 2);

    // Call WordListManager to handle with listname, trimmedWords to add into list.
    // TODO 
}

void cse::WordLang::parseSave() {
    mTokenManager.Use();

    auto listname = mTokenManager.Use();

    // Check if a listname provided
    if(listname == mTokenManager.eof_token || listname != emplex::Lexer::ID_LISTNAME) {
        // Print syntax error 
        mErrorManager.printInfo("Incorrect Syntax: Missing listname identifier.");
        return;
    }

    auto filename = mTokenManager.Use();
    if(filename == mTokenManager.eof_token || filename != emplex::Lexer::ID_STRING) { 
        // Missing file name to open
        mErrorManager.printInfo("Incorrect Syntax: Missing filename to add.");
        return;
    }
    // Syntax is correct, call WordListManager to handle.
    std::string trimmedFilename = filename.lexeme.substr(1, filename.lexeme.length() - 2);

    // Call WordListManager to handle with listname, trimmedFilename to save into.
    // TODO 
}

void cse::WordLang::parseLength() {
    mTokenManager.Use();

    // Check if = is used 
    if(mTokenManager.Use_if(232) == mTokenManager.eof_token ) {
        // Print syntax error 
        mErrorManager.printInfo("Incorrect Syntax: Missing assign(=) operator.");
        return;
    }

    auto length = mTokenManager.Use();

    if(length == mTokenManager.eof_token || (length != emplex::Lexer::ID_NUMBER && length != emplex::Lexer::ID_STAR) ) {
        // Print syntax error 
        mErrorManager.printInfo("Incorrect Syntax: Length value must be number or \"*\"");
        return;
    }

    // Call WordListManager to handle with length.
    // TODO 
}

void cse::WordLang::parseContainsAny() {
    mTokenManager.Use();

    auto letters = mTokenManager.Use();
    if( letters == mTokenManager.eof_token || letters != emplex::Lexer::ID_STRING ) {
        // Print syntax error 
        mErrorManager.printInfo("Incorrect Syntax: CONTAINS_ANY must have string of letters.");
        return;
    }

    std::string trimmedLetters = letters.lexeme.substr(1, letters.lexeme.length() - 2);
    // Call WordListManager to handle with trimmedLetters.
    // TODO 
}

void cse::WordLang::parseContainsAll() {
    mTokenManager.Use();
    
    auto letters = mTokenManager.Use();
    if (letters == mTokenManager.eof_token || letters != emplex::Lexer::ID_STRING) {
        // Print error and return
        mErrorManager.printInfo("Incorrect Syntax: CONTAINS_ALL must have string of letters.");
        return;
    }

    std::string trimmedLetters = letters.lexeme.substr(1, letters.lexeme.length() - 2);
    // TODO - call WordListManager to handle with trimmedLetters
    // Maybe we can combine this function with parseContainsAny() to avoid code duplication?
    // have just "parseContains" which accepts boolean 
}

void cse::WordLang::parseNotContains() {
    mTokenManager.Use();

    auto letters = mTokenManager.Use();
    if (letters == mTokenManager.eof_token || letters != emplex::Lexer::ID_STRING) {
        // Print error and return
        mErrorManager.printInfo("Incorrect Syntax: NOT_CONTAINS must have string of letters.");
        return;
    }

    std::string trimmedLetters = letters.lexeme.substr(1, letters.lexeme.length() - 2);

    // TODO - call WordListManager to handle trimmedLetters
}

void cse::WordLang::parseGet() {
    mTokenManager.Use(); // use keywords "GET"

    auto letters = mTokenManager.Use();
    if (letters == mTokenManager.eof_token || letters != emplex::Lexer::ID_STRING) {
        // Print error and return
        mErrorManager.printInfo("Incorrect Syntax: NOT_CONTAINS must have string of letters.");
        return;
    }

    std::string trimmedLetters = letters.lexeme.substr(1, letters.lexeme.length() - 2);

    // TODO - call WordListManager to handle trimmedLetters
}

void cse::WordLang::parseReset() {
    mTokenManager.Use(); // use keyword "RESET"

    auto listname = mTokenManager.Use();
    if (listname.id != emplex::Lexer::ID_LISTNAME) {
        // check if we have listname after
        mErrorManager.printInfo("Expected list name after RESET");
        return;
    }

    if (mTokenManager.Peek() != mTokenManager.eof_token()) {
        // check if we don't have anything else apart from RESET
        mErrorManager.printInfo("Encountered unknown symbols after \"RESET\" token.");
        return;
    }

    // TODO - call WordListManager to handle RESET
}

void cse::WordLang::parseResetLast() {
    mTokenManager.Use(); // use keyword "RESET_LAST"

    if (mTokenManager.Peek() != mTokenManager.eof_token()) {
        // check if we don't have anything else apart from RESET
        mErrorManager.printInfo("Encountered unknown symbols after \"RESET_LAST\" token.");
        return;
    }

    // TODO - call WordListManager to handle RESET_LAST
}

void cse::WordLang::parseWorldle() {
    mTokenManager.Use(); // use keyword "WORDLE"

    // TODO - parse and call WordListManager
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