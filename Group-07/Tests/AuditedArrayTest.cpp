/*
 * AuditedArrayTest.cpp
 */
#include <cassert>
#include <iostream>
#include "gtest/gtest.h"
#include "AuditedArray.hpp"

TEST(AuditedArrayTest, Construct) {
  AuditedArray<int, 5> arr;
}

TEST(AuditedArrayTest, Size) {
  AuditedArray<int, 5> arr;
  ASSERT_EQ(arr.size(), 5);
}

TEST(AuditedArrayTest, Access) {
  AuditedArray<int, 5> arr;
  arr[0] = 1;
  ASSERT_EQ(arr[0], 1);
}

TEST(AuditedArrayTest, Assignment) {
  AuditedArray<int, 5> arr;
  arr[0] = 1;
  AuditedArray<int, 5> arr2 = arr;
  for (int i = 0; i < 5; i++) {
    ASSERT_EQ(arr2[i], arr[i]);
  }
}

TEST(AuditedArrayTest, Fill) {
  AuditedArray<int, 5> arr;
  arr.fill(1);
  for (int i = 0; i < 5; i++) {
    ASSERT_EQ(arr[i], 1);
  }
}

TEST(AuditedArrayTest, Contains) {
  AuditedArray<int, 5> arr;
  arr.fill(1);
  arr[1] = 3;
  ASSERT_TRUE(arr.contains(3));
  ASSERT_FALSE(arr.contains(2));
}

TEST(AuditedArrayTest, IndexOf) {
  AuditedArray<int, 5> arr;
  arr.fill(1);
  arr[1] = 3;
  ASSERT_EQ(arr.indexOf(1), 0);
  ASSERT_EQ(arr.indexOf(2), -1);
}

