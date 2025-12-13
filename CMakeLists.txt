cmake_minimum_required(VERSION 3.14)
project(raylib_cpp_example LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Disable unused stuff
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING  OFF CACHE BOOL "" FORCE)

# raylib (submodule)
add_subdirectory(external/raylib)

# raylib-cpp (submodule, header-only)
add_subdirectory(external/raylib-cpp)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        raylib
        raylib_cpp
)

