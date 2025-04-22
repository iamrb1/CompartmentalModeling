#include "RichTextState.hpp"
#include "catch.hpp"

TEST_CASE("RichTextState default initialization", "[RichTextState]") {
  // Create a RichTextState; its constructor now only sets default text.
  RichTextState state;

  // Verify the default text.
  REQUIRE(state.getText() == "Hello, colorful bold world!");

  // Retrieve serialized outputs (expecting plain formatting, since no methods
  // were called).
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

TEST_CASE("Italicize text", "[RichTextSerialize][BasicItalic]") {
  RichTextState state{};
  state.edit_start_pos(7);
  state.applyItalic();

  std::string expected = "<b>Hello</b><i> World!</i>";
  REQUIRE(state.getHTML() == expected);
}
