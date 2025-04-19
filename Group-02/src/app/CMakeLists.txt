message(STATUS "Building Executable: ${EXECUTABLE_NAME}")

# Set the source files
set(
    SOURCE_FILES
    main.cpp
    Simulation.cpp
    Simulation.h
)

set(
    MAIN_QML_FILES
    SimulationUI.qml
)

set(
    QML_MODULES
    Utilities
    Components
)

# Qt6 setup
find_package(Qt6 REQUIRED COMPONENTS Quick)
qt_standard_project_setup(REQUIRES 6.8)
# Add resources
qt_add_resources(
    SOURCE_FILES
    resources.qrc
)
# Add the executable target
qt_add_executable(
    ${EXECUTABLE_NAME}
)
# Add Main QML files
qt_add_qml_module(
    ${EXECUTABLE_NAME}
    URI ${EXECUTABLE_NAME}
    QML_FILES ${MAIN_QML_FILES}
    SOURCES ${SOURCE_FILES}
)

# Set the C++ standard to C++20
target_compile_features(${EXECUTABLE_NAME} PRIVATE cxx_std_20)
# Precompiled header
#target_precompile_headers(${EXECUTABLE_NAME} PRIVATE pch.h)

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

# Link libraries
target_link_libraries(${EXECUTABLE_NAME} PRIVATE Qt6::Quick)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${LIBRARY_NAME})

# Link plugins
foreach (MODULE ${QML_MODULES})
    message(STATUS "Adding QML module: ${MODULE}")
    add_subdirectory(${MODULE})
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${MODULE}Libplugin)
endforeach ()

include(GNUInstallDirs)
install(
    TARGETS ${EXECUTABLE_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
