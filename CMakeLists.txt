cmake_minimum_required(VERSION 3.14)
project(mandelbrot_set LANGUAGES CXX)

# PROJECT_ROOT_PATH available to all targets
add_compile_definitions(PROJECT_ROOT_PATH="${CMAKE_SOURCE_DIR}")

# Add custom cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(ProjectConfig)
include(Warnings)
include(RaylibConfig)
include(External)

# Project source
add_subdirectory(src)

# Tests
option(MANDELBROT_BUILD_TESTS "Build Mandelbrot unit tests" OFF)
if(MANDELBROT_BUILD_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()
