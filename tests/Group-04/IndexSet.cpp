#include "catch.hpp"

#define TEST_CSE_ASSERT
#include <vector>

#include "CseAssert.hpp"
#include "IndexSet.hpp"

// Test constructor and size calculation
TEST_CASE("Constructor", "[IndexSetTest]") {
  // Index constructor
  cse::IndexSet set1(1, 5, 8);
  REQUIRE(set1.get_all_indices() == std::vector<size_t>{1, 5, 8});

  // Default constructor
  cse::IndexSet set2;
  REQUIRE(set2.get_all_indices() == std::vector<size_t>{});

  // Move constructor
  cse::IndexSet set3{std::move(set1)};
  REQUIRE(set3.get_all_indices() == std::vector<size_t>{1, 5, 8});

  // Range constructors
  cse::IndexSet set4({0, 5});  // [0,5)
  REQUIRE(set4.size() == 5);

  cse::IndexSet set5({10, 15});  // [10,15)
  REQUIRE(set5.size() == 5);

  // Empty range
  cse::IndexSet set6({3, 3});  // [3,3)
  REQUIRE(set6.size() == 0);
}

// Test insert_range efficiency
TEST_CASE("InsertRange", "[IndexSetTest]") {
  cse::IndexSet set;

  // Basic range insertion
  set.insert_range(0, 5);  // [0,5)
  REQUIRE(set.size() == 5);

  // Overlapping range
  set.insert_range(3, 8);  // Should merge with [0,5) to become [0,8)
  REQUIRE(set.size() == 8);

  // Adjacent range
  set.insert_range(8, 10);  // Should merge with [0,8) to become [0,10)
  REQUIRE(set.size() == 10);

  // Disjoint range
  set.insert_range(12, 15);  // Should add [12,15)
  REQUIRE(set.size() == 13);

  // Invalid range (start >= end)
  REQUIRE_ASSERT(set.insert_range(5, 3));
  REQUIRE(set.size() == 13);  // Size should not change
}

// Test iterator functionality
TEST_CASE("Iterator", "[IndexSetTest]") {
  cse::IndexSet set;
  set.insert_range(0, 3);    // [0,3)
  set.insert_range(5, 8);    // [5,8)
  set.insert_range(10, 12);  // [10,12)

  // Test forward iteration
  std::vector<std::size_t> expected = {0, 1, 2, 5, 6, 7, 10, 11};
  std::vector<std::size_t> actual;

  for (auto it = set.begin(); it != set.end(); ++it) {
    actual.push_back(*it);
  }

  REQUIRE(actual == expected);

  // Test range-based for loop
  actual.clear();
  for (const auto &index : set) {
    actual.push_back(index);
  }

  REQUIRE(actual == expected);

  // Test const iterator
  const cse::IndexSet const_set = set;
  actual.clear();
  for (auto it = const_set.cbegin(); it != const_set.cend(); ++it) {
    actual.push_back(*it);
  }

  REQUIRE(actual == expected);
}

// Original AppendAt test
TEST_CASE("AppendAt", "[IndexSetTest]") {
  cse::IndexSet set1;
  set1.insert_range(0, 3);  // [0,3)

  cse::IndexSet set2;
  set2.insert_range(0, 4);  // [0,4)

  // Append set2 starting at index 5
  set1.append_at(set2, 5);

  // Expected result: set1 should now contain [0,3) and [5,9)
  REQUIRE(set1.contains(0));
  REQUIRE(set1.contains(1));
  REQUIRE(set1.contains(2));
  REQUIRE_FALSE(set1.contains(3));
  REQUIRE_FALSE(set1.contains(4));
  REQUIRE(set1.contains(5));
  REQUIRE(set1.contains(6));
  REQUIRE(set1.contains(7));
  REQUIRE(set1.contains(8));
  REQUIRE_FALSE(set1.contains(9));

  // Test merging case
  cse::IndexSet set3;
  set3.insert_range(0, 2);  // [0,2)
  set1.append_at(set3, 3);  // Should merge with [0,3)

  // Expected: [0,5), [5,9)
  REQUIRE(set1.contains(3));
  REQUIRE(set1.contains(4));
  REQUIRE(set1.contains(5));
}

// Test copy operations
TEST_CASE("CopyOperations", "[IndexSetTest]") {
  cse::IndexSet original;
  original.insert_range(0, 3);    // [0,3)
  original.insert_range(5, 8);    // [5,8)
  original.insert_range(10, 12);  // [10,12)

  // Test copy constructor
  cse::IndexSet copied(original);
  REQUIRE(copied.size() == original.size());

  // Verify all ranges are copied correctly
  std::vector<std::size_t> original_indices = original.get_all_indices();
  std::vector<std::size_t> copied_indices = copied.get_all_indices();
  REQUIRE(copied_indices == original_indices);

  // Modify copy and verify original is unchanged
  copied.insert(3);  // Should merge with [0,3) to become [0,4)
  REQUIRE(copied.contains(3));
  REQUIRE_FALSE(original.contains(3));

  // Test copy assignment
  cse::IndexSet assigned;
  assigned = original;
  REQUIRE(assigned.size() == original.size());

  // Verify assigned copy has correct ranges
  std::vector<std::size_t> assigned_indices = assigned.get_all_indices();
  REQUIRE(assigned_indices == original_indices);
}

// Test move operations
TEST_CASE("MoveOperations", "[IndexSetTest]") {
  cse::IndexSet original;
  original.insert_range(0, 3);    // [0,3)
  original.insert_range(5, 8);    // [5,8)
  original.insert_range(10, 12);  // [10,12)

  std::vector<std::size_t> original_indices = original.get_all_indices();

  // Test move constructor
  cse::IndexSet moved(std::move(original));
  std::vector<std::size_t> moved_indices = moved.get_all_indices();
  REQUIRE(moved_indices == original_indices);

  // Original should be in valid but empty state after move
  REQUIRE(original.size() == 0);
  REQUIRE(original.get_all_indices().empty());

  // Create new set for move assignment test
  cse::IndexSet another;
  another.insert_range(0, 3);    // [0,3)
  another.insert_range(5, 8);    // [5,8)
  another.insert_range(10, 12);  // [10,12)

  std::vector<std::size_t> another_indices = another.get_all_indices();

  // Test move assignment
  cse::IndexSet move_assigned;
  move_assigned = std::move(another);

  // Verify moved-to object has correct ranges
  std::vector<std::size_t> assigned_indices = move_assigned.get_all_indices();
  REQUIRE(assigned_indices == another_indices);

  // Original should be in valid but empty state after move
  REQUIRE(another.size() == 0);
  REQUIRE(another.get_all_indices().empty());
}

// Test set operations
TEST_CASE("SetOperations", "[IndexSetTest]") {
  cse::IndexSet set1, set2;

  // Set1: [0,3) ∪ [5,7)
  set1.insert_range(0, 3);
  set1.insert_range(5, 7);

  // Set2: [2,6)
  set2.insert_range(2, 6);

  SECTION("Test union") {
    // Expected: [0,7) - continuous range due to overlap
    cse::IndexSet expected{{0, 7}};

    auto union_set = set1 | set2;
    REQUIRE(union_set == expected);

    set1 |= set2;
    REQUIRE(set1 == expected);
  }

  SECTION("Test intersection") {
    // Expected: [2,3) ∪ [5,6)
    cse::IndexSet expected{2, 5};

    auto intersect_set = set1 & set2;
    REQUIRE(intersect_set == expected);

    set1 &= set2;
    REQUIRE(set1 == expected);
  }

  SECTION("Test difference") {
    // Expected: [0,2) ∪ [6,7)
    cse::IndexSet expected{0, 1, 6};

    auto diff_set = set1 - set2;
    REQUIRE(diff_set == expected);

    set1 -= set2;
    REQUIRE(set1 == expected);
  }

  SECTION("Test symmetric difference") {
    // Expected: [0,2) ∪ [3,5) ∪ [6,7)
    cse::IndexSet expected{0, 1, 3, 4, 6};

    auto sym_diff_set = set1 ^ set2;
    REQUIRE(sym_diff_set == expected);

    set1 ^= set2;
    REQUIRE(sym_diff_set == expected);
  }

  SECTION("Test comparisons") {
    // Test subset/superset
    cse::IndexSet subset;
    subset.insert_range(1, 2);  // [1,2)

    // Test subset (<=)
    REQUIRE(subset <= set1);
    REQUIRE_FALSE(set1 <= subset);
    REQUIRE_FALSE(set1 <= set2);

    // Test superset (>=)
    REQUIRE_FALSE(subset >= set1);
    REQUIRE(set1 >= subset);
    REQUIRE_FALSE(set1 >= set2);

    // Test equality
    REQUIRE(cse::IndexSet{0, 3, 6} == cse::IndexSet{0, 3, 6});
  }

  SECTION("Test edge cases") {
    cse::IndexSet empty_set;
    REQUIRE(empty_set <= set1);  // Empty set is subset of all sets
    REQUIRE(set1 >= empty_set);  // All sets are superset of empty set
    REQUIRE((empty_set | set1).size() == set1.size());  // Union with empty set
    REQUIRE((empty_set & set1).size() == 0);  // Intersection with empty set
    REQUIRE((set1 - empty_set).size() ==
            set1.size());                     // Difference with empty set
    REQUIRE((empty_set - set1).size() == 0);  // Empty difference
  }
}

// Test basic operations
TEST_CASE("Test basic operations", "[IndexSetTest]") {
  cse::IndexSet set;

  // Test initial state
  REQUIRE(set.size() == 0);
  REQUIRE(!set.contains(0));

  // Test single insert
  set.insert(5);
  REQUIRE(set.size() == 1);
  REQUIRE(set.contains(5));
  REQUIRE(!set.contains(4));
  REQUIRE(!set.contains(6));

  SECTION("Test single remove") {
    // Test single remove
    set.remove(5);
    REQUIRE(set.size() == 0);
    REQUIRE(!set.contains(5));
  }

  SECTION("Test additional insert") {
    set.insert(7);
    REQUIRE(!set.contains(4));
    REQUIRE(set.contains(5));
    REQUIRE(!set.contains(6));
    REQUIRE(set.contains(7));
  }
}

// Test range operations
TEST_CASE("Test range operations", "[IndexSetTest]") {
  cse::IndexSet set;

  // Insert consecutive numbers
  for (size_t i = 5; i < 10; ++i) {
    set.insert(i);
  }

  // Test range creation
  REQUIRE(set.size() == 5);
  auto range = set.get_containing_range(7);
  REQUIRE(range.has_value());
  REQUIRE(range->start == 5);
  REQUIRE(range->end == 10);

  // Test range queries
  auto next_range = set.get_next_range(4);
  REQUIRE(next_range.has_value());
  REQUIRE(next_range->start == 5);
  REQUIRE(next_range->end == 10);

  auto prev_range = set.get_prev_range(12);
  REQUIRE(prev_range.has_value());
  REQUIRE(prev_range->start == 5);
  REQUIRE(prev_range->end == 10);

  // Test get_all_indices
  auto indices = set.get_all_indices();
  REQUIRE(indices.size() == 5);
  for (size_t i = 0; i < indices.size(); ++i) {
    REQUIRE(indices[i] == i + 5);
  }
}

// Test range merging
TEST_CASE("Test range merging", "[IndexSetTest]") {
  cse::IndexSet set;

  // Insert non-consecutive ranges
  set.insert(1);
  set.insert(2);
  set.insert(4);
  set.insert(5);
  set.insert(3);  // This should merge all into one range

  REQUIRE(set.size() == 5);
  auto range = set.get_containing_range(3);
  REQUIRE(range.has_value());
  REQUIRE(range->start == 1);
  REQUIRE(range->end == 6);
}

// Test range splitting
TEST_CASE("Test range splitting", "[IndexSetTest]") {
  cse::IndexSet set;

  // Create a range and split it
  for (size_t i = 1; i <= 5; ++i) {
    set.insert(i);
  }

  set.remove(3);
  REQUIRE(set.size() == 4);
  REQUIRE(!set.contains(3));

  auto first_range = set.get_containing_range(2);
  auto second_range = set.get_containing_range(4);
  REQUIRE(first_range.has_value());
  REQUIRE(second_range.has_value());
  REQUIRE(first_range->start == 1);
  REQUIRE(first_range->end == 3);
  REQUIRE(second_range->start == 4);
  REQUIRE(second_range->end == 6);
}

TEST_CASE("Test Iterator", "[IndexSetTest]") {
  cse::IndexSet set;

  auto iterator = set.begin();
  auto end = set.end();
  REQUIRE(iterator == end);

  set.insert(2);
  set.insert(4);
  set.insert(5);

  iterator = set.begin();
  end = set.end();
  REQUIRE(iterator != end);
  REQUIRE(*iterator == 2);
  REQUIRE(*iterator == 2);

  iterator++;
  REQUIRE(*iterator == 4);

  ++iterator;
  REQUIRE(*iterator == 5);

  ++iterator;
  REQUIRE(iterator == end);
}

TEST_CASE("Test shifts", "[IndexSetShift]") {
  cse::IndexSet set{};

  set.insert(2);
  set.insert(5);
  set.insert(8);

  std::size_t end = *set.max_index() + 1;

  SECTION("Basic left shift") {
    set.shift_left(2);
    REQUIRE(set == cse::IndexSet{0, 3, 6});
  }

  SECTION("Basic right shift") {
    set.shift_right(2);
    REQUIRE(set == cse::IndexSet{4, 7, 10});
  }

  SECTION("Left shift all indices out") {
    set.shift_left(end);
    REQUIRE(set.get_all_indices().empty());
  }

  SECTION("Right shift indices out") {
    set.shift_right_within(end, 0, end);
    REQUIRE(set.get_all_indices().empty());
  }

  SECTION("Left shift/right shift inverse") {
    cse::IndexSet copy{set};
    copy.shift_right(end);
    copy.shift_left(end);
    REQUIRE(set == copy);
  }

  SECTION("Left shift range of indices") {
    set.shift_left_within(2, 3, end);
    REQUIRE(set == cse::IndexSet{2, 3, 6});
  }

  SECTION("Right shift range of indices") {
    set.shift_right_within(2, 3, end + 2);
    REQUIRE(set == cse::IndexSet{2, 7, 10});
  }

  set.insert_range(3, 7);
  set.insert(10);

  SECTION("Left shift range out") {
    // range to shift in lies within an index range:
    // only shifting out indices 3 and 4, range is from 2 to 6
    set.shift_left_within(2, 3, 5);
    REQUIRE(set == cse::IndexSet{2, 5, 6, 8, 10});
  }

  SECTION("Right shift range out") {
    // range to shift in lies within an index range:
    // only shifting out indices 3 and 4, range is from 2 to 6
    set.shift_right_within(2, 3, 5);
    REQUIRE(set == cse::IndexSet{2, 5, 6, 8, 10});
  }
}

TEST_CASE("Test clamping", "[IndexSetClamp]") {
  cse::IndexSet set{};
  set.insert_range(0, 10);

  SECTION("Check simple range") {
    cse::IndexSet expected{{3, 8}};
    set.clamp(3, 8);
    REQUIRE(set == expected);
  }

  set.clear();
  set.insert_range(0, 2);
  set.insert_range(5, 8);
  set.insert_range(10, 15);

  SECTION("Check multiple ranges") {
    cse::IndexSet expected{5, 6, 7, 10, 11};
    set.clamp(3, 12);
    REQUIRE(set == expected);
  }

  SECTION("Check no-op clamp") {
    cse::IndexSet expected{set};
    set.clamp(*set.min_index(), *set.max_index() + 1);
    REQUIRE(set == expected);
  }
}
