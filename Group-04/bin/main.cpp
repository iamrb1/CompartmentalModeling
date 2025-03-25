#define CSE4_IMPL

#include "IndexSet.hpp"
#include "RichText.hpp"

int main() {
  cse::RichText text{"pillow"};
  text.apply_format_to_range(cse::RichText::Format{"bold"}, 2, 4);
  cse_assert_eq(text.formats_at(0).size(), size_t{0});
  cse_assert_eq(text.formats_at(1).size(), size_t{0});
  cse_assert_eq(text.formats_at(2).size(), size_t{1});
  cse_assert_eq(text.formats_at(3).size(), size_t{1});
  cse_assert_eq(text.formats_at(4).size(), size_t{0});
  cse_assert_eq(text.formats_at(5).size(), size_t{0});

  text.update([](std::string &string) {
    string.erase(5, 1);
    string.erase(0, 1);
    return cse::IndexSet{0, 1} | cse::IndexSet{5, 6};
  });

  cse_assert_eq(text.to_string(), "illo");
  cse_assert_eq(text.formats_at(0).size(), size_t{0});
  cse_assert_eq(text.formats_at(1).size(), size_t{1});
  cse_assert_eq(text.formats_at(2).size(), size_t{1});
  cse_assert_eq(text.formats_at(3).size(), size_t{0});
  return 0;
}
