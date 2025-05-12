qt_add_library(UtilitiesLib STATIC)

set_target_properties(UtilitiesLib PROPERTIES AUTOMOC ON)
target_link_libraries(UtilitiesLib PRIVATE Qt6::Quick)

set(
    MODULE_QML_FILES
    ThemeManager.qml
    LightTheme.qml
    DarkTheme.qml
    Constants.qml
    MessageModule.qml
)

set(
    SINGLETON_QMLS
    ThemeManager.qml
    LightTheme.qml
    DarkTheme.qml
    Constants.qml
)


set_source_files_properties(
    ${SINGLETON_QMLS} PROPERTIES QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(
    UtilitiesLib
    URI Utilities
    QML_FILES ${MODULE_QML_FILES}
)
