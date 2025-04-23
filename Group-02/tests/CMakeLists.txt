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

#set(
#    SOURCE_FILES
#    ${PROJECT_SOURCE_DIR}/src/app/Simulation.h
#    ${PROJECT_SOURCE_DIR}/src/app/Simulation.cpp
#    ${PROJECT_SOURCE_DIR}/src/app/Components/Compartment.h
#    ${PROJECT_SOURCE_DIR}/src/app/Components/Compartment.cpp
#    ${PROJECT_SOURCE_DIR}/src/app/Components/Connection.h
#    ${PROJECT_SOURCE_DIR}/src/app/Components/Connection.cpp
#)
#
#set(
#    QML_FILES
#    ${PROJECT_SOURCE_DIR}/src/app/SimulationUI.qml
#    ${PROJECT_SOURCE_DIR}/src/app/Components/CompartmentCanvasUI.qml
#    ${PROJECT_SOURCE_DIR}/src/app/Components/ConnectionUI.qml
#    ${PROJECT_SOURCE_DIR}/src/app/Components/CompartmentUI.qml
#    ${PROJECT_SOURCE_DIR}/src/app/Components/SidebarUI.qml
#    ${PROJECT_SOURCE_DIR}/src/app/Components/ConnectionEditUI.qml
#    ${PROJECT_SOURCE_DIR}/src/app/Components/CompartmentEditUI.qml
#    ${PROJECT_SOURCE_DIR}/src/app/Components/GraphWindowUI.qml
#    ${PROJECT_SOURCE_DIR}/src/app/Components/TimePlotUI.qml
#)

qt_policy(SET QTP0001 NEW)
qt_add_executable(tests ${TEST_FILES})
qt_add_resources(
    SOURCE_FILES
#    ${PROJECT_SOURCE_DIR}/src/app/resources.qrc
)
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
    cseg2Lib
    # Link to plugins for QML access
    ComponentsLibplugin
    UtilitiesLibplugin
    cseg2Libplugin
)

target_include_directories(
    tests
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)


