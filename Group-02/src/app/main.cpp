/**
 * @file main.cpp
 * @author Nitish Maindoliya
 */
#include <cse/StateGrid.h>
#include <cse/ActionMap.h>
#include <iostream>
#include <cse/DataMap.h>

int main() {
  std::cout << "Hello, World!" << std::endl;

  cse::ActionMap action_map;
  action_map.insert("sayHello", []() { std::cout << "Hello, Lambda!" << std::endl; });
  action_map.trigger("sayHello");

//  cse::DataMap data_map;
//  data_map.set("rahul", 20);
//  data_map.set("joe", std::string("p2eo"));
//  int bob = data_map.get<int>("rahul");
//  std::cout << "value is" << " " << bob << std::endl;
//  auto joe = data_map.get<int>("joe");
//  std::cout << "string is" << " " << joe << std::endl;
//
//  data_map.delKey("joe");
//  std::cout << data_map.keyContain("joe") << std::endl;

  return 0;
}
