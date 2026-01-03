#pragma once

#include <mandelbrot_error.hpp>
#include <string_view>

#include "raylib-cpp.hpp"

#include "config.hpp"

// Class to run the app
class App {
  public:
    // Returns the only one instance of the app
    static std::expected<App *, MandelbrotError>
    New(const std::string &title, std::string_view config_file);

    // Delete copy operations
    App(const App &) = delete;
    App &operator=(const App &) = delete;

    // Delete move operations
    App(App &&) = delete;
    App &operator=(App &&) = delete;

    // Getters
    [[nodiscard]] const Config &GetConfig() const;
    [[nodiscard]] int GetFps() const;
    [[nodiscard]] const raylib::Window &GetWindow() const;

    // Run the app
    void Run();

  private:
    // Private constructor and destructor
    explicit App(const std::string &title, const Config &config);
    ~App() = default;

    int fps_;
    raylib::Window window_;
    raylib::Shader shader_;

#ifdef MANDELBROT_BUILD_TESTS
    // Only for tests: allow resetting the singleton
    static void ResetSingletonForTests() { initialized = false; }
#endif
};
