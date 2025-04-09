#include "RichText.hpp"
#include "SerializerRTF.hpp"
#include "catch.hpp"

TEST_CASE("Default RTF serializer basic formatting", "[RichTextRTF]") {
  auto serializer = cse::SerializerRTF();
  cse::RichText text{"Hello world!"};
  text.apply_format(cse::TextFormat{"bold"}, 0, 5);
  std::string rtf_output = text.serialize(serializer).output;
  REQUIRE(rtf_output.find("{\\rtf1\\ansi\\deff0\\deflang1033") != std::string::npos);
  REQUIRE(rtf_output.find("{\\b Hello") != std::string::npos);
  REQUIRE(rtf_output.find("}\n") != std::string::npos);
}

TEST_CASE("RTF serializer with multiple formats", "[RichTextRTF]") {
  auto serializer = cse::SerializerRTF();
  cse::RichText text{"Rich Text Format Test"};
  text.apply_format(cse::TextFormat{"bold"}, 0, 4);
  text.apply_format(cse::TextFormat{"italic"}, 5, 9);
  text.apply_format(cse::TextFormat{"color", "red"}, 10, 16);
  std::string rtf_output = text.serialize(serializer).output;
  REQUIRE(rtf_output.find("{\\b Rich") != std::string::npos);
  REQUIRE(rtf_output.find("{\\i Text") != std::string::npos);
  REQUIRE(rtf_output.find("{\\cf2 Format") != std::string::npos);
}

TEST_CASE("RTF serializer with hyperlinks", "[RichTextRTF]") {
  auto serializer = cse::SerializerRTF();
  cse::RichText text{"Click here to visit MSU"};
  text.apply_format(cse::TextFormat{"link", "https://msu.edu"}, 0, 10);
  std::string rtf_output = text.serialize(serializer).output;
  REQUIRE(rtf_output.find("{\\field{\\*\\fldinst HYPERLINK \"https://msu.edu\"}{\\fldrslt Click here}") != std::string::npos);
}

TEST_CASE("RTF serializer with wide characters", "[RichTextRTF]") {
  auto serializer = cse::SerializerRTF<wchar_t>();
  cse::BasicRichText<wchar_t> text(L"Unicode RTF Test");
  text.apply_format(cse::TextFormat{"bold"}, 0, 7);
  text.apply_format(cse::TextFormat{"italic"}, 8, 11);
  std::wstring rtf_output = text.serialize(serializer).output;
  REQUIRE(rtf_output.find(L"{\\b Unicode") != std::wstring::npos);
  REQUIRE(rtf_output.find(L"{\\i RTF") != std::wstring::npos);
}

TEST_CASE("RTF serializer nested formatting", "[RichTextRTF]") {
  auto serializer = cse::SerializerRTF();
  cse::RichText text{"Nested formatting test"};
  text.apply_format(cse::TextFormat{"bold"}, 0, text.size());
  text.apply_format(cse::TextFormat{"italic"}, 7, 17);
  text.apply_format(cse::TextFormat{"color", "red"}, 18, text.size());
  std::string rtf_output = text.serialize(serializer).output;
  REQUIRE(rtf_output.find("{\\b Nested") != std::string::npos);
  REQUIRE(rtf_output.find("{\\i formatting") != std::string::npos);
  REQUIRE(rtf_output.find("{\\cf2 test") != std::string::npos);
} 