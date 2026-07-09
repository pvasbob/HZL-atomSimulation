cmake_minimum_required(VERSION 3.20)

project(HZLAtomSimulation
    VERSION 0.1.0
    DESCRIPTION "A modern C++ atom simulation engine"
    LANGUAGES C CXX
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

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v2.0.8
)

FetchContent_MakeAvailable(glfw)
FetchContent_Populate(glad)

set(GLAD_SOURCES_DIR ${glad_SOURCE_DIR})
include(${glad_SOURCE_DIR}/cmake/GladConfig.cmake)

glad_add_library(glad_gl_core_33
    STATIC
    REPRODUCIBLE
    API gl:core=3.3
)

find_package(OpenGL REQUIRED)

add_executable(HZLAtomSimulation
    src/hzl/core/Application.cpp
    src/hzl/core/Application.h
    src/hzl/core/Timestep.h
    src/hzl/platform/Window.cpp
    src/hzl/platform/Window.h
    src/hzl/renderer/Renderer.cpp
    src/hzl/renderer/Renderer.h
    src/hzl/renderer/Shader.cpp
    src/hzl/renderer/Shader.h
    src/hzl/renderer/VertexArray.cpp
    src/hzl/renderer/VertexArray.h
    src/hzl/renderer/VertexBuffer.cpp
    src/hzl/renderer/VertexBuffer.h
    src/main.cpp
)

add_dependencies(HZLAtomSimulation
    glad_gl_core_33
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
        glad_gl_core_33
        OpenGL::GL
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
