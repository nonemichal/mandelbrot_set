#include "ui.hpp"

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
                    .y = static_cast<float>(screen_height) -
                         (static_cast<float>(CalculateSliderHeight()) * 4) -
                         static_cast<float>(CalculateVerticalMargin()),
                    .width = static_cast<float>(CalculateSliderWidth()),
                    .height = static_cast<float>(CalculateSliderHeight())}),
      base_iter_slider(base_iter_bounds, "Base iter", &base_iter_val,
                       Config::RENDER_BASE_ITER_MIN,
                       Config::RENDER_BASE_ITER_MAX),
      color_detail_value(1.0F),
      color_detail_bounds(
          Rectangle{.x = static_cast<float>(screen_width / 8),
                    .y = static_cast<float>(screen_height) -
                         (static_cast<float>(CalculateSliderHeight()) * 2) -
                         static_cast<float>(CalculateVerticalMargin()),
                    .width = static_cast<float>(CalculateSliderWidth()),
                    .height = static_cast<float>(CalculateSliderHeight())}),
      color_detail_slider(color_detail_bounds, "Color detail",
                          &color_detail_value, 0.1F, 10.0F),
      bailout_power_val(static_cast<float>(
          config.GetRenderValue(Config::RenderOption::BailoutPower))),
      bailout_power_bounds(
          Rectangle{.x = static_cast<float>(screen_width / 8),
                    .y = static_cast<float>(screen_height) -
                         static_cast<float>(CalculateVerticalMargin()),
                    .width = static_cast<float>(CalculateSliderWidth()),
                    .height = static_cast<float>(CalculateSliderHeight())}),
      bailtout_power_slider(
          bailout_power_bounds, "Bailout power", &bailout_power_val,
          Config::RENDER_BAILOUT_POWER_MIN, Config::RENDER_BAILOUT_POWER_MAX),
      full_color_palette_bounds(
          Rectangle{.x = (base_iter_bounds.x + base_iter_bounds.width) * 1.2F,
                    .y = base_iter_bounds.y,
                    .width = static_cast<float>(CalculateCheckboxWidth()),
                    .height = static_cast<float>(CalculateCheckboxHeight())}),
      full_color_palette_checkbox(full_color_palette_bounds,
                                  "Full color palette",
                                  &full_color_palette_val) {
    GuiSetStyle(DEFAULT, TEXT_SIZE, static_cast<int>(font_size));
}

void UI::Update() {
    base_iter_slider.Draw();
    color_detail_slider.Draw();
    bailtout_power_slider.Draw();
    full_color_palette_checkbox.Draw();
}

std::size_t UI::GetFontSize() const { return font_size; }

float UI::GetBaseIterValue() const { return base_iter_val; }

float UI::GetBailoutPowerValue() const { return bailout_power_val; }

float UI::GetColorDetailValue() const { return color_detail_value; }

bool UI::GetFullColorPaletteValue() const { return full_color_palette_val; }

std::size_t UI::CalculateSliderWidth() const { return screen_width / 2; }

std::size_t UI::CalculateSliderHeight() const {
    return static_cast<std::size_t>(static_cast<float>(font_size) * 1.5F);
}

std::size_t UI::CalculateCheckboxWidth() const {
    return static_cast<std::size_t>(static_cast<float>(font_size) * 1.5F);
}

std::size_t UI::CalculateCheckboxHeight() const {
    return static_cast<std::size_t>(static_cast<float>(font_size) * 1.5F);
}

std::size_t UI::CalculateVerticalMargin() const {
    return static_cast<std::size_t>(
        (static_cast<float>(screen_height) / 6) -
        (static_cast<float>(CalculateSliderHeight()) * 5));
}
