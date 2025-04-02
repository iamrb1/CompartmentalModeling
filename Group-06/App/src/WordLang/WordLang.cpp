#include "WordLang.hpp"

void WordLang::start() {
    std::cout << "Welcome to WordLang! Type your query below:\n";

    mIsActive = true;
    std::string input;
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