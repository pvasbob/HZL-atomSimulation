cmake_minimum_required(VERSION 3.20)

project(HZLAtomSimulation
    VERSION 0.1.0
    DESCRIPTION "A modern C++ atom simulation engine"
    LANGUAGES CXX
)

include(FetchContent)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)

add_executable(HZLAtomSimulation
    src/hzl/core/Application.cpp
    src/hzl/core/Application.h
    src/hzl/core/Timestep.h
    src/hzl/platform/Window.cpp
    src/hzl/platform/Window.h
    src/main.cpp
)

target_include_directories(HZLAtomSimulation
    PRIVATE
        src
)

target_compile_features(HZLAtomSimulation
    PRIVATE
        cxx_std_20
)

target_link_libraries(HZLAtomSimulation
    PRIVATE
        glfw
)

if(MSVC)
    target_compile_options(HZLAtomSimulation
        PRIVATE
            /W4
            /permissive-
    )
else()
    target_compile_options(HZLAtomSimulation
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )
endif()
