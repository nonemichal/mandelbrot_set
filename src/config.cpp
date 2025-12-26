#include "config.hpp"

#include <array>
#include <expected>
#include <format>
#include <print>
#include <string>
#include <string_view>

#include "raylib-cpp.hpp"

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

    // Initialize an empty Config object
    Config config{};

    // Window table section
    for (size_t i = 0; i < WINDOW_OPTIONS_COUNT; i++) {
        const auto *const table_name = WINDOW_TABLE_NAME.data();
        const auto *const option_name = WINDOW_OPTIONS_STR.at(i).data();

        const auto find_val =
            toml::find<std::optional<int>>(root, table_name, option_name);
        if (!find_val.has_value()) {
            // Return an error if the option is missing
            return std::unexpected(
                std::format("Missing window config option: {}", option_name));
        }
        config.window_config_.at(i) = *find_val;
        TraceLog(LOG_INFO, "MANDELBROT_SET: Setting %s %s -> %d", table_name,
                 option_name, *find_val);
    }

    // Shaders table section
    for (size_t i = 0; i < SHADER_OPTIONS_COUNT; i++) {
        const auto *const table_name = SHADER_TABLE_NAME.data();
        const auto *const option_name = SHADER_OPTIONS_STR.at(i).data();

        const auto find_val = toml::find<std::optional<std::string>>(
            root, table_name, option_name);
        if (!find_val.has_value()) {
            // Return an error if the option is missing
            return std::unexpected(
                std::format("Missing shader config option: {}", option_name));
        }
        config.shader_config_.at(i) = *find_val;
        TraceLog(LOG_INFO, "MANDELBROT_SET: Setting %s %s -> %s", table_name,
                 option_name, find_val->c_str());
    }

    return config;
}

// NOTE: Assumes the caller uses a valid option and
// does not attempt to access Count_
int Config::GetWindowValue(WindowOption option) const {
    const auto index = static_cast<size_t>(option);
    assert(index < window_config_.size());
    const auto value = window_config_.at(index);
    TraceLog(LOG_DEBUG, "MANDELBROT_SET: Getting window %s -> %d",
             WINDOW_OPTIONS_STR.at(index).data(), value);
    return value;
}

// NOTE: Assumes the caller uses a valid option and
// does not attempt to access Count_
std::string_view Config::GetShaderValue(ShaderOption option) const {
    const auto index = static_cast<size_t>(option);
    assert(index < shader_config_.size());
    const auto &value = shader_config_.at(index);
    TraceLog(LOG_DEBUG, "MANDELBROT_SET: Getting shader %s -> %s",
             SHADER_OPTIONS_STR.at(index).data(), value.c_str());
    return value;
}
