#pragma once

#include <algorithm>
#include <cstddef>

#include "raylib.h"

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

    [[nodiscard]] float GetBaseIterVal() const;
    [[nodiscard]] float GetBailoutPowerVal() const;
    [[nodiscard]] std::size_t GetFontSize() const;

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
    Slider slider_base_iter;

    // Bailout power slider
    float bailout_power_val;
    Rectangle bailout_power_bounds;
    Slider slider_bailtout_power;
};
