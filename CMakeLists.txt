cmake_minimum_required(VERSION 3.19)
include(npm.cmake)
project(ekx-demo)

add_executable(${PROJECT_NAME}
        src/demo_main.c
        src/3d/sample_3d.c
        src/3d/camera_arcball.c
        src/3d/model_utils.c
        src/sample_text.c
        src/sample_base.c
        src/piko/examples.c
        src/piko/sample_piko.c
        src/sample_audio.c

        src/sim/follow.c
        src/sim/motion.c
        src/sim/sample_sim.c

        src/sample_flash.c
        src/sample_integrations.c
        src/ui/minimal_ui.c
        src/modplay/pocketmod.c
        src/scripting/scripting.c
        src/scripting/js_api.c
)

set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 11)

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
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")
else ()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Oz -flto")
endif ()

if(EK_UITEST)
    add_definitions(-DEK_UITEST)
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/export/uitest")
else()
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build")
endif()
