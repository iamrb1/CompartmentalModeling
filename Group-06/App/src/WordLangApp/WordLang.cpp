#include "WordLang.hpp"
#include "../../../StringSet/StringSet.hpp"
#include "../../../StaticString/StaticString.hpp"
#include "FileSource.hpp"

void WordLang::start() {
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
    std::cout << "Words loaded: " << set.size() << "\n";
    while (mIsActive) {
        std::cout << ">>> ";
        std::getline(std::cin, input);

        std::cout << "You wrote: " << input << std::endl;
    }
}

WordLang::WordLang() {

}

WordLang::~WordLang() {
    
}

void WordLang::Parse(const std::string& input) {
    // In a switch tokenize the inputs to see which commands are called

    // Call command specific parse function
    /* Each parse function should check syntax, required variables, if the given 
       List name exists or file can open etc.

       If no error, calls WordListManager to do the operations and couts if necessary 
       no return required in this case since ther might be type issues.

       
    */
}