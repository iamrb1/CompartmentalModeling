#include <iostream>
#include <string>
#include <sstream>

template <typename T>
concept uses_to_string = requires(T & value) { std::to_string(value); };

template <typename T>
concept has_ToString = requires(T & value) { value.ToString(); };


template <typename T>
std::string to_string(T value) {
  if constexpr (std::convertible_to<T,std::string>) {
    std::cout << "A:";
    return value;
  }
  else if constexpr (uses_to_string<T>) {
    std::cout << "B:";
    return std::to_string(value);
  }
  else if constexpr (has_ToString<T>) {
    std::cout << "C:";
    return value.ToString();
  }
  else {
    std::cout << "D:";
    std::stringstream ss;
    ss << value;    
    return ss.str();
  }
}

template <std::convertible_to<std::string> T>
int OnlyConvertibleToString(T value) {
  return 7;
}

struct MyStruct {
  int x = 5;
  double y = 12.34;
  std::string z = "MINE!";

  std::string ToString() { return "THIS IS THE REAL OUTPUT FOR MyStruct"; }
};

std::ostream & operator<<(std::ostream & os, const MyStruct & my_s) {
  os << "MY_STRUCT";
  return os;
}

int main()
{
  std::string str = "HELLO";
  MyStruct my_struct;
  
  std::cout << "12 -> " << to_string(12) << std::endl;
  std::cout << "\"abc\" -> " << to_string("abc") << std::endl;
  std::cout << "str -> " << to_string(str) << std::endl;
  std::cout << "my_struct -> " << to_string(my_struct) << std::endl;

  OnlyConvertibleToString(str);
  // OnlyConvertibleToString(12);
}