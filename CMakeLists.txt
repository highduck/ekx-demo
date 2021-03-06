cmake_minimum_required(VERSION 3.13)
include(npm.cmake)
project(ekx-demo)

add_executable(${PROJECT_NAME}
        src/demo_main.hpp
        src/demo_main.cpp
        src/3d/sample_3d.cpp
        src/3d/camera_arcball.cpp
        src/sample_text.cpp
        src/sample_base.cpp
        src/piko/piko.h
        src/piko/examples.h
        src/piko/examples.cpp
        src/piko/sample_piko.cpp

        src/sim/Motion.cpp
        src/sim/Motion.hpp
        src/sim/SampleSim.cpp
        src/sim/SampleSim.hpp

        src/sample_flash.hpp
        src/sample_flash.cpp
        src/sample_integrations.cpp
        src/ui/minimal.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        )

#target_compile_definitions(${PROJECT_NAME} PUBLIC
#        )

target_include_directories(${PROJECT_NAME} PUBLIC src)

target_link_libraries(${PROJECT_NAME}
        PUBLIC ekx::appbox
        )

add_definitions("-ffast-math -fno-exceptions -fno-rtti -Wall -Wextra")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
    #    add_definitions(-DNDEBUG)
else ()
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Oz -ffast-math -fno-exceptions -fno-rtti -Wall -Wextra")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Oz -flto")
    #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-dead_strip -Wl,-dead_strip_dylibs")
    #    add_definitions(-DNDEBUG)
endif ()

if(EK_UITEST)
    add_definitions(-DEK_UITEST)
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/export/uitest")
else()
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build")
endif()