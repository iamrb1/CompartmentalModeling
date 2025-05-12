message(STATUS "Adding tests target")

set(
    TEST_FILES
    main.cpp
    TestCompartment.cpp
    TestCompartment.h
    TestConnection.cpp
    TestConnection.h
    TestSimulation.cpp
    TestSimulation.cpp
    TestSimulation.h
)

qt_add_executable(tests ${TEST_FILES})
qt_add_qml_module(tests
    URI tests
    SOURCES ${SOURCE_FILES}
)

target_link_libraries(
    tests
    PRIVATE
    Qt6::Test Qt6::Quick Qt6::Charts
    # Link to base libraries for C++ access
    ComponentsLib
    UtilitiesLib
    ApplicationLib
    # Link to plugins for QML access
    ComponentsLibplugin
    UtilitiesLibplugin
    ApplicationLibplugin
)

target_include_directories(
    tests
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src/app
)


