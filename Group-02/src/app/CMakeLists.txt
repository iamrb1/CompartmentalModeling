message(STATUS "Building Executable: ${EXECUTABLE_NAME}")

# Set the QML modules
set(
    QML_MODULES
    Utilities
    Components
    Application
)

# Set the WIN32 flag to create a Windows GUI application (no console)
if(WIN32)
  set(CMAKE_WIN32_EXECUTABLE ON)
endif()

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
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Release build
if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
  # Add windeployqt command
  include(InstallRequiredSystemLibraries)
  get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
  get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
  find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

  add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
      --qmldir "${CMAKE_SOURCE_DIR}"
      --plugindir "$<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/plugins"
      --no-translations
      --no-opengl-sw
      "$<TARGET_FILE:${EXECUTABLE_NAME}>"
      COMMENT "Running windeployqt..."
  )
elseif(APPLE AND CMAKE_BUILD_TYPE STREQUAL "Release")
  # Add macdeployqt command for macOS
  get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
  get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
  find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")

  add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
      COMMAND "${MACDEPLOYQT_EXECUTABLE}"
      "$<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../.."
      -qmldir="${CMAKE_SOURCE_DIR}"
      COMMENT "Running macdeployqt..."
  )
endif()
