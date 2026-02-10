#pragma once

#include <string>
#include <string_view>

#include "raylib.h"

#include "raygui.h"

// UI checkbox component
struct Checkbox {
    Rectangle bounds;
    std::string text;
    bool *value_ptr;

    Checkbox(Rectangle rec, std::string_view txt, bool *val_ptr)
        : bounds(rec), text(txt), value_ptr(val_ptr) {}

    void Draw() const { GuiCheckBox(bounds, text.c_str(), value_ptr); }

    [[nodiscard]] Rectangle GetBounds() const { return bounds; }
    [[nodiscard]] bool GetValue() const { return *value_ptr; }
};
