# Used Professor Owen's CMake files from CSE 335 for our project

cmake_minimum_required(VERSION 3.20)
cmake_policy(SET CMP0167 NEW)
project(Group-07)

# Required C++ version
set(CMAKE_CXX_STANDARD 17)

# Add your shared library folder
set(APPLICATION_LIBRARY Team07Library)
add_subdirectory(${APPLICATION_LIBRARY})
include_directories(${APPLICATION_LIBRARY})
include_directories(Team07Application)

# Boost
find_package(Boost REQUIRED COMPONENTS filesystem system)
include_directories(${Boost_INCLUDE_DIRS})

# Source files
set(SOURCE_FILES
        Team07Application/main.cpp
        Team07Application/Simulation.hpp
)

# Executable
add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE ${SOURCE_FILES})

# Link to your library and Boost
target_link_libraries(${PROJECT_NAME}
        ${APPLICATION_LIBRARY}
        Boost::filesystem
        Boost::system
)

# macOS support (optional)
if(APPLE)
    set(RESOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/team07.app/Contents/Resources)
endif()