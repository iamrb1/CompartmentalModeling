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

    void Parse(const std::string& input);

private:
/**
 * @brief Boolean that tracks if the app is currently running
 * 
 */
    bool mIsActive = false;
};


#endif  // CSE_WORDLANG_HPP_