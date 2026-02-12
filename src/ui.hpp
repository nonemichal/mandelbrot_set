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

    static constexpr Color BACKGROUND_COLOR = {25, 25, 25, 255};

    [[nodiscard]] std::size_t GetFontSize() const;
    [[nodiscard]] float GetBaseIterValue() const;
    [[nodiscard]] float GetBailoutPowerValue() const;
    [[nodiscard]] float GetColorDetailValue() const;
    [[nodiscard]] bool GetUseSmoothingValue() const;
    [[nodiscard]] bool GetUseColorBoostValue() const;

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

    // Color cycling speed value
    float color_detail_value;
    Rectangle color_detail_bounds;
    Slider color_detail_slider;

    // Bailout power slider
    float bailout_power_val;
    Rectangle bailout_power_bounds;
    Slider bailtout_power_slider;

    // Iteration smoothing check box
    bool use_smoothing_val = false;
    Rectangle use_smoothing_bounds;
    Checkbox use_smoothing_checkbox;

    // Continuous palette check box
    bool use_color_boost_val = false;
    Rectangle use_color_boost_bounds;
    Checkbox use_color_boost_checkbox;

    [[nodiscard]] std::size_t CalculateSliderWidth() const;
    [[nodiscard]] std::size_t CalculateSliderHeight() const;
    [[nodiscard]] std::size_t CalculateCheckboxWidth() const;
    [[nodiscard]] std::size_t CalculateCheckboxHeight() const;
    [[nodiscard]] std::size_t CalculateVerticalMargin() const;
};
