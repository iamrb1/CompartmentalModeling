# Base CMake file for cseg2 project

# Set the minimum version of CMake that can be used to build the project
cmake_minimum_required(VERSION 3.14)

set(PROJECT_VERSION 0.1.0)
project(
    cseg2
    VERSION ${PROJECT_VERSION}
    DESCRIPTION "CSE 498 Group 2 Project"
    LANGUAGES CXX
)

set(LIBRARY_NAME cse)
set(EXECUTABLE_NAME ${PROJECT_NAME})
set(BUILD_SHARED_LIBS OFF)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)

  # -std=c++xx instead of -std=g++xx
  set(CMAKE_CXX_EXTENSIONS OFF)

  # Set build-type-specific optimization flags
  if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od /Zi")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /DNDEBUG")
  endif ()

  message(STATUS "CMake build type: ${CMAKE_BUILD_TYPE}")
  message(STATUS "CMake compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
  message(STATUS "CMake compiler flags: ${CMAKE_CXX_FLAGS}")

endif ()

# Qt setup
find_package(Qt6 REQUIRED COMPONENTS Quick Charts Test Widgets)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Add extern lib (expertk)
add_subdirectory(extern/exprtk)

# Add the subdirectories (libraries and executables)
add_subdirectory(src/cse)
add_subdirectory(src/app)

# Optional: Make tests optional with a CMake option
option(BUILD_TESTS "Build the tests" ON)
if (BUILD_TESTS)
  add_subdirectory(tests)
endif ()