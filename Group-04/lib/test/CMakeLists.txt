cmake_minimum_required(VERSION 3.10)
project(IndexSetTests)

# Find GTest package
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add test executable
add_executable(index_set_test IndexSetTest.cpp)
target_link_libraries(index_set_test ${GTEST_LIBRARIES} pthread) 