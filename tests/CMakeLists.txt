# Test source files
set(MANDELBROT_TEST_SOURCES test_main.cpp test_app.cpp test_config.cpp)

# Create executable for unit tests
add_executable(mandelbrot_tests ${MANDELBROT_TEST_SOURCES})

# Link core library
target_link_libraries(mandelbrot_tests PRIVATE mandelbrot_core)

# Include doctest headers
target_include_directories(
  mandelbrot_tests PRIVATE ${CMAKE_SOURCE_DIR}/external/doctest/doctest
                           ${CMAKE_SOURCE_DIR}/src)

# Register test with CTest
add_test(NAME mandelbrot_tests COMMAND mandelbrot_tests)
