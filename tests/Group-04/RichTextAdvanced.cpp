#include <string>

#include "CseString.hpp"
#include "IndexSet.hpp"
#include "RichText.hpp"
#include "catch.hpp"

// this test case doesn't actually have assertions, it just needs to compile
TEST_CASE("RichText template support", "[RichTextAdvanced]") {
  // basic template (std::string)
  cse::RichText text1;

  // character type template
  cse::BasicRichText<wchar_t> text2;

  // underlying string template
  cse::BasicRichText<char, cse::String> text3;
}

TEST_CASE("RichText move semantics", "[RichTextAdvanced]") {
  cse::RichText text1{"foo"};
  text1 = std::move("bar");
  REQUIRE(text1.to_string() == "bar");

  cse::RichText text2{text1};
  REQUIRE(text2.to_string() == "bar");
}

TEST_CASE("Clear formatting", "[RichTextAdvanced]") {
  cse::RichText text("hello world");
  cse::TextFormat bold("bold");
  cse::TextFormat italic("italic");

  text.apply_format(bold, 0, 5);
  text.apply_format(italic, 4, 8);

  SECTION("Test whole clear") {
    text.clear_format();
    REQUIRE_FALSE(text.get_format_range(bold).has_value());
    REQUIRE_FALSE(text.get_format_range(italic).has_value());
  }

  SECTION("Test IndexSet clear") {
    text.clear_format(cse::IndexSet{2, 3});
    REQUIRE(text.get_format_range(bold).value() == cse::IndexSet{0, 1, 4});
    REQUIRE(text.get_format_range(italic).value() == cse::IndexSet{{4, 8}});
  }

  SECTION("Test format clear") {
    text.clear_format(italic);
    REQUIRE(text.get_format_range(bold).value() == cse::IndexSet{{0, 5}});
    REQUIRE_FALSE(text.get_format_range(italic).has_value());
  }

  SECTION("Test format and IndexSet clear") {
    text.clear_format(bold, cse::IndexSet{2, 3});
    REQUIRE(text.get_format_range(bold).value() == cse::IndexSet{0, 1, 4});
    REQUIRE(text.get_format_range(italic).value() == cse::IndexSet{{4, 8}});
  }
}

TEST_CASE("New RichText operations", "[RichTextAdvanced]") {
  cse::TextFormat red("color", "red");
  cse::TextFormat blue("color", "blue");

  cse::RichText text1{"hello"};
  text1.apply_format(red, 0, text1.size());

  cse::RichText text2{"world!"};
  text2.apply_format(blue, 0, text2.size());

  // Add unformatted punctuation
  text1.insert(text1.size(), ", ");
  // Add formatted substring
  text2.insert(0, text1);

  REQUIRE(text2.to_string() == "hello, world!");
  REQUIRE(text2.get_format_range(red) == std::optional{cse::IndexSet{{0, 5}}});
  REQUIRE(text2.get_format_range(blue) ==
          std::optional{cse::IndexSet{{7, text2.size()}}});
  REQUIRE(text2.formats_at(4).size() == 1);
  REQUIRE(text2.formats_at(5).size() == 0);
  REQUIRE(text2.formats_at(6).size() == 0);
  REQUIRE(text2.formats_at(7).size() == 1);
}

TEST_CASE("RichText insertions and substitution", "[RichTextAdvanced]") {
  cse::TextFormat red("color", "red");
  cse::RichText text{"hello"};
  text.apply_format(red, 0, text.size());

  // ensure substitution retains formatting
  text[0] = 'm';
  REQUIRE(text.to_string() == "mello");
  REQUIRE(text.formats_at(0) == std::vector<cse::TextFormat>{red});

  // ensure formats are properly shifted
  text.insert(3, "lo yel");
  REQUIRE(text.to_string() == "mello yello");
  REQUIRE(text.formats_at(9) == std::vector<cse::TextFormat>{red});
}

// --------------------------------------------------------------------
// TESTS FOR ERASE METHODS
// --------------------------------------------------------------------
TEST_CASE("RichText erase methods", "[RichTextAdvanced]") {
  using cse::IndexSet;
  using cse::RichText;
  using cse::TextFormat;

  RichText text{"abcdefgh"};  // Indices: 0=a 1=b 2=c 3=d 4=e 5=f 6=g 7=h
  TextFormat bold("bold");
  TextFormat italic("italic");

  // Apply bold to [2,5) => c,d,e
  text.apply_format(bold, 2, 5);

  // Apply italic to [4,7) => e,f,g
  text.apply_format(italic, IndexSet({4, 7}));

  REQUIRE(text.to_string() == "abcdefgh");
  REQUIRE(text.get_format_range(bold).value() == IndexSet({2, 5}));
  REQUIRE(text.get_format_range(italic).value() == IndexSet({4, 7}));

  SECTION("Erase partial overlap in the middle (index/count)") {
    text.erase(3, 2);  // Remove 'd'(3) and 'e'(4)
    REQUIRE(text.to_string() == "abcfgh");
    // Bold was [2,5). Now only c remains (index 2)
    REQUIRE(text.get_format_range(bold).value() == IndexSet{2UL});

    // Italic was [4,7). Removing e => 'f'(old 5)->3, 'g'(old 6)->4 => {3,4}
    REQUIRE(text.get_format_range(italic).value() == IndexSet{3UL, 4UL});
  }

  SECTION("Erase entire bold region plus part of italic region") {
    text.erase(2, 4);  // Remove c,d,e,f
    REQUIRE(text.to_string() == "abgh");
    REQUIRE_FALSE(text.get_format_range(bold).has_value());

    // italic was [4,7) => e,f,g => e,f are gone; g(6) => new index 2 => {2}
    REQUIRE(text.get_format_range(italic).value() == IndexSet{2UL});
  }

  SECTION("Erase with IndexSet: multiple disjoint ranges") {
    // Remove b => [1,2) plus f,g => [5,7)
    IndexSet ranges;
    ranges.insert_range(1UL, 2UL);  // inserts index 1
    ranges.insert_range(5UL, 7UL);  // inserts indices 5, 6

    text.erase(ranges);
    REQUIRE(text.to_string() == "acdeh");

    // Bold was [2,5) => c,d,e => after removing f,g => unchanged, but
    // removing b(1) shifts it from {2,3,4} to {1,2,3}
    REQUIRE(text.get_format_range(bold).value() == IndexSet{1UL, 2UL, 3UL});

    // Italic was [4,7) => e,f,g => removing f,g => just {4} left
    // then removing b(1) shifts {4} => {3}
    REQUIRE(text.get_format_range(italic).value() == IndexSet{3UL});
  }

  SECTION("Erase entire text") {
    text.erase(0, text.size());
    REQUIRE(text.to_string() == "");
    REQUIRE_FALSE(text.get_format_range(bold).has_value());
    REQUIRE_FALSE(text.get_format_range(italic).has_value());
  }

  SECTION("Erase zero characters does nothing") {
    text.erase(2, 0);
    REQUIRE(text.to_string() == "abcdefgh");
    REQUIRE(text.get_format_range(bold).value() == IndexSet({2, 5}));
    REQUIRE(text.get_format_range(italic).value() == IndexSet({4, 7}));
  }
}

TEST_CASE("RichText segments", "[RichTextSegment]") {
  cse::RichText text{"abcdefghijkl"};
  cse::TextFormat bold("bold");
  cse::TextFormat italic("italic");
  cse::TextFormat red("color", "red");

  // Apply bold to [2,5) => c,d,e
  text.apply_format(bold, 2, 5);

  // Apply italic to [4,7) => e,f,g
  text.apply_format(italic, cse::IndexSet({4, 7}));

  // Apply red to [11, 12) => l
  text.apply_format(red, 11, 12);

  // test helper function
  auto segment_substr = [&](cse::IndexSet::IndexRange &segment) {
    return text.to_string().substr(segment.start, segment.end - segment.start);
  };

  SECTION("Unformatted segment (beginning)") {
    auto segment = text.segment_at(0);
    REQUIRE(segment_substr(segment) == "ab");
  }

  SECTION("Bold segment") {
    auto segment = text.segment_at(3);
    REQUIRE(segment_substr(segment) == "cd");
  }

  SECTION("Overlapping segment") {
    auto segment = text.segment_at(4);
    REQUIRE(segment_substr(segment) == "e");
  }

  SECTION("Unformatted segment (middle)") {
    auto segment = text.segment_at(10);
    REQUIRE(segment_substr(segment) == "hijk");
  }

  SECTION("End segment") {
    auto segment = text.segment_at(11);
    REQUIRE(segment_substr(segment) == "l");
  }
}
