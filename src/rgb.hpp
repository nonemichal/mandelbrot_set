#pragma once

#include <array>
#include <cstddef>

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

    static constexpr RGB FromHSV(float hue_degrees, float saturation,
                                 float value) {
        constexpr float hue_sector_degrees = 60.0F;
        constexpr int hue_sector_count = 6;

        // Color intensity
        float chroma = value * saturation;

        // Position on the color wheel
        float hue_sector_float = hue_degrees / hue_sector_degrees;
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
        float match_value = value - chroma;

        return rgb_primary + RGB{match_value, match_value, match_value};
    }

    // Generates RGB color palette with evenly spaced hues around the HSV color
    // wheel
    template <size_t palette_size>
    static constexpr std::array<RGB, palette_size>
    GenPaletteHSV(float saturation = 1.0F, float value = 1.0F) {
        constexpr float hue_full_circle_degrees = 360.0F;
        std::array<RGB, palette_size> palette{};

        for (size_t i = 0; i < palette_size; i++) {
            auto palette_size_float = static_cast<float>(palette_size);
            auto index_float = static_cast<float>(i);
            float hue_degrees =
                (hue_full_circle_degrees * index_float) / palette_size_float;
            palette[i] = FromHSV(hue_degrees, saturation, value);
        }

        return palette;
    }
};
