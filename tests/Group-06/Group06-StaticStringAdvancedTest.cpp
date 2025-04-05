//==============================================================================
// BEYOND THIS POINT TESTS ARE FOR ADVANCED MEMEBR FUNCTIONS
//==============================================================================
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>
#include <ostream>

#include "../../Group-06/StaticString/StaticString.hpp"
#include "../../third-party/Catch/single_include/catch2/catch.hpp"

using namespace cse;

TEST_CASE("Tests for advanced memeber functions", "[StaticString]") {

  SECTION("TESTS: Find_all member function") {
    StaticString<20> s("HelloWorldHelloWorld");
    StaticString<20> s2("abcdabcdabcdabcd");
    StaticString<20> s3("1234567890123545");

    REQUIRE(s.findAll("Hello") == std::vector<size_t>{0,10});
    REQUIRE(s2.findAll("ab") == std::vector<size_t>{0,4,8,12});
    REQUIRE(s3.findAll("34") ==std::vector<size_t>{2});
    
    REQUIRE(s.findAll('e') == std::vector<size_t>{1,11});
    REQUIRE(s.findAll('E') == std::vector<size_t>{});
    REQUIRE(s.findAll('d') == std::vector<size_t>{9,19});

    REQUIRE(s2.findAll("abcD") == std::vector<size_t>{});
    REQUIRE(s2.findAll("abcd") == std::vector<size_t>{0,4,8,12});

    REQUIRE(s3.findAll("0") == std::vector<size_t>{9});

  }

  SECTION("TESTS: rFind member function") {
    StaticString<20> s("abgcdefbedagcd");
    REQUIRE(s.rFind('g') == 11);
    REQUIRE(s.rFind('d') == 13);
    REQUIRE(s.rFind('a') == 10);
    REQUIRE(s.rFind('f') == 6);
    REQUIRE(s.rFind('b') == 7);
    REQUIRE(s.rFind("gcd") == 11);
    REQUIRE(s.rFind('z') == StaticString<10>::npos);
    REQUIRE(s.rFind('h') == s.npos);
  }

  SECTION("TESTS: rFindAll  member function") {
    StaticString<20> s("HelloWorldHelloWorld");
    StaticString<20> s2("abcdabcdabcdabcd");
    StaticString<20> s3("1234567890123545");

    REQUIRE(s.rFindAll("Hello") == std::vector<size_t>{10,0});
    REQUIRE(s2.rFindAll("ab") == std::vector<size_t>{12,8,4,0});
    REQUIRE(s3.rFindAll("34") == std::vector<size_t>{2});
    
    REQUIRE(s.rFindAll('e') == std::vector<size_t>{11,1});
    REQUIRE(s.rFindAll('E') == std::vector<size_t>{});//cse::StaticString::npos);
    REQUIRE(s.rFindAll('d') == std::vector<size_t>{19,9});

    REQUIRE(s2.rFindAll("abcD") == std::vector<size_t>{});//cse::StaticString::npos);
    REQUIRE(s2.rFindAll("abcd") == std::vector<size_t>{12,8,4,0});

    REQUIRE(s3.rFindAll("0") == std::vector<size_t>{9});
  }

  SECTION("TESTS: Replace  member function") {
    StaticString<20> s("HelloWorldHelloWorld");
    StaticString<20> s2("abcdabcdabcdabcd");
    StaticString<20> s3("1234567890123545");
    StaticString<20> s4("12345");
    StaticString<40> s5("abcdabcdabcdabcd");

    s.replace("Hello", "olleH");
    REQUIRE(std::strcmp(s.get_str(), "olleHWorldolleHWorld") == 0);

    s2.replace('d', '!');
    REQUIRE(std::strcmp(s2.get_str(), "abc!abc!abc!abc!") == 0);

    s2.replace('a', '*');
    REQUIRE(std::strcmp(s2.get_str(), "*bc!*bc!*bc!*bc!") == 0);

    // Replace by greater or smaller size
    // Replace checks if replace occurs size would be exceeded or not
    s3.replace("1234", "00");
    REQUIRE(std::strcmp(s3.get_str(), "00567890123545") == 0);

    s3.replace("00", "1234");
    REQUIRE(std::strcmp(s3.get_str(), "1234567890123545") == 0);

    s4.replace('1', "HI");
    REQUIRE(std::strcmp(s4.get_str(), "HI2345") == 0);

    s4.replace('2', "my");
    REQUIRE(std::strcmp(s4.get_str(), "HImy345") == 0);

    s4.replace('3', "Name");
    REQUIRE(std::strcmp(s4.get_str(), "HImyName45") == 0);

    s4.replace('4', "IS");
    REQUIRE(std::strcmp(s4.get_str(), "HImyNameIS5") == 0);

    s4.replace('5', "**********");
    REQUIRE(std::strcmp(s4.get_str(), "HImyNameIS**********") == 0);

    s5.replace("abcd", "123456");
    REQUIRE(std::strcmp(s5.get_str(), "123456123456123456123456") == 0);

    s5.replace('1', "aa");
    REQUIRE(std::strcmp(s5.get_str(), "aa23456aa23456aa23456aa23456") == 0);

    s5.replace("23456", 'b');
    REQUIRE(std::strcmp(s5.get_str(), "aabaabaabaab") == 0);

    s4.replace("**********",'5');
    REQUIRE(std::strcmp(s4.get_str(), "HImyNameIS5") == 0);
    //Check if replace with larger value than size it can take would occur or not?
    CHECK_THROWS(s.replace("o","******"));
  }

  SECTION("TESTS: Replace_if  member function") {
    // Takes a lambda as the condition
    StaticString<20> s("HelloWorldHelloWorld");

    s.replace_if('H','I', [s](size_t index) -> bool {return s[index] == 'H';});
    //std::cout << s.to_string();
    REQUIRE(std::strcmp(s.get_str(), "IelloWorldIelloWorld") == 0);

    s.replace_if('I','H', [s](size_t index) -> bool {return s[index] == 'H';});
    REQUIRE(std::strcmp(s.get_str(), "IelloWorldIelloWorld") == 0);

    StaticString<25> s7("HelloWorldHelloWorld");

    //Replace all o's with a W after it
    s7.replace_if('o','e', [s7](size_t index) -> bool {return index < s7.size() && s7[index + 1] == 'W';});
    //std::cout << s7;
    REQUIRE(std::strcmp(s7.get_str(), "HelleWorldHelleWorld") == 0);

    //Replace all e's with a W after it
    s7.replace_if('e','o', [s7](size_t index) -> bool {return index < s7.size() && s7[index + 1] == 'W';});
    REQUIRE(std::strcmp(s7.get_str(), "HelloWorldHelloWorld") == 0);


    //test replacing a string with a functional lambda
    StaticString<25> s4("HelloWorldHelloWorld");

    //Replace the H at index 0 with an e
    s4.replace_if('H','e', [s4](size_t index) -> bool { return index == 0; });
    REQUIRE(std::strcmp(s4.get_str(), "eelloWorldHelloWorld") == 0);

    //Replace all e's after index 0 with an H
    s4.replace_if('e','H', [s4](size_t index) -> bool { return index > 0; });
    REQUIRE(std::strcmp(s4.get_str(), "eHlloWorldHHlloWorld") == 0);

    StaticString<25> s9("HelloWorHelloNor");
    s9.replace_if('o','R', [s9](size_t index) -> bool { return index < s9.size() && s9[index + 1] == 'N'; });
    //std::cout << s9.get_str();
    REQUIRE(std::strcmp(s9.get_str(), "HelloWorHellRNor") == 0);

    s9.replace_if('o','R', [s9](size_t index) -> bool { return index < s9.size() && s9[index + 1] == 'W'; });
    REQUIRE(std::strcmp(s9.get_str(), "HellRWorHellRNor") == 0);

    StaticString<25> s10("HelloWorHelloWor");
    s10.replace_if('o','W', [s10](size_t index) -> bool { return index > 0 && s10[index - 1] == 'l'; });
    //std::cout << s10.get_str();
    REQUIRE(std::strcmp(s10.get_str(), "HellWWorHellWWor") == 0);

    s10.replace_if('W','o', [s10](size_t index) -> bool { return index > 0 && s10[index - 1] == 'l'; });
    REQUIRE(std::strcmp(s10.get_str(), "HelloWorHelloWor") == 0);

    StaticString<25> s12("HelloWorldHelloWorld");
    s12.replace_if('l','L', [s12](size_t index) -> bool { return index < s12.size() && s12[index + 1] == 'l'; });
    //std::cout << s12.get_str();
    REQUIRE(std::strcmp(s12.get_str(), "HeLloWorldHeLloWorld") == 0);

    s12.replace_if('L','l', [s12](size_t index) -> bool { return index < s12.size() && s12[index + 1] == 'l'; });
    REQUIRE(std::strcmp(s12.get_str(), "HelloWorldHelloWorld") == 0);
  }

  SECTION("TESTS: Operators member function") {
    //operator>> for file streams 
    std::istringstream iss("TestInput");
    StaticString<20> s;
    
    REQUIRE(std::strcmp(s.get_str(), "") == 0);
    REQUIRE(s.length() == 0);
    iss >> s;
    REQUIRE(std::strcmp(s.get_str(), "TestInput") == 0);
    REQUIRE(s.length() == 9);
    
    std::istringstream iss2("Hello World");
    StaticString<20> s2;

    iss2 >> s2;
    // Due to whitespace only first input read check that
    REQUIRE(std::strcmp(s2.get_str(), "Hello") == 0);
    REQUIRE(s2.length() == 5);

    //operator<< for streams
    StaticString<20> s3("HelloWorld");
    std::ostringstream oss;
    
    oss << s3;
    REQUIRE(oss.str() == "HelloWorld");
  }

  SECTION("TESTS: Iterators") {
    StaticString<10> s("abcdef");

    auto begin = s.begin();
    auto end = s.end();

    std::string result;
    int counter = 0;
    for (auto it = begin; it != end; ++it) {
        result.push_back(*it);
        counter++;
    }
    //std::cout << std::to_string(counter);
    REQUIRE(result == "abcdef");

    std::string temp_result;
    int temp_counter = 0;
    for(auto item : s) {
      temp_result.push_back(item);
      temp_counter++;
    }

    REQUIRE(temp_result == "abcdef");
  }

  SECTION("TESTS: Insert member function") {
    StaticString<30> s("HelloWorld");
    StaticString<20> s2("abcd");
    StaticString<20> s3("1234567890");

    s.insert("Hello", 0);
    REQUIRE(std::strcmp(s.get_str(), "HelloHelloWorld") == 0);

    s.insert("World", 10);
    REQUIRE(std::strcmp(s.get_str(), "HelloHelloWorldWorld") == 0);

    s.insert(" ", 5);
    REQUIRE(std::strcmp(s.get_str(), "Hello HelloWorldWorld") == 0);

    s2.insert("1234", 1);
    REQUIRE(std::strcmp(s2.get_str(), "a1234bcd") == 0);

    s2.insert("!!!", 8);
    REQUIRE(std::strcmp(s2.get_str(), "a1234bcd!!!") == 0);

    s2.insert('*', 3);
    REQUIRE(std::strcmp(s2.get_str(), "a12*34bcd!!!") == 0);

    s3.insert(std::string_view("Test"), 3);
    std::string_view inserted(s3.get_str() + 3, 4);
    REQUIRE(inserted == "Test");

    // Error Checking
    StaticString<10> s4("1234567890");
    CHECK_THROWS_AS(s4.insert('x',0), std::out_of_range);
    CHECK_THROWS_AS(s4.insert('x',5), std::out_of_range);
    CHECK_THROWS_AS(s4.insert('x',10), std::out_of_range);
    CHECK_THROWS_AS(s4.insert('x',9), std::out_of_range);

    CHECK_THROWS_AS(s4.insert("TEST",0), std::out_of_range);
    CHECK_THROWS_AS(s4.insert(std::string_view("Test"),4), std::out_of_range);

    CHECK_THROWS_AS(s4.insert("TEST", 10), std::out_of_range);
    CHECK_THROWS_AS(s4.insert(std::string("test"),0), std::out_of_range);

    StaticString<10> s5("12345678");
    CHECK_THROWS_AS(s5.insert("TEST",0), std::out_of_range);

    // Check if nullptr handled correctly
    // S2 is a12*34bcd!!!
    s2.set(3,'\0');
    CHECK(std::strcmp(s2.get_str(), "a12") == 0);

    s2.insert("TEST", 2);
    CHECK(std::strcmp(s2.get_str(), "a1TEST2") == 0);

    s2.insert("---", 7);
    CHECK(std::strcmp(s2.get_str(), "a1TEST2---") == 0);
  }

  SECTION("TESTS: Erase  member function") {
    StaticString<40> s("aaabbbcccaaa");

    s.erase("aaa");
    REQUIRE(std::strcmp(s.get_str(), "bbbccc") == 0);

    s = "aaabbbcccaaa";
    s.erase("aaa", 0);
    REQUIRE(std::strcmp(s.get_str(), "bbbcccaaa") == 0);

    s = "abcbdb";
    s.erase('b');
    REQUIRE(std::strcmp(s.get_str(), "acd") == 0);

    s.erase('f');
    REQUIRE(std::strcmp(s.get_str(), "acd") == 0);

    s.erase("acd");
    REQUIRE(std::strcmp(s.get_str(), "") == 0);

    s = "abababab";
    s.erase("ab");
    REQUIRE(std::strcmp(s.get_str(), "") == 0);
  }

  SECTION("TESTS: Remove member function") {
    StaticString<40> s("aaabbbcccaaa");

    s.remove(0,2);
    REQUIRE(std::strcmp(s.get_str(), "abbbcccaaa") == 0);

    s = "aaabbbcccaaa";
    s.remove(3,9);
    REQUIRE(std::strcmp(s.get_str(), "aaaaaa") == 0);

    s = "aaabbbcccaaa";
    s.remove(9,12);
    REQUIRE(std::strcmp(s.get_str(), "aaabbbccc") == 0);

    // Check Error cases 
    CHECK_THROWS_AS(s.remove(2,0), std::out_of_range);
    CHECK_THROWS_AS(s.remove(0,0), std::out_of_range);
    CHECK_THROWS_AS(s.remove(2,10), std::out_of_range);

    s.remove(0,1);
    REQUIRE(std::strcmp(s.get_str(), "aabbbccc") == 0);
  }

  SECTION("TESTS: Swap member function") {
    StaticString<40> s("Hello World Hello");
    StaticString<40> s2("Testing New Function");

    REQUIRE(std::strcmp(s.get_str(), "Hello World Hello") == 0);
    REQUIRE(std::strcmp(s2.get_str(), "Testing New Function") == 0);

    s.swap(s2);
    REQUIRE(std::strcmp(s2.get_str(), "Hello World Hello") == 0);
    REQUIRE(std::strcmp(s.get_str(), "Testing New Function") == 0);

    
    s.swap(s2, [](const auto& lhs, const auto& rhs) {
      return lhs.length() == rhs.length();
    });

    REQUIRE(std::strcmp(s2.get_str(), "Hello World Hello") == 0);
    REQUIRE(std::strcmp(s.get_str(), "Testing New Function") == 0);

    s = "hello";
    s2 = "world";

    REQUIRE(std::strcmp(s.get_str(), "hello") == 0);
    REQUIRE(std::strcmp(s2.get_str(), "world") == 0);

    s.swap(s2, [](const auto& lhs, const auto& rhs) {
      return lhs.length() == rhs.length();
    });

    REQUIRE(std::strcmp(s.get_str(), "world") == 0);
    REQUIRE(std::strcmp(s2.get_str(), "hello") == 0);

    StaticString<10> a("abcdefghij"); 
    StaticString<5> b("123");        

    REQUIRE_THROWS_AS(a.swap(b), std::out_of_range);

    REQUIRE_THROWS_AS(a.swap(b, [](const auto&, const auto&) {
      return true;
    }), std::out_of_range);

    // Test to see if StaticString Compare works with swap
    StaticString<10> c("abc");
    StaticString<10> d("ABC");

    auto func = [](char lhs, char rhs) {
      return std::tolower(lhs) == std::tolower(rhs);
    };

    c.swap(d, [&](const auto& lhs, const auto& rhs) {
      return lhs.compare(rhs, func);
    });
  
    REQUIRE(c == "ABC");
    REQUIRE(d == "abc");

  }

  SECTION("TESTS: Trim  member function") {
    StaticString<30> s("Hello World   ");

    s.trim();
    REQUIRE(std::strcmp(s.get_str(), "Hello World") == 0);

    s = "test";
    s.trim();
    REQUIRE(std::strcmp(s.get_str(), "test") == 0);

    // Removes the leading space 
    s = "   Hello World";

    s.trim();
    CHECK(std::strcmp(s.get_str(), "Hello World") == 0);

    s = "   Hello World   ";

    s.trim();
    CHECK(std::strcmp(s.get_str(), "Hello World") == 0);
  }
  
  
  SECTION("TESTS: Foreach with lambda  member function") {
    StaticString<20> s("abc");
    std::string collected;
    
    s.foreach([&collected](char ch) { collected.push_back(ch); });
    REQUIRE(collected == "abc");

    int count = 0;
    s.foreach([&count](char) { ++count; });
    REQUIRE(count == 3);


    StaticString<50> s2("Hello World");
    int vowelCount = 0;

    s2.foreach([&vowelCount](char& ch) {
         // If the character is a vowel, count it and convert to uppercase
         if (std::strchr("aeiou", ch)) {
             vowelCount++;
             ch = static_cast<char>(std::toupper(ch));
         }
    });

    REQUIRE(vowelCount == 3);
    REQUIRE(std::strcmp(s2.get_str(), "HEllO WOrld") == 0);
  }
  
  SECTION("TESTS: Transform with lambda member function") {
    StaticString<20> s("abc");

    auto transformed = s.transform([](char ch) -> char {
        if (ch >= 'a' && ch <= 'z')
            return static_cast<char>(ch - 'a' + 'A');
        return ch;
    });
    REQUIRE(std::strcmp(s.get_str(), "abc") == 0);
    REQUIRE(std::strcmp(transformed.get_str(), "ABC") == 0);
  }
  
  SECTION("TESTS: Count member function") { 
    StaticString<40> s("aabbccddeeff");

    auto counts = s.char_count();

    REQUIRE(counts['a'] == 2);
    REQUIRE(counts['b'] == 2);
    REQUIRE(counts['c'] == 2);
    REQUIRE(counts['d'] == 2);
    REQUIRE(counts['e'] == 2);
    REQUIRE(counts['f'] == 2);

    StaticString<40> s2("a");

    auto counts2 = s2.char_count();
    REQUIRE(counts2['a'] == 1);

    StaticString<40> s3("ababbabbaab");

    auto counts3 = s3.char_count();
    REQUIRE(counts3['a'] == 5);
    REQUIRE(counts3['b'] == 6);
  }

  SECTION("TESTS: split member function") {
    StaticString<30> s("Hello,World,Test");
    std::vector<StaticString<30>> result;

    result = s.split(',');

    REQUIRE(result.size() == 3);
    REQUIRE(result[0].to_string() == "Hello");
    REQUIRE(result[1].to_string() == "World");
    REQUIRE(result[2].to_string() == "Test");

    // Test to see if delimiter at the end
    StaticString<30> s2("Hello,World,Test,");
    std::vector<StaticString<30>> result2;

    result2 = s2.split(',');

    REQUIRE(result2.size() == 3);
    REQUIRE(result2[0].to_string() == "Hello");
    REQUIRE(result2[1].to_string() == "World");
    REQUIRE(result2[2].to_string() == "Test");

    // Test to see if delimiter at the beginning
    StaticString<30> s3(",Hello,World,Test");
    std::vector<StaticString<30>> result3;

    result3 = s3.split(',');

    REQUIRE(result3.size() == 3);
    REQUIRE(result3[0].to_string() == "Hello");
    REQUIRE(result3[1].to_string() == "World");
    REQUIRE(result3[2].to_string() == "Test");


    StaticString<30> s4("bcdabcdabcda");
    std::vector<StaticString<30>> result4;

    result3 = s4.split('a');
    REQUIRE(result3.size() == 3);
    REQUIRE(result3[0].to_string() == "bcd");
    REQUIRE(result3[1].to_string() == "bcd");
    REQUIRE(result3[2].to_string() == "bcd");

  }

  SECTION("TESTS: compare member function") {
    StaticString<10> s1("Hello");
    StaticString<10> s2("Hello");
    StaticString<10> s3("World");

    REQUIRE(s1.compare(s2, [](char a, char b) { return a == b; }) == true);
    REQUIRE(s1.compare(s3, [](char a, char b) { return a == b; }) == false);

    /*
      For the application part 
      StaticString<10> s1("Hello")
      StaticString<10> s1("__l*") 
      with a custom match pattern function or a class will be fed in to 
      compare function as comparator thus will be able to check if the 
      pattern matches.
    */
  }

  SECTION("TESTS: empty and clear") {
    StaticString<10> s1("Hello");
    StaticString<10> s2("Hello World");
    StaticString<10> s3("");

    REQUIRE(s1.empty() == false);
    s1.clear();
    REQUIRE(s1.empty() == true);

    for(auto i : s1) {
      REQUIRE(i == '\0');
    }

    REQUIRE(s3.empty() == true);
    s3.clear();
    REQUIRE(s3.empty() == true);


    REQUIRE(s2.empty() == false);
    s2.set(5,'\0');
    REQUIRE(s2.empty() == false);

    s2.clear();
    REQUIRE(s2.empty() == true);

    for(auto i : s2) {
      REQUIRE(i == '\0');
    }
  }
}

TEST_CASE("Tests for advanced edge case tests", "[StaticString]") {
  SECTION("TESTS: Constructor templated types") {
    StaticString<10> s("Hello World");
    REQUIRE(std::strcmp(s.get_str(), "Hello Worl") == 0); // Longer string but only allowed part used

    StaticString<10> s1("Hello World TEST");
    REQUIRE(std::strcmp(s1.get_str(), "Hello Worl") == 0); // Longer string but only allowed part used


    std::string input = "std::String";
    StaticString<11> s2(input);
    REQUIRE(std::strcmp(s2.get_str(), "std::String") == 0);

    std::string_view view{"std::string_view"};
    StaticString<30> s3(view);
    REQUIRE(std::strcmp(s3.get_str(), "std::string_view") == 0);

    StaticString<10> str1("hi");
    StaticString<10> str2 = str1;
    REQUIRE(std::strcmp(str2.get_str(), str1.get_str()) == 0);
    REQUIRE(std::strcmp(str2.get_str(), "hi") == 0);
  }

  SECTION("TESTS: operator= to work with std::string,string_view,char*") {
    StaticString<10> s1;
    StaticString<10> s2;
    StaticString<10> s3;
    StaticString<10> s4;
    StaticString<10> s5;
    StaticString<4> s6;

    CHECK(s1.empty() == true);
    CHECK(s2.empty() == true);
    CHECK(s3.empty() == true);
    CHECK(s4.empty() == true);
    CHECK(s5.empty() == true);
    CHECK(s6.empty() == true);

    std::string stdTest = "Hello";
    const std::string stdTestConst = "!!!!!";
    auto charTest = "World";
    std::string_view viewTest("TEST");
    const char* pointerTest = "WORKS";

    s1 = stdTest;
    s2 = stdTestConst;
    s3 = charTest;
    s4 = viewTest;
    s5 = pointerTest;

    // Check all types are working with = 
    REQUIRE(std::strcmp(s1.get_str(), "Hello") == 0);
    REQUIRE(std::strcmp(s2.get_str(), "!!!!!") == 0);
    REQUIRE(std::strcmp(s3.get_str(), "World") == 0);
    REQUIRE(std::strcmp(s4.get_str(), "TEST") == 0);
    REQUIRE(std::strcmp(s5.get_str(), "WORKS") == 0);

    // Test to see if correctly error is raised when string is too long
    std::string exceedsLimit = "HelloWorld";
    CHECK_THROWS_AS(s6 = exceedsLimit, std::out_of_range);

    // Test empty strings 
    s1 = "";
    REQUIRE(std::strcmp(s1.get_str(), "") == 0);
    
    s2.clear();
    REQUIRE(std::strcmp(s2.get_str(), "") == 0);

    s3 = std::nullptr_t();
    REQUIRE(std::strcmp(s3.get_str(), "") == 0);
  }

  SECTION("TESTS: Memory tests") {
    // Test to see beyond memeory is still preserverd or not
    StaticString<10> s("HelloWorld");
    s.set(5,'\0');

    REQUIRE(std::strcmp(s.get_str(), "Hello") == 0);

    std::size_t count = 0;
    char expected[11] = {'H','e','l','l','o','\0','o','r','l','d','\0'};

    for(auto item : s) {
      REQUIRE(item == expected[count]);
      count++;
    }

    SECTION("TESTS: View function tests") {
      StaticString<10> s;

      auto v = s.view();

      REQUIRE(v.empty());
      REQUIRE(v.size() == 0);
      REQUIRE(v.data() != nullptr);

      s = "Hello!!";
      auto v2 = s.view();

      REQUIRE(v2.empty() == false);
      REQUIRE(v2.size() == 7);
      REQUIRE(v2.data() == std::string_view("Hello!!"));
    }
  }

}