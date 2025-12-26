#pragma once

#include <string_view>

#include "raylib-cpp.hpp"

#include "config.hpp"
#include "drawing_scope.hpp"

// Class to run the app
class App {
  public:
    explicit App(const Config &config, std::string_view title)
        : window_(config.GetWindowValue(Config::WindowOption::Width),
                  config.GetWindowValue(Config::WindowOption::Height),
                  title.data()),
          fps(config.GetWindowValue(Config::WindowOption::Fps)) {}
    ~App() = default;

    // Delete copy operations
    App(const App &) = delete;
    App &operator=(const App &) = delete;

    // Delete move operations
    App(App &&) = delete;
    App &operator=(App &&) = delete;

    void Run() {
        window_.SetTargetFPS(fps);

        // Main loop
        while (
            !window_.ShouldClose()) {  // Detect window close button or ESC key
            // Update

            // Draw
            {
                DrawingScope drawing_scope(window_);
                window_.ClearBackground(RAYWHITE);
            }
        }
    }

  private:
    raylib::Window window_;
    int fps;
};
