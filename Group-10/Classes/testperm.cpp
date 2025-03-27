#include "PermutationManager.hpp"
#include <iostream>


void regularTest() {
  // Example usage
  std::vector<int> items = {1, 2, 3, 4, 5, 6};
  cse::PermutationManager<std::vector<int>> permManager(items, 3, false);

  int total = permManager.PermutationNumber(6, 3);
  //permManager.SetRepeating(false);
  int t = 0;

  // Print all permutations
  do {
      const auto& perm = permManager.GetCurrentPermutation();
      for (auto item : perm) {
          std::cout << item << " ";
      }
      std::cout << std::endl;
  } while (permManager.Next());

  std::cout << "going backwards now" << std::endl;
  // Print all permutations
  do {
    const auto& perm = permManager.GetCurrentPermutation();
    for (auto item : perm) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
    t += 1;
} while (permManager.Prev());

  std::cout << total << " " << t << std::endl; 
}


void requiredTest() {
  // Example usage
  std::vector<int> numbers = {1, 2, 3, 4, 5};
  cse::PermutationManager<std::vector<int>> permManager(numbers, 2, false, 1);
  permManager.SetRepeating(true);
  int t = 0;

  // Print all permutations
  do {
      const auto& perm = permManager.GetCurrentPermutation();
      for (auto item : perm) {
          std::cout << item << " ";
      }
      std::cout << std::endl;
      ++t;
  } while (permManager.Next());

  // now backwards
  std::cout << "\n" << std::endl;

  // Print all permutations
  do {
      const auto& perm = permManager.GetCurrentPermutation();
      for (auto item : perm) {
          std::cout << item << " ";
      }
      std::cout << std::endl;
      --t;
  } while (permManager.Prev());
  std::cout << t << std::endl;
}

void repeatTest() {
  // Example usage
  std::vector<int> items = {1, 2, 3, 4};
  cse::PermutationManager<std::vector<int>> permManager(items, 3, 1);
  permManager.SetRepeating(true);

  int total = permManager.PermutationNumber(4, 3);
  int t = 0;

  // Print all permutations
  do {
      const auto& perm = permManager.GetCurrentPermutation();
      for (auto item : perm) {
        std::cout << item << " ";
      }
      std::cout << std::endl;
  } while (permManager.Next());

  // now backwards
  std::cout << "\n" << std::endl;

  // Print all permutations
  do {
      const auto& perm = permManager.GetCurrentPermutation();
      for (auto item : perm) {
          std::cout << item << " ";
      }
      std::cout << std::endl;
  } while (permManager.Prev());
}


int main() {
  requiredTest();
}