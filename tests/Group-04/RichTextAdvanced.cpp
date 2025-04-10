#include <string>
#include "CseString.hpp"
#include "IndexSet.hpp"
#include "RichText.hpp"
#include "catch.hpp"
#include "RichTextState.hpp"


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

  RichText text{"abcdefgh"};  // a, b, c, d, e, f, g, h
  TextFormat bold("bold");
  TextFormat italic("italic");

  // Apply bold to [2,5) => c, d, e
  text.apply_format(bold, 2, 5);

  // Apply italic to [4,7) => e, f, g
  text.apply_format(italic, IndexSet({4, 7}));

  REQUIRE(text.to_string() == "abcdefgh");
  REQUIRE(text.get_format_range(bold).value() == IndexSet({2, 5}));
  REQUIRE(text.get_format_range(italic).value() == IndexSet({4, 7}));

  SECTION("Erase partial overlap in the middle (index/count)") {
    text.erase(3, 2);  // Remove characters at indices 3 and 4.
    REQUIRE(text.to_string() == "abcfgh");
    REQUIRE(text.get_format_range(bold).value() == IndexSet{2UL});
    REQUIRE(text.get_format_range(italic).value() == IndexSet{3UL, 4UL});
  }

  SECTION("Erase entire bold region plus part of italic region") {
    text.erase(2, 4);  // Remove characters at indices 2,3,4,5.
    REQUIRE(text.to_string() == "abgh");
    REQUIRE_FALSE(text.get_format_range(bold).has_value());
    REQUIRE(text.get_format_range(italic).value() == IndexSet{2UL});
  }

  SECTION("Erase with IndexSet: multiple disjoint ranges") {
    // Remove b (index 1) and f,g (indices 5,6).
    IndexSet ranges;
    ranges.insert_range(1UL, 2UL);
    ranges.insert_range(5UL, 7UL);
    text.erase(ranges);
    REQUIRE(text.to_string() == "acdeh");
    REQUIRE(text.get_format_range(bold).value() == IndexSet{1UL, 2UL, 3UL});
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

  text.apply_format(bold, 2, 5);              // c, d, e.
  text.apply_format(italic, cse::IndexSet({4, 7}));   // e, f, g.
  text.apply_format(red, 11, 12);             // l.

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

// --------------------------------------------------------------------
// NEW TESTS FOR RichTextState 
// --------------------------------------------------------------------
TEST_CASE("RichTextState default initialization", "[RichTextState]") {
    // Create a RichTextState; its constructor now only sets default text.
    RichTextState state;
    
    // Verify the default text.
    REQUIRE(state.getText() == "Hello, colorful bold world!");
    
    // Retrieve serialized outputs (expecting plain formatting, since no methods were called).
    std::string html = state.getHTML();
    INFO("HTML output: " << html);
    // Expect the output to contain the text.
    REQUIRE(html.find("Hello, colorful bold world!") != std::string::npos);
    
    std::string md = state.getMarkdown();
    INFO("Markdown output: " << md);
    REQUIRE(md.find("Hello, colorful bold world!") != std::string::npos);
    
    std::string rawHtml = state.getRawHTML();
    INFO("Raw HTML output: " << rawHtml);
    REQUIRE(rawHtml.find("Hello, colorful bold world!") != std::string::npos);
}

TEST_CASE("RichTextState updates", "[RichTextState]") {
    RichTextState state;
    
    // Update the text.
    state.setText("Testing state update");
    REQUIRE(state.getText() == "Testing state update");
    
    // Check that getHTML() reflects the new text.
    std::string html = state.getHTML();
    INFO("After setText: " << html);
    REQUIRE(html.find("Testing state update") != std::string::npos);
    
    // Apply bold to "state" (positions 8 to 13).
    state.applyBold(8, 13);
    html = state.getHTML();
    INFO("After applyBold: " << html);
    // Expect <b> and </b> tags to be present.
    REQUIRE(html.find("<b>") != std::string::npos);
    REQUIRE(html.find("</b>") != std::string::npos);
    
    // Apply blue color to "update" (positions 14 to end).
    state.applyColor("blue", 14, state.getText().size());
    html = state.getHTML();
    INFO("After applyColor: " << html);
    // The HTML serializer (SerializerHTML) uses std::format with a pattern like
    // "<span style=\"color: {};\">"
    // which for "blue" becomes "<span style=\"color: blue;\">"
    // So we test for "color: blue" (with a space).
    REQUIRE(html.find("color: blue") != std::string::npos);
}

// --------------------------------------------------------------------
// NEW TESTS FOR UNDO/REDO FUNCTIONALITY IN RichTextState
// --------------------------------------------------------------------
TEST_CASE("RichTextState undo/redo functionality", "[RichTextState]") {
    // Create a new state.
    RichTextState state;
    
    // Set initial text.
    state.setText("First state");
    REQUIRE(state.getText() == "First state");
    
    // Change to a new state.
    state.setText("Second state");
    REQUIRE(state.getText() == "Second state");
    
    // Undo: should revert to "First state".
    state.undo();
    REQUIRE(state.getText() == "First state");
    
    // Redo: should reapply "Second state".
    state.redo();
    REQUIRE(state.getText() == "Second state");

    // Now, test with formatting operations.
    state.setText("Undo formatting test");
    // No formatting applied yet.
    std::string html_noFormat = state.getHTML();
    // Apply bold to "formatting" (positions 5 to 16).
    state.applyBold(5, 16);
    std::string html_withBold = state.getHTML();
    // Expect the bold tags to appear in the HTML output.
    REQUIRE(html_withBold.find("<b>") != std::string::npos);
    
    // Undo the formatting change.
    state.undo();
    std::string html_afterUndo = state.getHTML();
    // Now the bold tags should not be present.
    REQUIRE(html_afterUndo.find("<b>") == std::string::npos);
    
    // Redo the formatting change.
    state.redo();
    std::string html_afterRedo = state.getHTML();
    // Bold tags should be present again.
    REQUIRE(html_afterRedo.find("<b>") != std::string::npos);
}

