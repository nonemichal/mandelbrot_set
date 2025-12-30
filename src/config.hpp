#pragma once

#include <array>
#include <cstdint>
#include <expected>
#include <string>
#include <string_view>

#include "toml.hpp"

#include "mandelbrot_error.hpp"

// Class that stores configuration file data
class Config {
  public:
    // Enums for config options
    // NOTE: Do not use COUNT_ to access elements
    enum class WindowOption : std::uint8_t { Width, Height, Fps, COUNT_ };
    enum class ShaderType : std::uint8_t { Vertex, Fragment, COUNT_ };

    // Numbers of configuration options
    static constexpr size_t WINDOW_OPTIONS_COUNT{
        static_cast<size_t>(WindowOption::COUNT_)};
    static constexpr size_t SHADER_TYPES_COUNT{
        static_cast<size_t>(ShaderType::COUNT_)};

    // Table names in configuration file
    static constexpr std::string_view WINDOW_TABLE_NAME{"window"};
    static constexpr std::string_view SHADER_TABLE_NAME{"shaders"};

    // List of configuration option names as strings
    // NOTE: The order must exactly match the order of the enums
    static constexpr std::array<std::string_view, WINDOW_OPTIONS_COUNT>
        WINDOW_OPTIONS_STR{"width", "height", "fps"};
    static constexpr std::array<std::string_view, SHADER_TYPES_COUNT>
        SHADER_TYPES_STR{"vertex", "fragment"};

    // Project root path
    static constexpr std::string_view ROOT_SV{PROJECT_ROOT_PATH};

    // Loads the configuration file
    [[nodiscard]] static std::expected<Config, MandelbrotError>
    Load(std::string_view config_file);

    // Getters
    [[nodiscard]] int GetWindowValue(WindowOption option) const;
    [[nodiscard]] const std::filesystem::path &
    GetShaderPath(ShaderType type) const;

  private:
    // Config values
    std::array<int, WINDOW_OPTIONS_COUNT> window_config_{};
    std::array<std::filesystem::path, SHADER_TYPES_COUNT> shader_paths_{};

    // Window config boundary values
    static constexpr int WINDOW_SIZE_MIN = 64;
    static constexpr int WINDOW_SIZE_MAX = 16384;  // at least 8K
    static constexpr int WINDOW_FPS_MIN = 1;
    static constexpr int WINDOW_FPS_MAX = 1000;

    // Creates full shader paths from file name
    static std::filesystem::path
    CreateShaderPath(std::string_view shader_file_name);

    using tomlRoot = toml::basic_value<toml::type_config>;

    // Load section from config file
    std::expected<void, MandelbrotError> LoadWindowConfig(const tomlRoot &root);
    std::expected<void, MandelbrotError> LoadShaderConfig(const tomlRoot &root);
};
