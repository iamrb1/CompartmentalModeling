#include "RichTextState.hpp"
#include "catch.hpp"

TEST_CASE("Italicize text", "[RichTextSerialize][BasicItalic]") {
  RichTextState state{};
  state.edit_start_pos(7);
  state.applyItalic();

  std::string expected = "<b>Hello</b><i> World!</i>";
  REQUIRE(state.getHTML() == expected);
}
