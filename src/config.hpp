#pragma once

#include <array>
#include <cstdint>
#include <expected>
#include <format>
#include <string>
#include <string_view>

#include "raylib-cpp.hpp"
#include "toml.hpp"

#include "enum_list.hpp"
#include "mandelbrot_error.hpp"

// Class that stores configuration file data
class Config {
  public:
    // Enums for config options
    enum class WindowOption : std::uint8_t {
#define X(name, str) name,
        WINDOW_OPTION_LIST(X)
#undef X
    };
    enum class ShaderType : std::uint8_t {
#define X(name, str) name,
        SHADER_TYPE_LIST(X)
#undef X
    };
    enum class RenderOption : std::uint8_t {
#define X(name, str) name,
        RENDER_OPTION_LIST(X)
#undef X
    };

    // Numbers of configuration options
    static constexpr size_t WINDOW_OPTIONS_COUNT{
        0 WINDOW_OPTION_LIST(X_ENUM_COUNT)};
    static constexpr size_t SHADER_TYPES_COUNT{
        0 SHADER_TYPE_LIST(X_ENUM_COUNT)};
    static constexpr size_t RENDER_OPTIONS_COUNT{
        0 RENDER_OPTION_LIST(X_ENUM_COUNT)};

    // Array of string names for window options
    static constexpr std::array<std::string_view, WINDOW_OPTIONS_COUNT>
        WINDOW_OPTIONS_STR{
#define X(name, str) str,
            WINDOW_OPTION_LIST(X)
#undef X
        };

    // Array of string names for shader types
    static constexpr std::array<std::string_view, SHADER_TYPES_COUNT>
        SHADER_TYPES_STR{
#define X(name, str) str,
            SHADER_TYPE_LIST(X)
#undef X
        };

    // Array of string names for render options
    static constexpr std::array<std::string_view, RENDER_OPTIONS_COUNT>
        RENDER_OPTIONS_STR{
#define X(name, str) str,
            RENDER_OPTION_LIST(X)
#undef X
        };

    // Table names in configuration file
    static constexpr std::string_view WINDOW_TABLE_NAME{"window"};
    static constexpr std::string_view SHADER_TABLE_NAME{"shaders"};
    static constexpr std::string_view RENDER_TABLE_NAME{"render"};

    // Project root path
    static constexpr std::string_view ROOT_SV{PROJECT_ROOT_PATH};

    // Config boundary values
    static constexpr int WINDOW_SIZE_MIN = 400;
    static constexpr int WINDOW_SIZE_MAX = 6000;
    static constexpr int WINDOW_FPS_MIN = 1;
    static constexpr int WINDOW_FPS_MAX = 1000;
    static constexpr int RENDER_BASE_ITER_MIN = 1;
    static constexpr int RENDER_BASE_ITER_MAX = 500;
    static constexpr int RENDER_BAILOUT_POWER_MIN = 2;
    static constexpr int RENDER_BAILOUT_POWER_MAX = 30;

    // Loads the configuration file
    [[nodiscard]] static std::expected<Config, MandelbrotError>
    Load(const std::filesystem::path &config_path);

    // Getters
    [[nodiscard]] int GetWindowValue(WindowOption option) const;
    [[nodiscard]] const std::filesystem::path &
    GetShaderPath(ShaderType type) const;
    [[nodiscard]] int GetRenderValue(
        RenderOption option) const;  // NOTE: Currently returned type is always
                                     // int but it can change in the future

  private:
    // Config values
    std::array<int, WINDOW_OPTIONS_COUNT> window_config{};
    std::array<std::filesystem::path, SHADER_TYPES_COUNT> shader_paths{};
    std::array<int, RENDER_OPTIONS_COUNT> render_config{};

    // Creates full shader paths from file name
    static std::filesystem::path
    CreateShaderPath(std::string_view shader_file_name);

    // Config file root type
    using tomlRoot = toml::basic_value<toml::type_config>;

    // Validation whether the config file has the appropriate table
    static bool HasTable(const tomlRoot &root, std::string_view table_name);

    // Load section from config file
    // TODO: One universal function should be written
    std::expected<void, MandelbrotError> LoadWindowConfig(const tomlRoot &root);
    std::expected<void, MandelbrotError> LoadShaderConfig(const tomlRoot &root);
    std::expected<void, MandelbrotError> LoadRenderConfig(const tomlRoot &root);
};
