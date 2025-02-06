#ifndef CSE_STRING_HPP
#define CSE_STRING_HPP

#include <string>
#include <cstddef>

namespace cse {
class String : public std::string {
public:
    using std::string::string;
    String();
    String(const std::string &other);
    String &operator=(const std::string &other);
    char &operator[](std::size_t pos);
    const char &operator[](std::size_t pos) const;
};
}

#endif
