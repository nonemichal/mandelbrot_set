#pragma once

#include "raylib-cpp.hpp"

#include "drawing_scope.hpp"

// Class to run the app
class App {
  public:
    App(int width, int height, const std::string &title)
        : window_(width, height, title) {}

    void Run(int fps) {
        window_.SetTargetFPS(fps);

        // Main loop
        // Detect window close button or ESC key
        while (!window_.ShouldClose()) {
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
};
