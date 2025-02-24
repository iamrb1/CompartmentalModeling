#include <string>
#include "RichText.hpp"
#include "catch.hpp"

// Test constructor and size calculation
TEST_CASE("Constructor", "[RichTextTest]") {
    cse::RichText text1;
    REQUIRE(text1.size() == 0);

    cse::RichText text2("string");
    REQUIRE(text2.size() == 6);

    std::string str1 = "string";
    cse::RichText text3(str1);
    REQUIRE(text3.size() == 6);
}

TEST_CASE("MultipleFormatting", "[RichTextTest]") {
    cse::RichText text1("hello world");
    REQUIRE(text1.size() == 11);
    cse::RichText::Format bold("bold");

    text1.applyFormatToRange(bold, 0, 5);
    REQUIRE(text1.formatsAt(3)->size() == 1);
    REQUIRE(text1.formatsAt(3)->at(0) == bold);
    REQUIRE(text1.formatsAt(8)->size() == 0);

    std::string str2 = "bold";
    cse::RichText::Format f2(str2);
    REQUIRE(text1.formatsAt(3)->at(0) == f2);

    cse::RichText::Format italic("italic");
    text1.applyFormatToRange(italic, 4, 8);
    REQUIRE(text1.formatsAt(3)->size() == 1);
    REQUIRE(text1.formatsAt(3)->at(0) == bold);
    REQUIRE(text1.formatsAt(4)->size() == 2);
    REQUIRE(text1.formatsAt(4)->at(0) == bold);
    REQUIRE(text1.formatsAt(4)->at(1) == italic);
    REQUIRE(text1.formatsAt(7)->size() == 1);
    REQUIRE(text1.formatsAt(7)->at(0) == italic);
    REQUIRE(text1.formatsAt(8)->size() == 0);
}

TEST_CASE("AppendFormattedText", "[RichTextTest]") {
    cse::RichText text1("hello");
    cse::RichText::Format bold("bold");
    text1.applyFormatToRange(bold, 0, 5);

    cse::RichText text2(" world");
    cse::RichText::Format italic("italic");
    text2.applyFormatToRange(italic, 0, 6);

    text1 += text2;

    REQUIRE(text1.size() == 11);
    REQUIRE(text1.toString() == std::string("hello world"));
    REQUIRE(text1.formatsAt(3)->size() == 1);
    REQUIRE(text1.formatsAt(3)->at(0) == bold);
    REQUIRE(text1.formatsAt(8)->size() == 1);
    REQUIRE(text1.formatsAt(8)->at(0) == italic);
}