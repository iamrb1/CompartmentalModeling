qt_add_library(ComponentsLib STATIC)

set_target_properties(ComponentsLib PROPERTIES AUTOMOC ON)
target_link_libraries(ComponentsLib PRIVATE Qt6::Quick Qt6::Charts ResourcesLib)
target_link_libraries(ComponentsLib PUBLIC exprtk::exprtk)

set(
    MODULE_QML_FILES
    CompartmentUI.qml
    ConnectionUI.qml
    SidebarUI.qml
    TimePlotUI.qml
    ConnectionEditUI.qml
    CompartmentEditUI.qml
    CompartmentCanvasUI.qml
    GraphWindowUI.qml
)

set(
    MODULE_SOURCES
    Compartment.h
    Compartment.cpp
    Connection.h
    Connection.cpp
)

qt_add_qml_module(
    ComponentsLib
    URI Components
    QML_FILES ${MODULE_QML_FILES}
    SOURCES ${MODULE_SOURCES}
)
