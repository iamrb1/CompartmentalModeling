project(tests)

message(STATUS "Building Tests: ${PROJECT_NAME}")

set(
    TEST_FILES
    gtest_main.cpp
    FailSafeTest.cpp
        ../include/cse/StateGridPosition.h
)

# Get google test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Include Google Test directories
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Adding the Google Test library (using submodules)
# add_subdirectory(../extern/googletest ../extern/googletest/googletest/include EXCLUDE_FROM_ALL)

# Adding the Test target
add_executable(${PROJECT_NAME} ${TEST_FILES})

# Set the C++ standard to C++20
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# Linking project with cse library
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRARY_NAME})

# Linking project with Google Test libraries
target_link_libraries(${PROJECT_NAME} PRIVATE gtest)

# Enable testing and add tests
include(GoogleTest)
gtest_add_tests(
    TARGET ${PROJECT_NAME}
    SOURCES ${TEST_FILES}
)