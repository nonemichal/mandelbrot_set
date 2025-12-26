#pragma once

#include <array>
#include <cstdint>
#include <expected>
#include <string>
#include <string_view>

// Class that stores configuration file data
class Config {
  public:
    // Enums for config options
    enum class WindowOption : std::uint8_t { Width, Height, Fps, Count_ };
    enum class ShaderOption : std::uint8_t { Vertex, Fragment, Count_ };

    // Numbers of configuration options
    static constexpr size_t WINDOW_OPTIONS_COUNT{
        static_cast<size_t>(WindowOption::Count_)};
    static constexpr size_t SHADER_OPTIONS_COUNT{
        static_cast<size_t>(ShaderOption::Count_)};

    // Names of tables in configuration file
    static constexpr std::string_view WINDOW_TABLE_NAME{"window"};
    static constexpr std::string_view SHADER_TABLE_NAME{"shaders"};

    // List of configuration option names as strings
    // NOTE: The order must exactly match the order of the enums
    static constexpr std::array<std::string_view, WINDOW_OPTIONS_COUNT>
        WINDOW_OPTIONS_STR{"width", "height", "fps"};
    static constexpr std::array<std::string_view, SHADER_OPTIONS_COUNT>
        SHADER_OPTIONS_STR{"vertex", "fragment"};

    static_assert(WINDOW_OPTIONS_STR.size() == WINDOW_OPTIONS_COUNT,
                  "WINDOW_OPTIONS_STR size must exactly match the number of "
                  "WindowOption enum values");
    static_assert(SHADER_OPTIONS_STR.size() == SHADER_OPTIONS_COUNT,
                  "SHADER_OPTIONS_STR size must exactly match the number of "
                  "ShaderOption enum values");

    // Loads the configuration file and returns a Config object
    [[nodiscard]] static std::expected<Config, std::string>
    Load(std::string_view config_path);

    // Get window config value
    [[nodiscard]] std::expected<int, std::string>
    GetWindowValue(WindowOption option) const;

    // Get shader config value
    [[nodiscard]] std::expected<std::string_view, std::string>
    GetShaderValue(ShaderOption option) const;

  private:
    Config() = default;

    std::array<int, WINDOW_OPTIONS_COUNT> window_config_{};
    std::array<std::string, SHADER_OPTIONS_COUNT> shader_config_{};
};
