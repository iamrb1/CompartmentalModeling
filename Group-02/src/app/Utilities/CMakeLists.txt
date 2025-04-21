qt_add_library(UtilitiesLib STATIC)

set_target_properties(UtilitiesLib PROPERTIES AUTOMOC ON)
target_link_libraries(UtilitiesLib PRIVATE Qt6::Quick)

set(
    MODULE_QML_FILES
    ThemeManager.qml
    LightTheme.qml
    DarkTheme.qml
    Constants.qml
)

set_source_files_properties(
    ${MODULE_QML_FILES} PROPERTIES QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(
    UtilitiesLib
    URI Utilities
    QML_FILES ${MODULE_QML_FILES}
)
