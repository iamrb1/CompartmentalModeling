#include <string>

#include "RichText.hpp"
#include "IndexSet.hpp"
#include "catch.hpp"


// example composite template usage
template <typename T> class Document {
  using text_t = cse::RichText<T>;

  // tree structure
  class Node;
  using child_t = std::variant<text_t, std::vector<Node>>;
  class Node {
    std::vector<child_t> children;
  };

  Node root;
};

// this test case doesn't actually have assertions, it just needs to compile
TEST_CASE("RichText template support", "[RichTextAdvanced]") {
  // basic templates
  cse::RichText text1;
  cse::RichText<wchar_t> text2;

  // containers
  cse::RichText<int> text3;
  // FIXME: Can not use non-trivial character types (yet?).
  // cse::RichText<std::vector<std::string>> text4;

  // usage in composite templates
  Document<char> doc;
}

TEST_CASE("RichText move semantics", "[RichTextAdvanced]") {
  cse::RichText text1{"foo"};
  text1 = std::move("bar");
  REQUIRE(text1.to_string() == "bar");

  cse::RichText text2{text1};
  REQUIRE(text2.to_string() == "bar");
}

TEST_CASE("New RichText operations", "[RichTextAdvanced]") {
  cse::TextFormat red("color", "red");
  cse::TextFormat blue("color", "blue");

  cse::RichText text1{"hello"};
  text1.apply_format_to_range(red, 0, text1.size() - 1);

  cse::RichText text2{"world!"};
  text2.apply_format_to_range(blue, 0, text2.size() - 1);

  // Add unformatted punctuation
  text1.insert(text1.size(), ", ");
  // Add formatted substring
  text2.insert(0, text1);

  REQUIRE(text2.to_string() == "hello, world!");
  // TODO: Spaceship operator on IndexSet
  //REQUIRE(text2.get_format_range(red) == std::optional{cse::IndexSet{0, 4}});
  //REQUIRE(text2.get_format_range(blue) ==
  //        std::optional{cse::IndexSet{7, text2.size() - 1}});
  REQUIRE(text2.formats_at(5).size() == 0);
}

TEST_CASE("RichText update container functionality", "[RichTextAdvanced]") {
  cse::TextFormat red("color", "red");
  cse::RichText text{"hello"};
  text.apply_format_to_range(red, 0, text.size() - 1);

  text.update([](std::string &string) {
    string[0] = 'j';
    // nullopt for substitutions only
    return std::nullopt;
  });
  REQUIRE(text.to_string() == "jello");

  text.update([](std::string &string) {
    // substitutions can be performed at the same time as insertions or
    // deletions (but insertions and deletions cannot be performed at the same
    // time)
    string[0] = 'p';
    string[1] = 'i';
    string.push_back('w');

    // insertion: return indices in the original string that had
    // characters inserted after them.
    //
    // might be more complex for multi-character discontinuous insertions, or
    // just disallow that
    return cse::IndexSet{4, 4};
  });

  REQUIRE(text.to_string() == "pillow");
  // expand format from insertion
  REQUIRE(text.formats_at(5).size() == 1);

  text.update([](std::string &string) {
    string.erase(4, 2);
    string.erase(0, 1);
    // deletion: return indices removed in old string
    return cse::IndexSet{0, 0} | cse::IndexSet{4, 5};
  });

  REQUIRE(text.to_string() == "ill");

  // edge cases are tricky, we might just have to limit what is
  // allowed, or reduce scope to exclude expanding formats (instead, just
  // offseting them)
}
