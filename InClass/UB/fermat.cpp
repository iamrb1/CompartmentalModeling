#include <iostream>

int found_a = -1;
int found_b = -1;
int found_c = -1;

bool SolveFermat3()
{
  const int max_value = 1000;

  for (int a = 1, b = 1, c = 1; true; )
  {
    if (a*a*a + b*b*b == c*c*c) {
      found_a = a;
      found_b = b;
      found_c = c;
      return true; //disproved!!
    }

    a++;
    if (a > max_value) {
      a = 1;
      b++;
    }
    if (b > max_value)
    {
      b = 1;
      c++;
    }
    if (c > max_value) {
      c = 1;
    }
  }

  return false; // No solutions found.
}

int main()
{
  std::cout << "Fermat's Last Theorem ";

  if (SolveFermat3()) {
    std::cout << " has been disproved!\n";    
  } else {
    std::cout << " had not been disproved.\n";
  }

  std::cout << "A = " << found_a
            << "; B = " << found_b
            << "; C = " << found_c
            << "\n";
}