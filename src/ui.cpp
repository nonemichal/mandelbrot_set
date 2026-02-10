#include "ui.hpp"

// 140, 20, 600, 30
//  = {140, 60, 600, 30}

UI::UI(const Config &config)
    : screen_width(static_cast<std::size_t>(
          config.GetWindowValue(Config::WindowOption::Width))),
      screen_height(static_cast<std::size_t>(
          config.GetWindowValue(Config::WindowOption::Height))),
      font_size(static_cast<std::size_t>(
          std::clamp(screen_height / 60, MIN_FONT_SIZE, MAX_FONT_SIZE))),
      base_iter_val(static_cast<float>(
          config.GetRenderValue(Config::RenderOption::BaseIter))),
      base_iter_bounds(
          Rectangle{.x = static_cast<float>(screen_width / 8),
                    .y = static_cast<float>(screen_height - (font_size * 3)),
                    .width = static_cast<float>(screen_width / 2),
                    .height = static_cast<float>(font_size) * 1.5F}),
      slider_base_iter(base_iter_bounds, "Base iter", &base_iter_val,
                       Config::RENDER_BASE_ITER_MIN,
                       Config::RENDER_BASE_ITER_MAX),
      bailout_power_val(static_cast<float>(
          config.GetRenderValue(Config::RenderOption::BailoutPower))),
      bailout_power_bounds(
          Rectangle{.x = base_iter_bounds.x,
                    .y = static_cast<float>(screen_height - (font_size * 6)),
                    .width = base_iter_bounds.width,
                    .height = base_iter_bounds.height}),
      slider_bailtout_power(
          bailout_power_bounds, "Bailout power", &bailout_power_val,
          Config::RENDER_BAILOUT_POWER_MIN, Config::RENDER_BAILOUT_POWER_MAX) {
    GuiSetStyle(DEFAULT, TEXT_SIZE, static_cast<int>(font_size));
}

void UI::Update() {
    // DrawRectangleRec(BASE_ITER_BOUNDS, Fade(DARKGRAY, 0.5f));
    slider_base_iter.Draw();
    // DrawRectangleRec(BAILOUT_POWER_BOUNDS, Fade(DARKGRAY, 0.5f));
    slider_bailtout_power.Draw();
}

float UI::GetBaseIterVal() const { return slider_base_iter.GetValue(); }

float UI::GetBailoutPowerVal() const {
    return slider_bailtout_power.GetValue();
}

std::size_t UI::GetFontSize() const { return font_size; }
