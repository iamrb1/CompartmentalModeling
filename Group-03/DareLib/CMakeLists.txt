project(DareLib)
set(SOURCE_FILES
        pch.h
        Circle.cpp
        Circle.h
        DataTracker.cpp
        DataTracker.h
        FixedPoint.cpp
        FixedPoint.h
        FunctionSet.h
        FunctionSet.cpp
        Sector.cpp
        Sector.h
        Surface.cpp
        Surface.h
)
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)