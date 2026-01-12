#pragma once

#include <mandelbrot_error.hpp>
#include <memory>
#include <string_view>

#include "raylib-cpp.hpp"

#include "config.hpp"

// Class to run the app
class App {
  public:
    // Creates an app instance
    static std::expected<std::unique_ptr<App>, MandelbrotError>
    New(const std::string &title, std::string_view config_file);

    // Delete copy operations
    App(const App &) = delete;
    App &operator=(const App &) = delete;

    // Delete move operations
    App(App &&) noexcept = delete;
    App &operator=(App &&) = delete;

    ~App() = default;

    // Run the app
    void Run();

  private:
    // Private constructor
    explicit App(const std::string &title, const Config &config);

    int fps_;
    raylib::Window window_;
    raylib::Shader shader_;
};
