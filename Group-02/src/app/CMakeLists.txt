message(STATUS "Building Executable: ${EXECUTABLE_NAME}")

# Set the source files
set(
    SOURCE_FILES
    main.cpp
)

# Add the executable target
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

# Set the C++ standard to C++20
target_compile_features(${EXECUTABLE_NAME} PRIVATE cxx_std_20)

# Link the executable to the CSE library
target_link_libraries(${EXECUTABLE_NAME} PRIVATE cse)

# Set the output directory of the executable
#set_target_properties(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
