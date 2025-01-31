cmake_minimum_required(VERSION 3.20)
project(team07)
set(APPLICATION_LIBRARY Team07Library)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(${APPLICATION_LIBRARY})
include_directories(${APPLICATION_LIBRARY})

# Define a variable containing a list of source files for the project
set(SOURCE_FILES main.cpp

)

add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE ${SOURCE_FILES})

add_subdirectory(Tests)

if(APPLE)
    # When building for MacOS, also copy files into the bundle resources
    set(RESOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/team07.app/Contents/Resources)
endif()

