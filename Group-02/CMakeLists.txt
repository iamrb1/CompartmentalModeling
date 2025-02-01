# Base CMake file for cseg2 project
cmake_minimum_required(VERSION 3.14)

project(
    cseg2
    VERSION 0.1
    DESCRIPTION "CSE 498 Group 2 Project"
    LANGUAGES CXX
)

set(LIBRARY_NAME cse)
set(EXECUTABLE_NAME ${PROJECT_NAME})

# Only do these if this is the main project, and not if it is included through add_subdirectory
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(CMAKE_CXX_STANDARD 20)

  # -std=c++xx instead of -std=g++xx
  set(CMAKE_CXX_EXTENSIONS OFF)

  # Docs only available if this is the main app
  find_package(Doxygen)
  if (DOXYGEN_FOUND)
    find_package(Doxygen)

    doxygen_add_docs(
        docs
        ${PROJECT_SOURCE_DIR}
        ALL
        COMMENT "Generate HTML documentation"
        CONFIG_FILE ${PROJECT_SOURCE_DIR}/Doxyfile
    )

  else ()
    message(STATUS "Doxygen not found, not adding docs target")
  endif ()

endif ()

# Add the subdirectories (libraries and executables)
add_subdirectory(src/cse)
add_subdirectory(src/app)

# Add the tests if this is the main project
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  add_subdirectory(tests)
endif ()