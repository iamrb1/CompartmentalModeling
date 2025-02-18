/**
 * @file ArgManager.cpp
 * @author By Will Crawford
 * @brief Implementation of the ArgManager class
 */

#include <iostream>
#include <vector>
#include "ArgManager.h"

/**
 * @brief Constructor for ArgManager
 * @param argc the amount of arguments
 * @param argv the list of arguments
 */
cse::ArgManager::ArgManager(int argc, char *argv[])
{
    //Load in all the command line options
    LoadArgManager();

    //pass argc and argv
    mArgc = argc;
    for (int i = 0; i < argc; i++)
    {
        mArgv.push_back(argv[i]);
    }
}

/**
 * @brief Loads in all of the command line options
 */
void cse::ArgManager::LoadArgManager()
{
    
}

/**
 * @brief Checks if a string is present in the input arguments
 * @param argv the string to check for
 * @return if the string is present in the input arguments
 */
bool cse::ArgManager::Has(std::string argv)
{
    for (int i = 0 ; i < mArgc; i++)
    {
        if (mArgv[i] == argv)
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Find a string and return the next string as the value of the option
 * @param argv the string to find
 * @return the next string as the value of the option
 */
std::string cse::ArgManager::GetOption(std::string argv)
{
    for (int i = 0 ; i < mArgc; i++)
    {
        if (mArgv[i] == argv)
        {
            if (i + 1 >= mArgc)
            {
                std::cout << "Invalid output provided for: " << argv << std::endl;
                return "";
            }
            else
            {
                return mArgv[i + 1];
            }
        }
    }
    std::cout << "This arg does not exist: " << argv << std::endl;

    return "";
}

/**
 * @brief Find an option, then get the number after it to determine the size of the list, then return the next values in a list as the value of the option
 * @param argv the string to find
 * @return the next strings in a list as the value of the option
 */
std::vector<std::string> cse::ArgManager::GetOptions(std::string argv)
{
    int count = -1;
    int startingIndex = -1;
    std::vector<std::string> toReturn = {};
    for (int i = 0 ; i < mArgc; i++)
    {
        if (mArgv[i] == argv)
        {
            if (i + 1 >= mArgc)
            {
                std::cout << "Invalid output provided for: " << argv << std::endl;
                return {""};
            }
            else if (count == -1)
            {
                startingIndex = i + 1;
                try {
                    count = std::stoi(mArgv[i + 1]);
                }
                catch(const std::exception& caughtError) {
                    std::cerr << "\"" << mArgv[i + 1] << "\" is not an integer" << std::endl;
                    return {""};
                }
                if (startingIndex + count >= mArgc)
                {
                    std::cout << "Not enough options provided for: " << argv << std::endl;
                    return {""};
                }
            }
        }
        else if (i > startingIndex && i <= startingIndex + count)
        {
            toReturn.push_back(mArgv[i]);
        }
    }

    if (count == -1)
    {
        std::cout << "This arg does not exist: " << argv << std::endl;
        return {""};
    }
    return toReturn;
}

/**
* @brief Prints help info for ArgManager
*/
void cse::ArgManager::PrintHelp()
{
    std::cout << "--help has been triggered\n";
    std::cout << "arguments available:\n";
    std::cout << "-h | The help command" << std::endl;
    std::cout << "-o | Gets the string name after it" << std::endl;
    std::cout << "-l | The integer given after determines the list size, every string after it will be added to the list until it reaches capacity" << std::endl;
}