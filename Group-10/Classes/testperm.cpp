#include "PermutationManager.hpp"
#include <iostream>


int main() {
  // Example usage
  std::vector<int> items = {1, 2, 3, 4};
  cse::PermutationManager<std::vector<int>> permManager(items, 3);

  int total = permManager.permutationNumber(4, 3);
  int t = 0;

  // Print all permutations
  do {
      const auto& perm = permManager.getCurrentPermutation();
      for (auto item : perm) {
          std::cout << item << " ";
      }
      std::cout << std::endl;
  } while (permManager.next());

  std::cout << "going backwards now" << std::endl;
  // Print all permutations
  do {
    const auto& perm = permManager.getCurrentPermutation();
    for (auto item : perm) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
    t += 1;
} while (permManager.prev());

  std::cout << total << " " << t << std::endl; 
}
