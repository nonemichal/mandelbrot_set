cmake_minimum_required(VERSION 3.14)
project(mandelbrot_set LANGUAGES CXX)

# Add custom cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Global project config
include(ProjectConfig)
include(Warnings)
include(DependenciesConfig)

# Third-party libraries
add_subdirectory(external/raylib)
add_subdirectory(external/raylib-cpp)
add_subdirectory(external/toml11)

# Project source
add_subdirectory(src)

# Tests
if(MANDELBROT_BUILD_TESTS)
  include(CTest)
  enable_testing()
  add_subdirectory(tests)
endif()
