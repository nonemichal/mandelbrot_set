#pragma once

#include <string_view>

#include "raylib-cpp.hpp"

#include "config.hpp"

// Class to run the app
class App {
  public:
    explicit App(const Config &config, std::string_view title);
    ~App() = default;

    // Delete copy operations
    App(const App &) = delete;
    App &operator=(const App &) = delete;

    // Delete move operations
    App(App &&) = delete;
    App &operator=(App &&) = delete;

    void Run();

  private:
    raylib::Window window_;
    int fps_;
    struct {
        std::string_view vertex;
        std::string_view fragment;
    } shaders_;
};
