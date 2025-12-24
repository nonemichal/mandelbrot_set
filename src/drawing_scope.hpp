#pragma once

#include "raylib-cpp.hpp"

// Class using RAII to determine drawing scope
// It has to be used whenever you want to draw something on the window
class DrawingScope {
  public:
    explicit DrawingScope(raylib::Window &window) : window_(window) {
        window_.BeginDrawing();
    }
    ~DrawingScope() { window_.EndDrawing(); }

    // Delete copy operations
    DrawingScope(const DrawingScope &) = delete;
    DrawingScope &operator=(const DrawingScope &) = delete;

    // Delete move operations
    DrawingScope(DrawingScope &&) = delete;
    DrawingScope &operator=(DrawingScope &&) = delete;

  private:
    raylib::Window &window_;
};
