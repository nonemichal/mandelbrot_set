cmake_minimum_required(VERSION 3.14)
project(mandelbrot_set LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# raylib
set(BUILD_EXAMPLES
    OFF
    CACHE BOOL "" FORCE)
set(BUILD_GAMES
    OFF
    CACHE BOOL "" FORCE)
set(BUILD_TESTING
    OFF
    CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS
    OFF
    CACHE BOOL "" FORCE)
add_subdirectory(external/raylib)

# raylib-cpp
add_subdirectory(external/raylib-cpp)

# toml11
add_subdirectory(external/toml11)

# Executable
set(SOURCE_FILES src/main.cpp src/config.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE raylib raylib_cpp toml11::toml11)

# Define PROJECT_ROOT_DIR macro
target_compile_definitions(${PROJECT_NAME}
                           PRIVATE PROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}")

# Warnings
set(COMPILE_FLAGS
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wsign-conversion
    -Wno-deprecated-literal-operator)
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_FLAGS})
