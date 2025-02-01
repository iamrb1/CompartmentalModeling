if (!LIBRARY_NAME)
  # If the LIBRARY_NAME is not set, set it to the project name
  set(LIBRARY_NAME ${PROJECT_NAME})
endif ()

message(STATUS "Building Library: ${LIBRARY_NAME}")

# Set the header and source files
set(
    HEADER_FILES
    ${PROJECT_SOURCE_DIR}/include/cse/ActionMap.h
)

set(
    SOURCE_FILES
    ActionMap.cpp
)

# Add the library target
add_library(${LIBRARY_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

# Include the directories
target_include_directories(${LIBRARY_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Set the C++ standard to C++20
target_compile_features(${LIBRARY_NAME} PRIVATE cxx_std_20)

# source_group(
#    TREE ${PROJECT_SOURCE_DIR}/include
#    PREFIX "Header Files"
#    FILES ${HEADER_FILES}
# )