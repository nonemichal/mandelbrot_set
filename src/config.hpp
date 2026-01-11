#pragma once

#include <array>
#include <cstdint>
#include <expected>
#include <string>
#include <string_view>

#include "toml.hpp"

#include "enum_list.hpp"
#include "mandelbrot_error.hpp"

// Class that stores configuration file data
class Config {
  public:
    // Enums for config options
    enum class WindowOption : std::uint8_t {
#define X(name, str) name,  // Expand each macro element into an enum value
        WINDOW_OPTION_LIST(X)
#undef X
    };
    enum class ShaderType : std::uint8_t {
#define X(name, str) name,  // Expand each macro element into an enum value
        SHADER_TYPE_LIST(X)
#undef X
    };

    // Numbers of configuration options
    static constexpr size_t WINDOW_OPTIONS_COUNT{
        0 WINDOW_OPTION_LIST(X_ENUM_COUNT)};
    static constexpr size_t SHADER_TYPES_COUNT{
        0 SHADER_TYPE_LIST(X_ENUM_COUNT)};

    // Array of string names for window options
    static constexpr std::array<std::string_view, WINDOW_OPTIONS_COUNT>
        WINDOW_OPTIONS_STR{
#define X(name, str) str,  // Expand each macro element into its string
            WINDOW_OPTION_LIST(X)
#undef X
        };

    // Array of string names for shader types
    static constexpr std::array<std::string_view, SHADER_TYPES_COUNT>
        SHADER_TYPES_STR{
#define X(name, str) str,  // Expand each macro element into its string
            SHADER_TYPE_LIST(X)
#undef X
        };

    // Table names in configuration file
    static constexpr std::string_view WINDOW_TABLE_NAME{"window"};
    static constexpr std::string_view SHADER_TABLE_NAME{"shaders"};

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
    static constexpr int WINDOW_SIZE_MAX = 16384;
    static constexpr int WINDOW_FPS_MIN = 1;
    static constexpr int WINDOW_FPS_MAX = 1000;

    // Creates full shader paths from file name
    static std::filesystem::path
    CreateShaderPath(std::string_view shader_file_name);

    // Config file root type
    using tomlRoot = toml::basic_value<toml::type_config>;

    // Validation whether the config file has the appropriate table
    static bool HasTable(const tomlRoot &root, std::string_view table_name);

    // Load section from config file
    std::expected<void, MandelbrotError> LoadWindowConfig(const tomlRoot &root);
    std::expected<void, MandelbrotError> LoadShaderConfig(const tomlRoot &root);
};
