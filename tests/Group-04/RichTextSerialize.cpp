#include "catch.hpp"

#include "RichText.hpp"

TEST_CASE("RichText basic serialization", "[RichTextSerialize]") {
  cse::TextFormat bold("bold");
  cse::RichText text{"hello"};
  text.apply_format(bold);

  cse::RichText::Serializer serializer{"HTML"};
  serializer.AddRule("bold", "<strong>", "</strong>");

  std::string output = text.serialize(serializer).output;
  std::string expected = "<strong>hello</strong>";
  REQUIRE(output == expected);
}

TEST_CASE("RichText complex serialization", "[RichTextSerialize]") {
  std::string header = R"(<style>
.red {
  color: #ff0000
}
.blue{
  color: #0000ff
}
</style>
<body>)";

  std::string footer = "\n</body>";

  cse::TextFormat red("color", "red");
  cse::TextFormat blue("color", "blue");

  cse::RichText text{"hello world"};
  text.apply_format(red, 0, 5);
  text.apply_format(blue, 6, text.size());

  cse::RichText::Serializer serializer{"HTML", header, footer};

  auto color_start = [](cse::TextFormat const &format) {
    cse_assert(std::holds_alternative<std::string>(format.metadata));
    std::string color = std::get<std::string>(format.metadata);
    return std::format("<span class={}>", color);
  };
  serializer.AddRule("color", color_start, "</span>");

  std::string output = text.serialize(serializer).output;

  REQUIRE(output ==
          (header +
           "<span class=red>hello</span> <span class=blue>world</span>" +
           footer));
}
