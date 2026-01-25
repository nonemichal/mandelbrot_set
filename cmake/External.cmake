include(FetchContent)

# raylib
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
    SOURCE_DIR
    ${PROJECT_SOURCE_DIR}/external/raylib
)

# raylib-cpp
FetchContent_Declare(
    raylib-cpp
    GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
    GIT_TAG v5.5.0
    SOURCE_DIR
    ${PROJECT_SOURCE_DIR}/external/raylib-cpp
)

# toml11
FetchContent_Declare(
    toml11
    GIT_REPOSITORY https://github.com/ToruNiina/toml11.git
    GIT_TAG v4.4.0
    SOURCE_DIR
    ${PROJECT_SOURCE_DIR}/external/toml11
)

# doctest
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.4.12
    SOURCE_DIR
    ${PROJECT_SOURCE_DIR}/external/doctest
)

FetchContent_MakeAvailable(raylib raylib-cpp toml11 doctest)
