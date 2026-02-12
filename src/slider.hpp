#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <string>
#include <string_view>

#include "raylib.h"

#include "raygui.h"

// UI slider component
struct Slider {
    enum class Type : uint8_t { INT, FLOAT };

    Rectangle bounds;
    std::string text_title;
    Type type;
    float *value_ptr;
    float min_value;
    float max_value;

    Slider(Rectangle rec, std::string_view txt, Type type, float *val_ptr,
           float min_val, float max_val)
        : bounds(rec), text_title(txt), type(type), value_ptr(val_ptr),
          min_value(min_val), max_value(max_val) {}

    void Draw() const {
        // Draw the slider
        switch (type) {
        case Type::INT:
            *value_ptr = std::round(*value_ptr);
            GuiSlider(bounds, text_title.c_str(),
                      TextFormat("%d", static_cast<int>(*value_ptr)), value_ptr,
                      min_value, max_value);
            break;
        case Type::FLOAT:
            GuiSlider(bounds, text_title.c_str(),
                      TextFormat("%.1f", *value_ptr), value_ptr, min_value,
                      max_value);
        }
    }

    [[nodiscard]] Rectangle GetBounds() const { return bounds; }
    [[nodiscard]] float GetValue() const { return *value_ptr; }
};
