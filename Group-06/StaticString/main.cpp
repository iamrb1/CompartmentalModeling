#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <cctype>

#include "StaticString.hpp"

//CITE: https://chatgpt.com/share/67e4bfa0-3c98-800e-a0da-3abc3715e1b1
int main() {
  
  const int iterations = 100000;  // number of iterations for each test
  using namespace std::chrono;

  // Test 1: Construction, Copy, Assignment, and Element Access
  {
    auto start_std = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
      std::string s("Hello, World!");
      std::string s_copy(s);
      std::string s_assigned;
      s_assigned = s;
      [[maybe_unused]] volatile char ch = s[0]; // prevent optimization
    }
    auto end_std = high_resolution_clock::now();
    auto duration_std = duration_cast<microseconds>(end_std - start_std).count();

    auto start_static = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
      cse::StaticString<128> s("Hello, World!");
      cse::StaticString<128> s_copy(s);
      cse::StaticString<128> s_assigned;
      s_assigned = s;
      [[maybe_unused]] volatile char ch = s[0];
    }
    auto end_static = high_resolution_clock::now();
    auto duration_static = duration_cast<microseconds>(end_static - start_static).count();

    std::cout << "Test 1: Construction, Copy, Assignment, Element Access\n";
    std::cout << "std::string:  " << duration_std << " microseconds\n";
    std::cout << "StaticString: " << duration_static << " microseconds\n";
    double ratio = static_cast<double>(duration_std) / duration_static;
    std::cout << std::setprecision(3) << "RATIO: " << ratio << "x times faster.\n";
  }

  // Test 2: Append Operations
  {
    auto start_std = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
      std::string s("Hello");
      s.append(", World");
      s.push_back('!');
    }
    auto end_std = high_resolution_clock::now();
    auto duration_std = duration_cast<microseconds>(end_std - start_std).count();

    auto start_static = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
      cse::StaticString<128> s("Hello");
      s.append(", World");
      s.append('!');
    }
    auto end_static = high_resolution_clock::now();
    auto duration_static = duration_cast<microseconds>(end_static - start_static).count();

    std::cout << "\nTest 2: Append Operations\n";
    std::cout << "std::string:  " << duration_std << " microseconds\n";
    std::cout << "StaticString: " << duration_static << " microseconds\n";
    double ratio = static_cast<double>(duration_std) / duration_static;
    std::cout << std::setprecision(3) << "RATIO: " << ratio << "x times faster.\n";
  }

  // Test 3: Find Operations
  {
    auto start_std = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
      std::string s("Hello, World!");
      [[maybe_unused]] volatile size_t pos1 = s.find('o');
      [[maybe_unused]] volatile size_t pos2 = s.find("World");
    }
    auto end_std = high_resolution_clock::now();
    auto duration_std = duration_cast<microseconds>(end_std - start_std).count();

    auto start_static = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
      cse::StaticString<128> s("Hello, World!");
      [[maybe_unused]] volatile size_t pos1 = s.find('o');
      [[maybe_unused]] volatile size_t pos2 = s.find("World");
    }
    auto end_static = high_resolution_clock::now();
    auto duration_static = duration_cast<microseconds>(end_static - start_static).count();

    std::cout << "\nTest 3: Find Operations\n";
    std::cout << "std::string:  " << duration_std << " microseconds\n";
    std::cout << "StaticString: " << duration_static << " microseconds\n";
    double ratio = static_cast<double>(duration_std) / duration_static;
    std::cout << std::setprecision(3) << "RATIO: " << ratio << "x times faster.\n";
  }

  // Test 4: Insert Operations
  {
    auto start_std = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
      std::string s("HelloWorld");
      s.insert(5, ", ");
    }
    auto end_std = high_resolution_clock::now();
    auto duration_std = duration_cast<microseconds>(end_std - start_std).count();

    auto start_static = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
      cse::StaticString<128> s("HelloWorld");
      s.insert(", ", 5);
    }
    auto end_static = high_resolution_clock::now();
    auto duration_static = duration_cast<microseconds>(end_static - start_static).count();

    std::cout << "\nTest 4: Insert Operations\n";
    std::cout << "std::string:  " << duration_std << " microseconds\n";
    std::cout << "StaticString: " << duration_static << " microseconds\n";
    double ratio = static_cast<double>(duration_std) / duration_static;
    std::cout << std::setprecision(3) << "RATIO: " << ratio << "x times faster.\n";
  }

  // Test 5: Remove/Erase Operations
  {
    auto start_std = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
      std::string s("Hello, World!");
      s.erase(5, 2);  // remove ", "
    }
    auto end_std = high_resolution_clock::now();
    auto duration_std = duration_cast<microseconds>(end_std - start_std).count();

    auto start_static = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
      cse::StaticString<128> s("Hello, World!");
      s.remove(5, 7);  // remove characters at indexes 5 to 6
    }
    auto end_static = high_resolution_clock::now();
    auto duration_static = duration_cast<microseconds>(end_static - start_static).count();

    std::cout << "\nTest 5: Remove/Erase Operations\n";
    std::cout << "std::string:  " << duration_std << " microseconds\n";
    std::cout << "StaticString: " << duration_static << " microseconds\n";
    double ratio = static_cast<double>(duration_std) / duration_static;
    std::cout << std::setprecision(3) << "RATIO: " << ratio << "x times faster.\n";
  }

  // Test 6: Replace Operations
  {
    auto start_std = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
      std::string s("Hello, World!");
      size_t pos = s.find("World");
      if (pos != std::string::npos) {
        s.replace(pos, 5, "Universe");
      }
    }
    auto end_std = high_resolution_clock::now();
    auto duration_std = duration_cast<microseconds>(end_std - start_std).count();

    auto start_static = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
      cse::StaticString<128> s("Hello, World!");
      s.replace("World", "Universe");
    }
    auto end_static = high_resolution_clock::now();
    auto duration_static = duration_cast<microseconds>(end_static - start_static).count();

    std::cout << "\nTest 6: Replace Operations\n";
    std::cout << "std::string:  " << duration_std << " microseconds\n";
    std::cout << "StaticString: " << duration_static << " microseconds\n";
    double ratio = static_cast<double>(duration_std) / duration_static;
    std::cout << std::setprecision(3) << "RATIO: " << ratio << "x times faster.\n";
  }

  // Test 7: Split Operations
  {
    // Custom split for std::string since there is no built-in one
    auto split_string = [](const std::string& s, char delimiter) -> std::vector<std::string> {
      std::vector<std::string> result;
      size_t start = 0, end = 0;
      while ((end = s.find(delimiter, start)) != std::string::npos) {
        result.push_back(s.substr(start, end - start));
        start = end + 1;
      }
      result.push_back(s.substr(start));
      return result;
    };

    auto start_std = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
      std::string s("one,two,three,four");
      auto parts = split_string(s, ',');
      [[maybe_unused]] volatile size_t dummy = parts.size();
    }
    auto end_std = high_resolution_clock::now();
    auto duration_std = duration_cast<microseconds>(end_std - start_std).count();

    auto start_static = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
      cse::StaticString<128> s("one,two,three,four");
      auto parts = s.split(',');
      [[maybe_unused]] volatile size_t dummy = parts.size();
    }
    auto end_static = high_resolution_clock::now();
    auto duration_static = duration_cast<microseconds>(end_static - start_static).count();

    std::cout << "\nTest 7: Split Operations\n";
    std::cout << "std::string (custom): " << duration_std << " microseconds\n";
    std::cout << "StaticString:         " << duration_static << " microseconds\n";
    double ratio = static_cast<double>(duration_std) / duration_static;
    std::cout << std::setprecision(3) << "RATIO: " << ratio << "x times faster.\n";
  }

  // Test 8: Transform Operations (convert to uppercase)
  {
    auto start_std = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
      std::string s("Hello, World!");
      std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    }
    auto end_std = high_resolution_clock::now();
    auto duration_std = duration_cast<microseconds>(end_std - start_std).count();

    auto start_static = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
      cse::StaticString<128> s("Hello, World!");
      [[maybe_unused]] auto t = s.transform([](char c) {
        return static_cast<char>(std::toupper(c));
      });
    }
    auto end_static = high_resolution_clock::now();
    auto duration_static = duration_cast<microseconds>(end_static - start_static).count();

    std::cout << "\nTest 8: Transform Operations (Uppercase)\n";
    std::cout << "std::string:  " << duration_std << " microseconds\n";
    std::cout << "StaticString: " << duration_static << " microseconds\n";
    double ratio = static_cast<double>(duration_std) / duration_static;
    std::cout << std::setprecision(3) << "RATIO: " << ratio << "x times faster.\n";
  }

  // Test 9: Substring Extraction (substr)
  {
    auto start_std = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
      std::string s("Hello, World!");
      volatile auto sub = s.substr(0, 5);
    }
    auto end_std = high_resolution_clock::now();
    auto duration_std = duration_cast<microseconds>(end_std - start_std).count();

    auto start_static = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
      cse::StaticString<128> s("Hello, World!");
      [[maybe_unused]] volatile auto sub = s.substr(0, 5);
    }
    auto end_static = high_resolution_clock::now();
    auto duration_static = duration_cast<microseconds>(end_static - start_static).count();

    std::cout << "\nTest 9: substr()\n";
    std::cout << "std::string:  " << duration_std << " microseconds\n";
    std::cout << "StaticString: " << duration_static << " microseconds\n";
    double ratio = static_cast<double>(duration_std) / duration_static;
    std::cout << std::setprecision(3) << "RATIO: " << ratio << "x times faster.\n";
  }

  // Test 10: Trimming Whitespace
  {
    auto trim_std = [](std::string str) {
      // Trim trailing whitespace
      str.erase(str.find_last_not_of(" \t\r") + 1);
      // Trim leading whitespace
      str.erase(0, str.find_first_not_of(" \t\r"));
      return str;
    };

    auto start_std = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
      std::string s("   Hello, World!   ");
      [[maybe_unused]] auto t = trim_std(s);
    }
    auto end_std = high_resolution_clock::now();
    auto duration_std = duration_cast<microseconds>(end_std - start_std).count();

    auto start_static = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
      cse::StaticString<128> s("   Hello, World!   ");
      s.trim();
    }
    auto end_static = high_resolution_clock::now();
    auto duration_static = duration_cast<microseconds>(end_static - start_static).count();

    std::cout << "\nTest 10: trim()\n";
    std::cout << "std::string (custom): " << duration_std << " microseconds\n";
    std::cout << "StaticString:         " << duration_static << " microseconds\n";
    double ratio = static_cast<double>(duration_std) / duration_static;
    std::cout << std::setprecision(3) << "RATIO: " << ratio << "x times faster.\n";
  }

  return 0;
}