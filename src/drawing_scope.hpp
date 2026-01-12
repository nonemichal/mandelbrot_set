#pragma once

#include "raylib-cpp.hpp"

// Class to determine drawing scope
// NOTE: It has to be used whenever something on the window will be drawn
class DrawingScope {
  public:
    // Takes mutable reference because it controls drawing state
    explicit DrawingScope(raylib::Window &draw_target) : window_(draw_target) {
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
