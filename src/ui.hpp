#pragma once

#include "raylib.h"

#include "slider.hpp"

class UI {
  public:
    UI();
    void Update();

    [[nodiscard]] float GetBaseIterVal() const;
    [[nodiscard]] float GetBailoutPowerVal() const;

  private:
    float base_iter_val;
    Slider slider_base_iter;
    float bailout_power_val;
    Slider slider_bailtout_power;
};
