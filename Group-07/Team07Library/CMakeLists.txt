project(Team07Library)

set(SOURCE_FILES
        AuditedArray.cpp
        AuditedArray.hpp
        DataFileManager.cpp
        DataFileManager.h
        OutputLog.cpp
        OutputLog.hpp
        Scheduler.cpp
        Scheduler.hpp
        Serializer.cpp
        Serializer.hpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})