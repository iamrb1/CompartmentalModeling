#include <iostream>

char MyFun(int x) {
  if (x > 50) return 'A';
  if (x > 1) return 'B';
}

int main() {
  std::cout << MyFun(100) << std::endl;
  std::cout << MyFun(10) << std::endl;
  std::cout << MyFun(1) << std::endl;
}