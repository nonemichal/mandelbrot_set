# ============================================================
# Core project library
# ============================================================

# Source files for the core library
set(MANDELBROT_CORE_SOURCES app.cpp config.cpp)

# Create static library
add_library(mandelbrot_core STATIC ${MANDELBROT_CORE_SOURCES})

# Expose headers
target_include_directories(mandelbrot_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Apply warning flags
target_compile_options(mandelbrot_core PUBLIC ${PROJECT_WARNING_FLAGS})

# Link third-party libraries
target_link_libraries(mandelbrot_core PUBLIC raylib raylib_cpp toml11::toml11)

# ============================================================
# Main application executable
# ============================================================

# Create executable
add_executable(mandelbrot_set main.cpp)

# Link core logic library
target_link_libraries(mandelbrot_set PRIVATE mandelbrot_core)

# Make the executable appear in the root of build/
set_target_properties(
    mandelbrot_set
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
