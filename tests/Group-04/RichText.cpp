#include "RichText.hpp"

#include <string>

#include "catch.hpp"

using fmt_vec = std::vector<cse::TextFormat>;

// Test constructor and size calculation
TEST_CASE("Constructor", "[RichTextTest]") {
  cse::RichText text1;
  REQUIRE(text1.size() == 0);

  cse::RichText text2("string");
  REQUIRE(text2.size() == 6);
  REQUIRE(text2.to_string() == "string");

  std::string str1 = "string";
  cse::RichText text3(str1);
  REQUIRE(text3.size() == 6);
  REQUIRE(text3.to_string() == "string");

  cse::RichText text4(text3);
  REQUIRE(text4.size() == 6);
  REQUIRE(text4.to_string() == "string");
}

TEST_CASE("MultipleFormatting", "[RichTextTest]") {
  cse::RichText text1("hello world");
  REQUIRE(text1.size() == 11);
  cse::TextFormat bold("bold");

  text1.apply_format(bold, 0, 5);
  REQUIRE(text1.formats_at(3) == fmt_vec{bold});
  REQUIRE(text1.formats_at(8) == fmt_vec{});

  std::string str2 = "bold";
  cse::TextFormat f2(str2);
  REQUIRE(text1.formats_at(3).at(0) == f2);

  // check that the formats applied correctly
  cse::TextFormat italic("italic");
  text1.apply_format(italic, cse::IndexSet{std::pair{4, 8}});
  REQUIRE(text1.formats_at(3) == fmt_vec{bold});
  REQUIRE(text1.formats_at(4) == fmt_vec{bold, italic});
  REQUIRE(text1.formats_at(7) == fmt_vec{italic});
}

TEST_CASE("AppendFormattedText", "[RichTextTest]") {
  cse::RichText text1("hello");
  cse::TextFormat bold("bold");
  text1.apply_format(bold, 0, 5);

  cse::RichText text2(" world");
  cse::TextFormat italic("italic");
  text2.apply_format(italic, 0, 6);

  text1 += text2;

  REQUIRE(text1.size() == 11);
  REQUIRE(text1.to_string() == std::string("hello world"));
  REQUIRE(text1.formats_at(3) == fmt_vec{bold});
  REQUIRE(text1.formats_at(8) == fmt_vec{italic});
}

TEST_CASE("ComplexFormatting", "[RichTextTest]") {
  cse::RichText text("hello world");

  cse::TextFormat bold("bold");
  text.apply_format(bold, 0, 11);

  cse::TextFormat textSize("text size", 11);
  text.apply_format(textSize, 0, 11);

  cse::TextFormat link("link", "https://cse498.github.io/");
  text.apply_format(link, 0, 11);

  auto formats = text.formats_at(3);

  REQUIRE(formats.size() == 3);
  REQUIRE(formats.at(0) == bold);
  REQUIRE(formats.at(0).name == std::string("bold"));
  REQUIRE(std::get<std::monostate>(formats.at(0).metadata) == std::monostate());
  REQUIRE(formats.at(1) == link);
  REQUIRE(formats.at(1).name == std::string("link"));
  REQUIRE(std::get<std::string>(formats.at(1).metadata) ==
          std::string("https://cse498.github.io/"));
  REQUIRE(formats.at(2) == textSize);
  REQUIRE(formats.at(2).name == std::string("text size"));
  REQUIRE(std::get<int32_t>(formats.at(2).metadata) == 11);
}
