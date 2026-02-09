#include "ui.hpp"

#include "config.hpp"
#include "raygui.h"

UI::UI()
    : base_iter_val(Config::RENDER_BASE_ITER_MAX / 2),
      slider_base_iter((Rectangle){120, 20, 600, 30}, "Base iter value",
                       &base_iter_val, Config::RENDER_BASE_ITER_MIN,
                       Config::RENDER_BASE_ITER_MAX),
      bailout_power_val(Config::RENDER_BAILOUT_POWER_MAX / 2),
      slider_bailtout_power((Rectangle){120, 60, 600, 30},
                            "Bailout power value", &bailout_power_val,
                            Config::RENDER_BAILOUT_POWER_MIN,
                            Config::RENDER_BAILOUT_POWER_MAX) {}

void UI::Update() {
    slider_base_iter.Draw();
    slider_bailtout_power.Draw();
}

float UI::GetBaseIterVal() const { return slider_base_iter.GetValue(); }

float UI::GetBailoutPowerVal() const {
    return slider_bailtout_power.GetValue();
}
