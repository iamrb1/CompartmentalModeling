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