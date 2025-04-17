qt_add_library(ComponentsLib STATIC)

set_target_properties(ComponentsLib PROPERTIES AUTOMOC ON)
target_link_libraries(ComponentsLib PRIVATE Qt6::Quick)

set (
    MODULE_QML_FILES
    Compartment.qml
    Connection.qml
)

qt_add_qml_module(
    ComponentsLib
    URI Components
    QML_FILES ${MODULE_QML_FILES}
)
