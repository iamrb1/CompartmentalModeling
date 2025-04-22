/**
 * @file main.cpp
 * @brief Entry point for the CSV Command Line Manipulator.
 *
 * Developed by: Max Krawec, Calen Green, Pedro Mitkiewicz, Shahaab Ali,
 * and Muhammad Asif Masood
 */

#include "FinalApplication.h"
#include <iostream>

/**
 * @brief main (Program Entry Point)
 */
int main() {
  FinalApplication final_application;
  final_application.MainMenu(std::cout, std::cin);
  return 0;
}