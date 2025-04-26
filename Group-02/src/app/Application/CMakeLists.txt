qt_add_library(ApplicationLib STATIC)

set_target_properties(ApplicationLib PROPERTIES AUTOMOC ON)
target_link_libraries(ApplicationLib PRIVATE Qt6::Quick Qt6::Widgets exprtk::exprtk ResourcesLib)
target_link_libraries(ApplicationLib PUBLIC exprtk::exprtk)

set(
    MODULE_QML_FILES
    SimulationUI.qml
)

set(
    MODULE_SOURCES
    Simulation.cpp
    Simulation.h
)

qt_add_qml_module(
    ApplicationLib
    URI Application
    QML_FILES ${MODULE_QML_FILES}
    SOURCES ${MODULE_SOURCES}
)
