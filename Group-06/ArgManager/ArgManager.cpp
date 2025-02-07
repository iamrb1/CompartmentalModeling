/**
 * Implementation of the ArgManager class
 * 
 * By Will Crawford
 */

#include <iostream>
#include <vector>
#include "ArgManager.h"

/**
 * Constructor for ArgManager
 * @param argc
 * @param argv 
 */
cse::ArgManager::ArgManager(int argc, char *argv[])
{
    //Load in all the command line options
    LoadArgManager();

    //pass argc and argv
    _argc = argc;
    for (int i = 0; i < argc; i++)
    {
        _argv.push_back(argv[i]);
    }
}

/**
 * Loads in all of the command line options
 */
void cse::ArgManager::LoadArgManager()
{

}

/**
 * Checks if a string is present in the input arguments
 * @param argv 
 * @return
 */
bool cse::ArgManager::Has(std::string argv)
{
    for (int i = 0 ; i < _argc; i++)
    {
        if (_argv[i] == argv)
        {
            return true;
        }
    }
}

/**
 * Find a string and return the next string as the value of the option
 * @param argv
 * @return
 */
std::string cse::ArgManager::GetOption(std::string argv)
{
    for (int i = 0 ; i < _argc; i++)
    {
        if (_argv[i] == argv)
        {
            return _argv[i + 1];
        }
    }
}