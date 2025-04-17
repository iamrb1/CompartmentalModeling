#include "RichText.hpp"
#include "SerializerHTML.hpp"
#include "SerializerMarkdown.hpp"
#include "SerializerRTF.hpp"
#include "SerializerLaTeX.hpp"
#include "catch.hpp"

TEST_CASE("Basic serialization", "[RichTextSerialize]") {
  cse::TextFormat bold("bold");
  cse::RichText text{"hello"};
  text.apply_format(bold);

  // add rule for strong
  cse::RichText::Serializer serializer{"HTML"};
  serializer.AddRule("bold", "<strong>", "</strong>");

  // test strong rule was applied
  std::string output = text.serialize(serializer).output;
  std::string expected = "<strong>hello</strong>";
  REQUIRE(output == expected);
}

TEST_CASE("Wide string serialization", "[RichTextSerialize]") {
  using WideText = cse::BasicRichText<wchar_t>;
  cse::TextFormat italic("italic");
  WideText text{L"meow"};
  text.apply_format(italic);

  // add rule for emphasis, with wide string literals for the rule tokens
  WideText::Serializer serializer{"HTML"};
  serializer.AddRule("italic", L"<em>", L"</em>");

  // check the output is also a wide string
  std::wstring output = text.serialize(serializer).output;
  std::wstring expected = L"<em>meow</em>";
  REQUIRE(output == expected);
}

TEST_CASE("Complex serialization", "[RichTextSerialize]") {
  // some header/footer code to add the proper colors to the generated HTML
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

  // set the header and footer
  cse::RichText::Serializer serializer{"HTML", header, footer};

  // rule which uses the metadata to determine color class
  auto color_start = [](cse::TextFormat const &format) {
    cse_assert(std::holds_alternative<std::string>(format.metadata));
    std::string color = std::get<std::string>(format.metadata);
    return std::format("<span class={}>", color);
  };
  serializer.AddRule("color", color_start, "</span>");

  std::string output = text.serialize(serializer).output;

  // check that the output has the correct classes and header/footer
  REQUIRE(output ==
          (header +
           "<span class=red>hello</span> <span class=blue>world</span>" +
           footer));
}

TEST_CASE("Default HTML serializer", "[RichTextSerialize]") {
  std::string header = cse::SERIALIZER_HTML_HEADER;
  std::string footer = cse::SERIALIZER_HTML_FOOTER;
  auto serializer = cse::SerializerHTML();

  cse::RichText text{"Hello world!"};
  text.apply_format(cse::TextFormat{"color", "red"}, 0, 5);

  std::string output = text.serialize(serializer).output;
  REQUIRE(
      output ==
      (header + "<span style=\"color: red;\">Hello</span> world!" + footer));
}

TEST_CASE("Default Markdown serializer", "[RichTextSerialize]") {
  auto serializer = cse::SerializerMarkdown<wchar_t>();
  cse::BasicRichText<wchar_t> text(L"Some unicode markdown!");
  text.apply_format(cse::TextFormat{"bold", true}, 0, 4);

  std::wstring output = text.serialize(serializer).output;
  REQUIRE(output == std::wstring(L"**Some** unicode markdown!"));

  cse::BasicRichText<wchar_t> text2(L"I'm a header!\nI'm text.\n");
  text2.apply_format(cse::TextFormat("header", 1), 0, 13);
  text2.apply_format(cse::TextFormat("italic"), 18, 22);

  output = text2.serialize(serializer).output;
  REQUIRE(output == std::wstring(L"\n# I'm a header!\n\nI'm *text*.\n"));

  cse::BasicRichText<wchar_t> text3(L"Click here to see the schedule.");
  text3.apply_format(
      cse::TextFormat("link", "https://cse498.github.io/schedule.html"), 6, 10);

  output = text3.serialize(serializer).output;
  REQUIRE(output ==
          std::wstring(L"Click [here](https://cse498.github.io/schedule.html) "
                       L"to see the schedule."));
}


// ────────────────────────────────────────────────────────────────────────────
// ADVANCED TESTS FOR LaTeX Serializer
// ────────────────────────────────────────────────────────────────────────────

static const std::string latex_header = R"(\documentclass{article}
\usepackage{hyperref}
\usepackage[normalem]{ulem}
\begin{document}
)";
static const std::string latex_footer = R"(
\end{document}
)";

TEST_CASE("LaTeX italic formatting", "[RichTextSerialize][LaTeX]") {
    auto serializer = cse::SerializerLaTeX<char>();
    cse::RichText text{"italic"};
    // apply italic to entire word
    text.apply_format(cse::TextFormat("italic", std::monostate()), 0, 6);

    std::string output = text.serialize(serializer).output;
    std::string expected = latex_header
                         + std::string("\\textit{italic}")
                         + latex_footer;
    REQUIRE(output == expected);
}

TEST_CASE("LaTeX strikethrough formatting", "[RichTextSerialize][LaTeX]") {
    auto serializer = cse::SerializerLaTeX<char>();
    cse::RichText text{"strike"};
    // apply strikethrough across all
    text.apply_format(cse::TextFormat("strikethrough", std::monostate()), 0, 6);

    std::string output = text.serialize(serializer).output;
    std::string expected = latex_header
                         + std::string("\\sout{strike}")
                         + latex_footer;
    REQUIRE(output == expected);
}

TEST_CASE("LaTeX link formatting", "[RichTextSerialize][LaTeX]") {
    auto serializer = cse::SerializerLaTeX<char>();
    cse::RichText text{"link text"};
    // apply link to "link"
    text.apply_format(cse::TextFormat("link", std::string("http://ex.com")), 0, 4);

    std::string output = text.serialize(serializer).output;
    // \href{url}{link} plus the trailing " text"
    std::string expected = latex_header
                         + std::string("\\href{http://ex.com}{link} text")
                         + latex_footer;
    REQUIRE(output == expected);
}

TEST_CASE("LaTeX header formatting levels", "[RichTextSerialize][LaTeX]") {
    auto serializer = cse::SerializerLaTeX<char>();
    cse::RichText text{"Heading\n"};
    // level 1 => \section{…}\n
    text.apply_format(cse::TextFormat("header", int32_t(1)), 0, 7);

    std::string output = text.serialize(serializer).output;
    // After header token, the remaining "\n" in text yields a blank line
    std::string expected = latex_header
                         + std::string("\\section{Heading}\n\n")
                         + latex_footer;
    REQUIRE(output == expected);

    // Now test level 2 => \subsection{…}\n
    text = cse::RichText{"Subheading\n"};
    text.apply_format(cse::TextFormat("header", int32_t(2)), 0, 10);

    output = text.serialize(serializer).output;
    expected = latex_header
             + std::string("\\subsection{Subheading}\n\n")
             + latex_footer;
    REQUIRE(output == expected);
}
