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
 #include "../../../ErrorManager/ErrorManager.hpp"


namespace cse {
/**
  * @brief Main clas for WordLang application
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
         */
        void start();
    
        void parse(const std::string& input);
        
        void parseList();
        std::vector<std::string> parseMultipleLists();
   
        void parsePrint();

        void parseSetCurrent();
        
        void parseAdd();

        void parseSave();

        void parseLength();

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
         * Resets all filters applied to the list
         */
        void parseReset();

        /**
         * @brief Parses RESET_LAST
         * 
         */
        void parseResetLast();

        /**
         * @brief Parses WORDLE
         * 
         */
        void parseWorldle();
    
    private:
    /**
     * @brief Boolean that tracks if the app is currently running
     * 
     */
        bool mIsActive = false;
    
        TokenManager mTokenManager;
   
        ErrorManager mErrorManager;
   
        WordListManager mWordListManager;
    };

}
 
 
 #endif  // CSE_WORDLANG_HPP_