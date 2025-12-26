#include "config.hpp"

#include <array>
#include <expected>
#include <format>
#include <print>
#include <string>
#include <string_view>

#include "toml.hpp"

// Loads the configuration file and returns a Config object
std::expected<Config, std::string> Config::Load(std::string_view config_path) {
    // Parse config file
    auto parse_result = toml::try_parse(config_path.data());
    // Check if parsing succeeded
    if (!parse_result.is_ok()) {
        const auto &errors = parse_result.unwrap_err();
        // Use the first error and convert it to string
        const std::string error_msg = format_error(errors.at(0));
        return std::unexpected(
            std::format("Failed to parse config file:\n{}", error_msg));
    }
    // Get config file root
    const auto &root = parse_result.unwrap();

    // Create empty config
    Config config{};

    // Window table section
    for (size_t i = 0; i < WINDOW_OPTIONS_COUNT; i++) {
        const auto *const table_name = WINDOW_TABLE_NAME.data();
        const auto *const option_name = WINDOW_OPTIONS_STR.at(i).data();

        const auto find_val =
            toml::find<std::optional<int>>(root, table_name, option_name);
        if (!find_val.has_value()) {
            // If value not found return error
            return std::unexpected(
                std::format("Missing window config option: {}", option_name));
        }
        config.window_config_.at(i) = *find_val;
    }

    // Shaders table section
    for (size_t i = 0; i < SHADER_OPTIONS_COUNT; i++) {
        const auto *const table_name = SHADER_TABLE_NAME.data();
        const auto *const option_name = SHADER_OPTIONS_STR.at(i).data();

        const auto find_val = toml::find<std::optional<std::string>>(
            root, table_name, option_name);
        if (!find_val.has_value()) {
            // If value not found return error
            return std::unexpected(
                std::format("Missing shader config option: {}", option_name));
        }
        config.shader_config_.at(i) = *find_val;
    }

    return config;
}

// Get window config value
std::expected<int, std::string>
Config::GetWindowValue(WindowOption option) const {
    auto index = static_cast<size_t>(option);
    if (index >= WINDOW_OPTIONS_COUNT) {
        return std::unexpected("Invalid window config option");
    }

    return window_config_.at(index);
}

// Get shader config value
std::expected<std::string_view, std::string>
Config::GetShaderValue(ShaderOption option) const {
    auto index = static_cast<size_t>(option);
    if (index >= SHADER_OPTIONS_COUNT) {
        return std::unexpected("Invalid shader config option");
    }

    return shader_config_.at(index);
}
