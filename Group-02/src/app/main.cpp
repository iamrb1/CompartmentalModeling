/**
 * @file main.cpp
 * @author Nitish Maindoliya
 */
#include <cse/StateGrid.h>
#include <cse/ActionMap.h>
#include <cse/DataMap.h>
#include <iostream>
#include <cse/AuditedVector.h>

int main() {
  std::cout << "Hello, World!" << std::endl;

  // cse::ActionMap action_map;
  // action_map.insert("sayHello", []() { std::cout << "Hello, Lambda!" << std::endl; });
  // action_map.trigger("sayHello");
  //
//   cse::DataMap data_map;
//   data_map.insert("rahul", 20);
//   data_map.insert("joe", std::string("p2eo"));
//   int bob = data_map.at<int>("rahul");
//   std::cout << "value is" << " " << bob << std::endl;
//   auto joe = data_map.at<std::string>("joe");
//   std::cout << "string is" << " " << joe << std::endl;
//
//   data_map.key_delete("joe");
//   std::cout << data_map.contains("joe") << std::endl;
//
//   data_map["rahul"] = 30;
//   bob = data_map.at<int>("rahul");
//   std::cout << "value is" << " " << bob << std::endl;
//
//   data_map["xor"] = std::string{"bob"};
//   std::cout << data_map.at<std::string>("xor") << std::endl;

  // cse::AuditedVector<int> vector;
  // vector.push_back(1);
  // vector.push_back(2);
  // vector.push_back(3);
  // std::cout << vector.size() << '\n';
  // vector.pop_back();
  // vector.pop_back();
  // vector.pop_back();
  // std::cout << vector.size();
  // vector.pop_back();
  // std::cout << vector[1];
  // return 0;
}
