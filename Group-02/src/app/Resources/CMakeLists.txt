qt_add_library(ResourcesLib STATIC)

qt_add_resources(ResourcesLib "resources"
    PREFIX "/"
    FILES
    resources.qrc
)

target_include_directories(ResourcesLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
