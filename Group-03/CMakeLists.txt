# Declare the minimum required CMake version
cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 17)

# Name the project
project(DareToRun)
set(APPLICATION_LIBRARY DareLib)


add_subdirectory(${APPLICATION_LIBRARY})
include_directories(${APPLICATION_LIBRARY})

# Define a variable containing a list of source files for the project
set(SOURCE_FILES pch.h main.cpp
        Tests/DataTrackerTests/Makefile
        Tests/FunctionSetTest/Makefile
        Tests/CircleTests/Makefile)

# Define the build target for the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Link required libraries to the executable
target_link_libraries(${PROJECT_NAME} ${APPLICATION_LIBRARY})

target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)


## Test

add_subdirectory(third-party/Catch)

enable_testing()
# DataTracker Tests
file(GLOB_RECURSE TEST_FILES Tests/DataTrackerTests/*.cpp)

add_executable(DataTrackerTests ${TEST_FILES})

target_link_libraries(DataTrackerTests PRIVATE Catch2::Catch2 ${APPLICATION_LIBRARY})

add_test(NAME DataTrackerTests COMMAND DataTrackerTests)
# FunctionSet Tests
file(GLOB_RECURSE TEST_FILES Tests/FunctionSetTest/*.cpp)

add_executable(FunctionSetTest ${TEST_FILES})

target_link_libraries(FunctionSetTest PRIVATE Catch2::Catch2 ${APPLICATION_LIBRARY})

add_test(NAME FunctionSetTest COMMAND FunctionSetTest)

# Circle Tests
file(GLOB_RECURSE TEST_FILES Tests/CircleTests/*.cpp)

add_executable(CircleTests ${TEST_FILES})

target_link_libraries(CircleTests PRIVATE Catch2::Catch2 ${APPLICATION_LIBRARY})

add_test(NAME CircleTests COMMAND CircleTests)