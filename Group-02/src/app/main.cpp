/**
* @file main.cpp
 * @author Nitish Maindoliya
 */

#include <cse/ActionMap.h>
#include <iostream>

int main() {
  std::cout << "Hello, World!" << std::endl;

  cse::ActionMap action_map;
  action_map.insert("sayHello", []() { std::cout << "Hello, Lambda!" << std::endl; });
  action_map.trigger("sayHello");

  return 0;
}
