cmake_minimum_required(VERSION 3.20)

project(HZLAtomSimulation
    VERSION 0.1.0
    DESCRIPTION "A modern C++ atom simulation engine"
    LANGUAGES CXX
)

add_executable(HZLAtomSimulation
    src/hzl/core/Application.cpp
    src/hzl/core/Application.h
    src/hzl/core/Timestep.h
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
