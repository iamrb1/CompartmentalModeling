project(Team07Library)

set(SOURCE_FILES
        AuditedArray.cpp
        AuditedArray.hpp
        DataFileManager.cpp
        DataFileManager.hpp
        OutputLog.cpp
        OutputLog.hpp
        Scheduler.cpp
        Scheduler.h
        Serializer.cpp
        Serializer.hpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})