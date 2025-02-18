/**
 * @file ArgManager.cpp
 * @author By Will Crawford
 * @brief The Arg Manger Class
 */

#ifndef ARGMANAGER__ARGMANAGER_H
#define ARGMANAGER__ARGMANAGER_H

#include <iostream>
#include <vector>
#include <functional>

namespace cse {

    /**
     * @brief the ArgManager class
     */
    class ArgManager {
    public:
        ArgManager(int argc, char *argv[]);
        void LoadArgManager();
        bool Has(std::string argv);
        std::string GetOption(std::string argv);
        std::vector<std::string> GetOptions(std::string argv);
        void PrintHelp();
        
    private:
        //store args

        /// @brief number of arguments
        int mArgc = 0;

        /// @brief List of options that have been given
        std::vector<std::string> mArgv;
    };
}

#endif //ARGMANAGER__ARGMANAGER_H

