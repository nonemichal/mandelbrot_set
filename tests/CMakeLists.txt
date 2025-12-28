# Test source files
set(MANDELBROT_TEST_SOURCES test_main.cpp)

# Create executable for unit tests
add_executable(mandelbrot_tests ${MANDELBROT_TEST_SOURCES})

# Link test library
target_link_libraries(mandelbrot_tests PRIVATE mandelbrot_core)

# Apply warning flags
target_compile_options(mandelbrot_tests PRIVATE ${PROJECT_WARNING_FLAGS})

# Include project headers
target_include_directories(
  mandelbrot_tests PRIVATE ${CMAKE_SOURCE_DIR}/src
                           ${CMAKE_SOURCE_DIR}/external/doctest)

# Register tests with CTest
include(CTest)
add_test(NAME mandelbrot_tests COMMAND mandelbrot_tests)
