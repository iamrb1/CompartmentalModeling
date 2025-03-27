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
  text.apply_format(italic, cse::IndexSet{std::pair{4, 8}});

  SECTION("Test whole clear") {
    // TODO
    // text.clear();
    REQUIRE_FALSE(text.get_format_range(bold).has_value());
    REQUIRE_FALSE(text.get_format_range(italic).has_value());
  }

  SECTION("Test IndexSet clear") {
    // TODO
    // text.clear(cse::IndexSet{2, 3});
    REQUIRE(text.get_format_range(bold).value() == cse::IndexSet{0, 1, 4});
    REQUIRE(text.get_format_range(italic).value() == cse::IndexSet{});
  }

  SECTION("Test format clear") {
    // TODO
    // text.clear(italic);
    REQUIRE(text.get_format_range(bold).value() ==
            cse::IndexSet{std::pair{0, 5}});
    REQUIRE_FALSE(text.get_format_range(italic).has_value());
  }

  SECTION("Test format and IndexSet clear") {
    // TODO
    // text.clear(bold, cse::IndexSet{2, 3});
    REQUIRE(text.get_format_range(bold).value() == cse::IndexSet{0, 1, 4});
    REQUIRE(text.get_format_range(italic).value() ==
            cse::IndexSet{std::pair{4, 8}});
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
  REQUIRE(text2.get_format_range(red) ==
          std::optional{cse::IndexSet{std::pair{0, 5}}});
  REQUIRE(text2.get_format_range(blue) ==
          std::optional{cse::IndexSet{std::pair{7, text2.size()}}});
  REQUIRE(text2.formats_at(5).size() == 0);
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
