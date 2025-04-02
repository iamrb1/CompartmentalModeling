#pragma once
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

private:
/**
 * @brief Boolean that tracks if the app is currently running
 * 
 */
    bool mIsActive = false;
};