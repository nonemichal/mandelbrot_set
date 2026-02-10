#pragma once

#include <algorithm>
#include <cstddef>

#include "raylib.h"

#include "checkbox.hpp"
#include "config.hpp"
#include "raygui.h"
#include "slider.hpp"

// NOTE: The UI and all related components were implemented using plain raylib,
// because raylib-cpp does not support raygui

// Entire UI section
class UI {
  public:
    explicit UI(const Config &config);
    void Update();

    [[nodiscard]] std::size_t GetFontSize() const;
    [[nodiscard]] float GetBaseIterValue() const;
    [[nodiscard]] float GetBailoutPowerValue() const;
    [[nodiscard]] bool GetFullColorPaletteValue() const;

    static constexpr Color BACKGROUND_COLOR = {25, 25, 25, 255};

  private:
    std::size_t screen_width;
    std::size_t screen_height;
    std::size_t font_size;

    static constexpr std::size_t MIN_FONT_SIZE = 10;
    static constexpr std::size_t MAX_FONT_SIZE = 30;

    // Base iter slider
    float base_iter_val;
    Rectangle base_iter_bounds;
    Slider base_iter_slider;

    // Bailout power slider
    float bailout_power_val;
    Rectangle bailout_power_bounds;
    Slider bailtout_power_slider;

    // Full color palette check box
    bool full_color_palette_val = false;  // Defaultly false
    Rectangle full_color_palette_bounds;
    Checkbox full_color_palette_checkbox;
};
