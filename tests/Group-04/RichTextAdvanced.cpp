#include <string>

#include "IndexSet.hpp"
#include "catch.hpp"

// Mocks
namespace cse {
template <typename T>
class RichText;

struct Format {
  std::string name;
  std::variant<std::string, int32_t, std::monostate> metadata;
}

}  // namespace cse

// example composite template usage
template <typename T>
class Document {
  using text_t = cse::RichText<T>;

  // tree structure
  class Node;
  using child_t = std::variant<text_t, std::vector<Node>>;
  class Node {
    std::vector<child_t> children;
  };

  Node root;
};

using RichString = cse::RichText<std::string>

// this test case doesn't actually have assertions, it just needs to compile
TEST_CASE("RichText template support", "[RichTextAdvanced]") {
  // basic templates
  cse::RichText<std::string> text1;
  cse::RichText<std::wstring> text2;

  // containers
  cse::RichText<std::vector<int>> text3;
  cse::RichText<std::vector<std::string>> text4;

  // usage in composite templates
  Document<std::string> doc;
}

TEST_CASE("RichText move semantics", "[RichTextAdvanced]") {
  RichString text1{"foo"};
  text1 = std::move("bar");
  REQUIRE(text1.to_string() == "bar");

  RichString text2{text1};
  REQUIRE(text2.to_string() == "bar");
}

TEST_CASE("New RichText operations", "[RichTextAdvanced]") {
  RichString::Format red("color", "red");
  RichString::Format blue("color", "blue");

  RichString text1{"hello"};
  text1.apply_format_to_range(red, 0, text1.size() - 1);

  RichString text2{"world"};
  text2.apply_format_to_range(blue, text2.size() - 1);

  // Add unformatted punctuation
  text1.insert(text1.size() - 1, ", ");
  // Add formatted substring
  text2.insert(0, text1);

  REQUIRE(text2.to_string() == "hello, world!");
  REQUIRE(text2.get_format_range(red) == std::optional{cse::IndexSet{0, 4}});
  REQUIRE(text2.get_format_range(blue) ==
          std::optional{cse::IndexSet{7, text2.size() - 1}});
  REQUIRE(text2.formats_at(5).size() == 0)
}
