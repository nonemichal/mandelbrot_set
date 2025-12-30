#pragma once

#include <array>
#include <cstdint>
#include <expected>
#include <string>
#include <string_view>

#include "toml.hpp"

// Class that stores configuration file data
class Config {
  public:
    // Enums for config options
    // NOTE: Do not use Count_ to access elements
    enum class WindowOption : std::uint8_t { Width, Height, Fps, Count_ };
    enum class ShaderType : std::uint8_t { Vertex, Fragment, Count_ };

    // Numbers of configuration options
    static constexpr size_t WINDOW_OPTIONS_COUNT{
        static_cast<size_t>(WindowOption::Count_)};
    static constexpr size_t SHADER_TYPES_COUNT{
        static_cast<size_t>(ShaderType::Count_)};

    // Names of tables in configuration file
    static constexpr std::string_view WINDOW_TABLE_NAME{"window"};
    static constexpr std::string_view SHADER_TABLE_NAME{"shaders"};

    // List of configuration option names as strings
    // NOTE: The order must exactly match the order of the enums
    static constexpr std::array<std::string_view, WINDOW_OPTIONS_COUNT>
        WINDOW_OPTIONS_STR{"width", "height", "fps"};
    static constexpr std::array<std::string_view, SHADER_TYPES_COUNT>
        SHADER_TYPES_STR{"vertex", "fragment"};

    static_assert(WINDOW_OPTIONS_STR.size() == WINDOW_OPTIONS_COUNT,
                  "WINDOW_OPTIONS_STR size must exactly match the number of "
                  "WindowOption enum values");
    static_assert(SHADER_TYPES_STR.size() == SHADER_TYPES_COUNT,
                  "SHADER_TYPES_STR size must exactly match the number of "
                  "ShaderType enum values");

    [[nodiscard]] static std::expected<Config, std::string>
    Load(std::string_view config_path);

    // Getters
    [[nodiscard]] int GetWindowValue(WindowOption option) const;
    [[nodiscard]] const std::filesystem::path &
    GetShaderPath(ShaderType type) const;

  private:
    // Project root path
    static constexpr std::string_view ROOT_SV{PROJECT_ROOT_PATH};

    // Config values
    std::array<int, WINDOW_OPTIONS_COUNT> window_config_{};
    std::array<std::filesystem::path, SHADER_TYPES_COUNT> shader_paths_{};

    // Window config boundary values
    static constexpr int WINDOW_SIZE_MIN = 64;
    static constexpr int WINDOW_SIZE_MAX = 16384;  // at least 8K
    static constexpr int WINDOW_FPS_MIN = 1;
    static constexpr int WINDOW_FPS_MAX = 1000;

    static std::filesystem::path
    CreateShaderPath(std::string_view shader_file_name);

    using tomlRoot = toml::basic_value<toml::type_config>;

    std::expected<void, std::string> LoadWindowConfig(const tomlRoot &root);
    std::expected<void, std::string> LoadShaderConfig(const tomlRoot &root);
};
