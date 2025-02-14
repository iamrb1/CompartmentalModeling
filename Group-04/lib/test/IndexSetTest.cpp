#include "../IndexSet.hpp"
#include <gtest/gtest.h>

TEST(IndexSetTest, AppendAt) {
    cse::IndexSet set1;
    set1.insertRange(0, 3);  // [0,3)
    
    cse::IndexSet set2;
    set2.insertRange(0, 4);  // [0,4)
    
    // Append set2 starting at index 5
    set1.appendAt(set2, 5);
    
    // Expected result: set1 should now contain [0,3) and [5,9)
    EXPECT_TRUE(set1.contains(0));
    EXPECT_TRUE(set1.contains(1));
    EXPECT_TRUE(set1.contains(2));
    EXPECT_FALSE(set1.contains(3));
    EXPECT_FALSE(set1.contains(4));
    EXPECT_TRUE(set1.contains(5));
    EXPECT_TRUE(set1.contains(6));
    EXPECT_TRUE(set1.contains(7));
    EXPECT_TRUE(set1.contains(8));
    EXPECT_FALSE(set1.contains(9));
    
    // Test merging case
    cse::IndexSet set3;
    set3.insertRange(0, 2);  // [0,2)
    set1.appendAt(set3, 3);  // Should merge with [0,3)
    
    // Expected: [0,5), [5,9)
    EXPECT_TRUE(set1.contains(3));
    EXPECT_TRUE(set1.contains(4));
    EXPECT_TRUE(set1.contains(5));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 