/**
 * @file WordLang.hpp
 * @author Mehmet Efe Caylan
 * @author Will Crawford
 * @author Ivan Bega
 * @author Orhan Aydin
 * @author Emil Rahn Siegel
 * @brief WordLang implementation file.
 * @version 0.1
 * @date 2025-03-29
 *
 */

 #ifndef CSE_WORDLANG_HPP_
 #define CSE_WORDLANG_HPP_
 
 #include <iostream>
 #include <string>
 #include "TokenManager.hpp"
 #include "WordListManager.hpp"

namespace cse {

/**
  * @brief Main class for WordLang application
  * 
  * WordLang is a simple Language that enables user to input using terminal
  * and output values.
  * 
  */
 class WordLang {
    public:
        /**
         * @brief Construct a new Word Lang object
         * 
         */
        WordLang();

        /**
         * @brief Destroy the Word Lang object
         * 
         */
        ~WordLang();
    
        /**
         * @brief Starts the app
         * 
         * Continuosly takes input from the user until EXIT keyword is entered
         * or keyboard interrupted. 
         */
        void start();

        /**
         * @brief Parse the user input
         * 
         * Parses the user input calls functions to check syntax and ensure
         * syntax is correct. Validated requests are directed to call 
         * WordListManager class to conduct operation requested.
         * 
         * @param input Terminal input of the user
         */
        void parse(const std::string& input);
        
        /**
         * @brief Parses LIST command 
         * 
         * Checks syntax and returns error messages to the terminal if syntax 
         * is wrong.
         * 
         * Creates and loads list to make operations. Have multiple keywords 
         * to narrow or broaden our lists.
         * 
         * If syntax is correct, class WordListManager to conduct operations
         * over the lists.
         * 
         * Valid Commands:
         * 
         * LIST listname = LOAD "filename"
         * LIST listname = COMBINED listname ... listname
         * LIST listname = DIFFERENCE listname ... listname
         * LIST listname = INTERSECTION listname ... listname
         * LIST listname = COPY listname
         *
         */
        void parseList();

        /**
         * @brief Parses input for multiple list name inputted 
         * 
         * @return std::vector<std::string> Vector of multiple lists requested
         */
        std::vector<std::string> parseMultipleLists();

        /**
         * @brief Parses PRINT command 
         * 
         * Checks syntax and returns error messages to the terminal if syntax 
         * is wrong.
         * 
         * Prints the outputs to the terminal
         * 
         * If syntax is correct, class WordListManager to conduct operations
         * over the lists.
         * 
         * Valid Command:
         *
         * PRINT number   print only given number of values
         * PRINT ALL  to print all values for request 
         * 
         */
        void parsePrint();

        /**
         * @brief Parses SET_CURRENT command 
         * 
         * Checks syntax and returns error messages to the terminal if syntax 
         * is wrong.
         * 
         * Sets the current searches to base on given lists
         * 
         * If syntax is correct, class WordListManager to conduct operations
         * over the lists.
         * 
         * Valid Command:
         * 
         * SET_CURRENT listname ... listname
         * 
         */
        void parseSetCurrent();
        
        /**
         * @brief Parses ADD command 
         * 
         * Checks syntax and returns error messages to the terminal if syntax 
         * is wrong.
         * 
         * If syntax is correct, class WordListManager to conduct operations
         * over the lists.
         * 
         * Valid Command:
         * 
         * ADD listname "space separated words"
         * 
         */
        void parseAdd();

        /**
         * @brief Parses SAVE command 
         * 
         * Checks syntax and returns error messages to the terminal if syntax 
         * is wrong.
         * 
         * Saves the given listname's values to a given filename, creates it 
         * if not exists, if exists writes over it.
         * 
         * If syntax is correct, class WordListManager to conduct operations
         * over the lists.
         * 
         * Valid Command:
         * 
         * SAVE listname "filename"
         * 
         */
        void parseSave();

        /**
         * @brief Parses LENGTH command 
         * 
         * Checks syntax and returns error messages to the terminal if syntax 
         * is wrong.
         * 
         * Restricts the word length for searches
         * 
         * If syntax is correct, class WordListManager to conduct operations
         * over the lists.
         * 
         * Valid Command:
         * 
         * LENGTH = number
         * LENGTH = *  to discard the lenght restriction
         * 
         */
        void parseLength();

        /**
         * @brief Parses CONTAINS_ANY command 
         * 
         * Checks syntax and returns error messages to the terminal if syntax 
         * is wrong.
         * 
         * Restricts the search to only letters given if exists in the word of 
         * lists
         * 
         * If syntax is correct, class WordListManager to conduct operations
         * over the lists.
         * 
         * Valid Command:
         * 
         * CONTAINS_ANY "collection of letters"
         * 
         */
        void parseContainsAny();

        /**
         * @brief Parses CONTAINS_ALL
         * 
         */
        void parseContainsAll();

        /**
         * @brief Parses NOT_CONTAINS
         * 
         */
        void parseNotContains();

        /**
         * @brief Parses GET
         *  This function returns words that match a pattern
         * 
         */
        void parseGet();

        /**
         * @brief Parses RESET
         * 
         * Parses RESET
         * 
         * Resets a list by reading it from the text file
         * 
         * Valid commands:
         * 
         * RESET - resets all current lists
         * 
         * RESET "listname" - resets specified listname
         * 
         */
        void parseReset();

        /**
         * @brief Parses WORDLE
         * 
         */
        void parseWordle();
    
    private:
        /// @brief Bool to check if program is still active
        bool mIsActive = false;

        /// @brief TokenManager that handles token creation
        TokenManager mTokenManager;

        /// @brief ErrorManager that handles error prompting
        ErrorManager mErrorManager;

        /// @brief WordListManager handles all the backend operations
        WordListManager mWordListManager;
    };

}
 
 
 #endif  // CSE_WORDLANG_HPP_