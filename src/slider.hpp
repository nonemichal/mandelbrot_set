#pragma once

#include <algorithm>
#include <string>
#include <string_view>

#include "raylib.h"

#include "raygui.h"

// UI slider component
struct Slider {
    Rectangle bounds;
    std::string text;
    float *value_ptr;
    float min_value;
    float max_value;

    Slider(Rectangle rec, std::string_view txt, float *val_ptr, float min_val,
           float max_val)
        : bounds(rec), text(txt), value_ptr(val_ptr), min_value(min_val),
          max_value(max_val) {}

    void Draw() const {
        // Draw the slider
        GuiSlider(bounds, text.c_str(),
                  TextFormat("%i", static_cast<int>(*value_ptr)), value_ptr,
                  min_value, max_value);
    }

    [[nodiscard]] Rectangle GetBounds() const { return bounds; }
    [[nodiscard]] float GetValue() const { return *value_ptr; }
};
