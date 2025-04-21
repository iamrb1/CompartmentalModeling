/**
 * @file WordLang.cpp
 * @author Mehmet Efe Caylan
 * @author Will Crawford
 * @author Ivan Bega
 * @author Orhan Aydin
 * @author Emil Rahn Siegel
 * @brief WordLang implementation file.
 * @version 0.1
 * @date 2025-03-29
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "WordLang.hpp"
#include "FileSource.hpp"
#include "lexer.hpp"
#include <istream>
#include <vector>

void cse::WordLang::start() {
    std::cout << "Welcome to WordLang! Type your query below:\n";

    // BEFORE TESTING TURN COLORS FALSE, OTHERWISE ALL TESTS WILL FAIL
    mErrorManager.enableColors(false);

    mIsActive = true;
    std::string input;

    while (mIsActive) {
        std::cout << ">>> ";
        std::getline(std::cin, input);

        if (input == "EXIT") {
            mIsActive = false;
            break;
        }
        
        parse(input);
    }
}

cse::WordLang::WordLang() : mErrorManager(), mWordListManager(mErrorManager) {

}

cse::WordLang::~WordLang() {
    
}

void cse::WordLang::parse(const std::string& input) {
    // Utility class that handles tokenization
    mTokenManager.Load(input);
    // If tests are failing, try uncommenting this as a TEMP fix
    // mErrorManager.enableColors(false);
    
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
        case Lexer::ID_WORDLE: {
            parseWordle();
            break;
        }
        default:{
            mErrorManager.printInfo("Incorrect Syntax: Check syntax, keyword is not found.");
            break;
        }
    }
}

void cse::WordLang::parseList() {
    mTokenManager.Use();

    // List name for the list created
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

    // List identifier such as LOAD, COMBINED, COPY, INTERSECTION, DIFFERENCE
    auto listIdentifier = mTokenManager.Use();

    if(listIdentifier.id == 253) {          // Load the list
        auto filename = mTokenManager.Use();

        if(filename == mTokenManager.eof_token || filename != emplex::Lexer::ID_STRING) { // Check file name
            // Missing file name to open
            mErrorManager.printInfo("Incorrect Syntax: Missing filename to open.");
            return;
        }

        if (mTokenManager.Peek() != mTokenManager.eof_token) {
            // check if we don't have anything else apart from LOAD
            mErrorManager.printInfo("Incorrect Syntax: Encountered unknown symbols after \"LOAD\" token.");
            return;
        }

        // Syntax is correct, call WordListManager to handle. With file name and list name to load
        std::string cut_name = filename.lexeme.substr(1,filename.lexeme.length() - 2);
        if (!mWordListManager.loadList(listname.lexeme, cut_name)) {
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

        if (mTokenManager.Peek() != mTokenManager.eof_token) {
            // check if we don't have anything else apart from COMBINE
            mErrorManager.printInfo("Incorrect Syntax: Encountered unknown symbols after \"COMBINE\" token.");
            return;
        }

        // Call WordListManager to handle with listname, listsToCombine to combine
        if (!mWordListManager.combine(listname.lexeme, listsToCombine)) {
            return;
        }

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

        if (mTokenManager.Peek() != mTokenManager.eof_token) {
            // check if we don't have anything else apart from DIFFERENCE
            mErrorManager.printInfo("Incorrect Syntax: Encountered unknown symbols after \"DIFFERENCE\" token.");
            return;
        }

        // Call WordListManager to handle with listname, listsToDifference to difference
        if (!mWordListManager.difference(listname.lexeme, listsToDifference)) {
            return;
        }

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

        if (mTokenManager.Peek() != mTokenManager.eof_token) {
            // check if we don't have anything else apart from INTERSECTION
            mErrorManager.printInfo("Incorrect Syntax: Encountered unknown symbols after \"INTERSECTION\" token.");
            return;
        }

        // Call WordListManager to handle with listname, listsToIntersection to intersection
        if (!mWordListManager.intersection(listname.lexeme, listsToIntersection)) {
            return;
        }

    } else if (listIdentifier.id == 249) {   // Copy the list
        auto listnameToCopy = mTokenManager.Use();

        if(listnameToCopy == mTokenManager.eof_token || listnameToCopy != emplex::Lexer::ID_LISTNAME) { 
            // Missing listname to copy
            mErrorManager.printInfo("Incorrect Syntax: Missing list name to copy.");
            return;
        }

        if (mTokenManager.Peek() != mTokenManager.eof_token) {
            // check if we don't have anything else apart from COPY
            mErrorManager.printInfo("Incorrect Syntax: Encountered unknown symbols after \"COPY\" token.");
            return;
        }

        // Call WordListManager to handle with listname, listnameToCopy to copy
        if (!mWordListManager.copy(listname.lexeme, listnameToCopy.lexeme)) {
            return;
        }

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
        if (!mWordListManager.print(0,true)) return;

    } else if(identifier.id == 235) { // PRINT X number
        if (!mWordListManager.print(std::stoi(identifier.lexeme))) return;

    } else {
        mErrorManager.printInfo("Incorrect Syntax: Incorrect identifier type, must be a number or \"ALL\"");
        return;
    }

    if (mTokenManager.Peek() != mTokenManager.eof_token) {
        // check if we don't have anything else apart from PRINT
        mErrorManager.printInfo("Incorrect Syntax: Encountered unknown symbols after \"PRINT\" token.");
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

    if (mTokenManager.Peek() != mTokenManager.eof_token) {
        // check if we don't have anything else apart from SET_CURRENT
        mErrorManager.printInfo("Incorrect Syntax: Encountered unknown symbols after \"SET_CURRENT\" token.");
        return;
    }

    // Call WordListManager to handle with listsToSetCurrent
    if (!mWordListManager.setCurrent(listsToSetCurrent)) {
        return;
    }
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

    if (mTokenManager.Peek() != mTokenManager.eof_token) {
        // check if we don't have anything else apart from ADD
        mErrorManager.printInfo("Incorrect Syntax: Encountered unknown symbols after \"ADD\" token.");
        return;
    }

    // Syntax is correct, call WordListManager to handle. WIth file name and list name to load
    std::string trimmedWords = words.lexeme.substr(1, words.lexeme.length() - 2);

    // Call WordListManager to handle with listname, trimmedWords to add into list.
    if (!mWordListManager.add(listname.lexeme, trimmedWords)) {
        return;
    }
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
        mErrorManager.printInfo("Incorrect Syntax: Missing filename to save.");
        return;
    }

    if (mTokenManager.Peek() != mTokenManager.eof_token) {
        // check if we don't have anything else apart from SAVE
        mErrorManager.printInfo("Incorrect Syntax: Encountered unknown symbols after \"SAVE\" token.");
        return;
    }

    // Syntax is correct, call WordListManager to handle.
    std::string trimmedFilename = filename.lexeme.substr(1, filename.lexeme.length() - 2);

    if(!mWordListManager.save(trimmedFilename, listname.lexeme)) {
        return;
    }
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

    if (mTokenManager.Peek() != mTokenManager.eof_token) {
        // check if we don't have anything else apart from LENGTH
        mErrorManager.printInfo("Incorrect Syntax: Encountered unknown symbols after \"LENGTH\" token.");
        return;
    }
    
    mWordListManager.setLengthRestriction(length.lexeme);

}

void cse::WordLang::parseContainsAny() {
    mTokenManager.Use();

    auto letters = mTokenManager.Use();

    if( letters == mTokenManager.eof_token || letters != emplex::Lexer::ID_STRING ) {
        // Print syntax error 
        mErrorManager.printInfo("Incorrect Syntax: CONTAINS_ANY must have string of letters.");
        return;
    }  

    if (mTokenManager.Peek() != mTokenManager.eof_token) {
        // check if we don't have anything else apart from CONTAINS_ANY
        mErrorManager.printInfo("Incorrect Syntax: Encountered unknown symbols after \"CONTAINS_ANY\" token.");
        return;
    }

    std::string trimmedLetters = letters.lexeme.substr(1, letters.lexeme.length() - 2);
    // Call WordListManager to handle with trimmedLetters.
    if(!mWordListManager.ContainsAny(trimmedLetters)) {
        return;
    }
    
}

void cse::WordLang::parseContainsAll() {
    mTokenManager.Use();
    
    auto letters = mTokenManager.Use();
    if (letters == mTokenManager.eof_token || letters != emplex::Lexer::ID_STRING) {
        // Print error and return
        mErrorManager.printInfo("Incorrect Syntax: CONTAINS_ALL must have string of letters.");
        return;
    }

    if (mTokenManager.Peek() != mTokenManager.eof_token) {
        // check if we don't have anything else apart from CONTAINS_ALL
        mErrorManager.printInfo("Incorrect Syntax: Encountered unknown symbols after \"CONTAINS_ALL\" token.");
        return;
    }

    std::string trimmedLetters = letters.lexeme.substr(1, letters.lexeme.length() - 2);
    if (!mWordListManager.ContainsAll(trimmedLetters)) {
        return;
    }
}

void cse::WordLang::parseNotContains() {
    mTokenManager.Use();

    auto letters = mTokenManager.Use();
    if (letters == mTokenManager.eof_token || letters != emplex::Lexer::ID_STRING) {
        // Print error and return
        mErrorManager.printInfo("Incorrect Syntax: NOT_CONTAINS must have string of letters.");
        return;
    }

    if (mTokenManager.Peek() != mTokenManager.eof_token) {
        // check if we don't have anything else apart from NOT_CONTAINS
        mErrorManager.printInfo("Incorrect Syntax: Encountered unknown symbols after \"NOT_CONTAINS\" token.");
        return;
    }

    std::string trimmedLetters = letters.lexeme.substr(1, letters.lexeme.length() - 2);
    if (!mWordListManager.NotContains(trimmedLetters)) {
        return;
    }
}

void cse::WordLang::parseGet() {
    mTokenManager.Use(); // use keywords "GET"

    auto letters = mTokenManager.Use();
    if (letters == mTokenManager.eof_token || letters != emplex::Lexer::ID_STRING) {
        // Print error and return
        mErrorManager.printInfo("Incorrect Syntax: GET must have string of letters or wildcards.");
        return;
    }

    if (mTokenManager.Peek() != mTokenManager.eof_token) {
        // check if we don't have anything else apart from GET
        mErrorManager.printInfo("Incorrect Syntax: Encountered unknown symbols after \"GET\" token.");
        return;
    }

    std::string trimmedLetters = letters.lexeme.substr(1, letters.lexeme.length() - 2);

    if (!mWordListManager.Get(trimmedLetters)) {
        return;
    }
}

void cse::WordLang::parseReset() {
    mTokenManager.Use(); // use keyword "RESET"

    auto listname = mTokenManager.Use();
    if (listname.id == mTokenManager.eof_token) {
        // if we don't have any listnames - reset all current listnames
        mWordListManager.reset();
        return;
    }

    if (listname == emplex::Lexer::ID_LISTNAME) {
        mWordListManager.reset(listname.lexeme);
        return;
    }

    mErrorManager.printInfo("Incorrect Syntax: encountered unknown symbols in RESET statement");
}

void cse::WordLang::parseWordle() {
    mTokenManager.Use(); // use keyword "WORDLE"
  
    if (mTokenManager.Use_if(230) == mTokenManager.eof_token) {  // Use '(' 
        mErrorManager.printInfo("Incorrect Syntax: Wordle command must have \'(\'.");
        return;
    }

    auto word = mTokenManager.Use();

    if(word == mTokenManager.eof_token || word != emplex::Lexer::ID_STRING) {
        mErrorManager.printInfo("Incorrect Syntax: Missing word inputted to the wordle.");
        return;
    }

    if (mTokenManager.Use_if(231) == mTokenManager.eof_token) {  // Use ',' 
        mErrorManager.printInfo("Incorrect Syntax: Missing \',\' between two strings.");
        return;
    }

    auto result = mTokenManager.Use();

    if(result == mTokenManager.eof_token || result != emplex::Lexer::ID_STRING) {
        mErrorManager.printInfo("Incorrect Syntax: Missing result string from wordle.");
        return;
    }

    if (mTokenManager.Use_if(229) == mTokenManager.eof_token) {  // Use ')' 
        mErrorManager.printInfo("Incorrect Syntax: Wordle command must have \')\' at the end.");
        return;
    }

    if (mTokenManager.Peek() != mTokenManager.eof_token) {
        // check if we don't have anything else apart from WORDLE
        mErrorManager.printInfo("Incorrect Syntax: Encountered unknown symbols after \"WORDLE\" token.");
        return;
    }

    std::string trimmedWord = word.lexeme.substr(1, word.lexeme.length() - 2);
    std::string trimmedResult = result.lexeme.substr(1, result.lexeme.length() - 2);
    if (!mWordListManager.wordle(trimmedWord, trimmedResult)) {
        return;
    }
}
