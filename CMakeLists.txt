cmake_minimum_required(VERSION 3.19)
include(npm.cmake)
project(ekx-demo)

add_executable(${PROJECT_NAME}
        src/demo_main.cpp
        src/3d/sample_3d.cpp
        src/3d/camera_arcball.cpp
        src/3d/model_utils.c
        src/sample_text.cpp
        src/sample_base.cpp
        src/piko/examples.h
        src/piko/examples.cpp
        src/piko/sample_piko.cpp

        src/sim/follow.c
        src/sim/Motion.cpp
        src/sim/Motion.hpp
        src/sim/SampleSim.cpp
        src/sim/SampleSim.hpp

        src/sample_flash.hpp
        src/sample_flash.cpp
        src/sample_integrations.cpp
        src/ui/minimal_ui.c
        src/modplay/pocketmod.c
        src/scripting/scripting.c
        src/scripting/js_api.c
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        C_STANDARD 11
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        )

add_custom_target(${PROJECT_NAME}-assets ALL
        COMMAND npm run dev-assets
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}-assets)

target_include_directories(${PROJECT_NAME} PUBLIC src)

target_link_libraries(${PROJECT_NAME}
        PUBLIC ekx::appbox
        PUBLIC ekx::quickjs
        )

add_definitions("-ffast-math -fno-exceptions -fno-rtti -Wall -Wextra")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
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
