#include "cse_string.hpp"
#include <stdexcept>

namespace cse {
String::String() : std::string() {}

String::String(const std::string &other) : std::string(other) {}

String &String::operator=(const std::string &other) {
    std::string::operator=(other);
    return *this;
}

char &String::operator[](std::size_t pos) {
#ifndef NDEBUG
    if (pos >= size()) {
        throw std::out_of_range("cse::String index out of range");
    }
#endif
    return std::string::operator[](pos);
}

const char &String::operator[](std::size_t pos) const {
#ifndef NDEBUG
    if (pos >= size()) {
        throw std::out_of_range("cse::String index out of range");
    }
#endif
    return std::string::operator[](pos);
}
}
