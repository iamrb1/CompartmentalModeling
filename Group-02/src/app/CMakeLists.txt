message(STATUS "Building Executable: ${EXECUTABLE_NAME}")

# Set the QML modules
set(
    QML_MODULES
    Utilities
    Components
    Application
)

# Add the executable target
qt_add_executable(
    ${EXECUTABLE_NAME}
    main.cpp
    Resources/resources.qrc
)

# Set the C++ standard to C++20
target_compile_features(${EXECUTABLE_NAME} PRIVATE cxx_std_20)

# Link libraries
add_subdirectory(Resources)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE Qt6::Quick Qt6::Widgets ResourcesLib)

# Set the executable properties
set_target_properties(${EXECUTABLE_NAME} PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE

    # If debug build, put binaries in bin/debug
    # If release build, put binaries in bin/release
    #    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/
)

# Link plugins
foreach (MODULE ${QML_MODULES})
  message(STATUS "Adding QML module: ${MODULE}")
  add_subdirectory(${MODULE})
  target_include_directories(${EXECUTABLE_NAME} PRIVATE ${MODULE})
  target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${MODULE}Libplugin)
endforeach ()

include(GNUInstallDirs)
install(
    TARGETS ${EXECUTABLE_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
