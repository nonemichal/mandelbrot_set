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
      base_iter_slider(base_iter_bounds, "Base iter", &base_iter_val,
                       Config::RENDER_BASE_ITER_MIN,
                       Config::RENDER_BASE_ITER_MAX),
      bailout_power_val(static_cast<float>(
          config.GetRenderValue(Config::RenderOption::BailoutPower))),
      bailout_power_bounds(
          Rectangle{.x = base_iter_bounds.x,
                    .y = static_cast<float>(screen_height - (font_size * 6)),
                    .width = base_iter_bounds.width,
                    .height = base_iter_bounds.height}),
      bailtout_power_slider(
          bailout_power_bounds, "Bailout power", &bailout_power_val,
          Config::RENDER_BAILOUT_POWER_MIN, Config::RENDER_BAILOUT_POWER_MAX),
      full_color_palette_bounds(
          Rectangle{.x = (base_iter_bounds.x + base_iter_bounds.width) * 1.2F,
                    .y = base_iter_bounds.y,
                    .width = base_iter_bounds.height,
                    .height = base_iter_bounds.height}),
      full_color_palette_checkbox(full_color_palette_bounds,
                                  "Full color palette",
                                  &full_color_palette_val) {
    GuiSetStyle(DEFAULT, TEXT_SIZE, static_cast<int>(font_size));
}

void UI::Update() {
    base_iter_slider.Draw();
    bailtout_power_slider.Draw();
    full_color_palette_checkbox.Draw();
}

std::size_t UI::GetFontSize() const { return font_size; }

float UI::GetBaseIterValue() const { return base_iter_val; }

float UI::GetBailoutPowerValue() const { return bailout_power_val; }

bool UI::GetFullColorPaletteValue() const { return full_color_palette_val; }
