cmake_minimum_required(VERSION 3.10)
project(IndexSet)

# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
enable_testing()

# Add test directory
add_subdirectory(lib/test) 