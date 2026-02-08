#pragma once

#include <array>
#include <cmath>
#include <cstddef>
#include <numbers>

struct RGB {
    float r;
    float g;
    float b;

    constexpr RGB() : r(0.0F), g(0.0F), b(0.0F) {}
    constexpr RGB(float red, float green, float blue)
        : r(red), g(green), b(blue) {}

    constexpr RGB operator+(const RGB &other) const {
        return {r + other.r, g + other.g, b + other.b};
    }

    static RGB FromHSV(float H, float S, float V) {
        static constexpr float hue_sector_degrees = 60.0F;
        static constexpr int hue_sector_count = 6;

        // Color intensity
        float chroma = V * S;

        // Position on the color wheel
        float hue_sector_float = H / hue_sector_degrees;
        int hue_sector_index = static_cast<int>(hue_sector_float);

        // Fractional part within the sector [0, 1)
        float sector_fraction =
            hue_sector_float - static_cast<float>(hue_sector_index);

        // Second largest color component
        float secondary_component =
            chroma *
            (1.0F - (sector_fraction < 0.5F ? sector_fraction * 2.0F
                                            : (1.0F - sector_fraction) * 2.0F));

        RGB rgb_primary;

        switch (hue_sector_index % hue_sector_count) {
        case 0:
            rgb_primary = {chroma, secondary_component, 0.0F};
            break;
        case 1:
            rgb_primary = {secondary_component, chroma, 0.0F};
            break;
        case 2:
            rgb_primary = {0.0F, chroma, secondary_component};
            break;
        case 3:
            rgb_primary = {0.0F, secondary_component, chroma};
            break;
        case 4:
            rgb_primary = {secondary_component, 0.0F, chroma};
            break;
        case 5:
            rgb_primary = {chroma, 0.0F, secondary_component};
            break;
        }

        // Match value (brightness)
        float match_value = V - chroma;

        return rgb_primary + RGB{match_value, match_value, match_value};
    }

    // Generates RGB color palette based on HSV representation
    // Source:
    // https://en.wikipedia.org/wiki/Plotting_algorithms_for_the_Mandelbrot_set#HSV_coloring
    template <size_t palette_size>
    static std::array<RGB, palette_size> GenPaletteHSV(float saturation = 1.0) {
        std::array<RGB, palette_size> palette{};
        for (size_t i = 0; i < palette_size; i++) {
            float s = static_cast<float>(i) / static_cast<float>(palette_size);
            float H = std::fmodf(std::pow(s * 360, 1.5F), 360);
            float S = saturation;
            float V = s * 100;
            palette[i] = FromHSV(H, S, V);
        }
        return palette;
    }

    static RGB FromLCH(float L, float C, float H) {
        // Convert hue angle from degrees to radians
        float h_rad = H * std::numbers::pi_v<float> / 180.0f;

        // Convert cylindrical LCH representation to Cartesian Lab
        float a = C * std::cos(h_rad);
        float b = C * std::sin(h_rad);

        // Reference white for D65 illuminant
        float Xn = 95.047f;
        float Yn = 100.000f;
        float Zn = 108.883f;

        // Inverse Lab nonlinear transform
        auto f_inv = [](float t) {
            float t3 = t * t * t;
            return (t3 > 0.008856f) ? t3 : (t - 16.0f / 116.0f) / 7.787f;
        };

        // Lab -> XYZ intermediate values
        float fy = (L + 16.0f) / 116.0f;
        float fx = fy + a / 500.0f;
        float fz = fy - b / 200.0f;

        float X = Xn * f_inv(fx);
        float Y = Yn * f_inv(fy);
        float Z = Zn * f_inv(fz);

        // Normalize XYZ to [0,1] range expected by sRGB matrix
        X /= 100.0f;
        Y /= 100.0f;
        Z /= 100.0f;

        // Convert XYZ -> linear RGB (sRGB conversion matrix)
        float r_lin = 3.2406f * X - 1.5372f * Y - 0.4986f * Z;
        float g_lin = -0.9689f * X + 1.8758f * Y + 0.0415f * Z;
        float b_lin = 0.0557f * X - 0.2040f * Y + 1.0570f * Z;

        // Apply sRGB gamma correction
        auto gamma = [](float u) {
            return (u <= 0.0031308f)
                       ? 12.92f * u
                       : 1.055f * std::pow(u, 1.0f / 2.4f) - 0.055f;
        };

        return RGB{gamma(r_lin), gamma(g_lin), gamma(b_lin)};
    }

    // Generates RGB color palette based on LCH representation
    // Source:
    // https://en.wikipedia.org/wiki/Plotting_algorithms_for_the_Mandelbrot_set#LCH_coloring
    template <size_t palette_size>
    static std::array<RGB, palette_size> GenPaletteLCH() {
        std::array<RGB, palette_size> palette{};
        for (size_t i = 0; i < palette_size; i++) {
            float s = static_cast<float>(i) / static_cast<float>(palette_size);
            float v =
                1.0F - std::pow(std::cos(std::numbers::pi_v<float> * s), 2.0F);
            float L = 75 - (75 * v);
            float C = 28 + L;
            float H = std::fmodf(std::pow(s * 360, 1.5F), 360);
            palette[i] = FromLCH(L, C, H);
        }
        return palette;
    }
};
