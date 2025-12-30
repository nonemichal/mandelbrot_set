#include "config.hpp"

#include <array>
#include <expected>
#include <format>
#include <string>
#include <string_view>

#include "raylib-cpp.hpp"
#include "toml.hpp"

// Loads the configuration file
std::expected<Config, std::string> Config::Load(std::string_view config_path) {
    // Validate config path
    if (!std::filesystem::exists(config_path)) {
        return std::unexpected(
            std::format("Config file does not exist -> {}", config_path));
    }

    // Parse config file
    auto parse_result = toml::try_parse(config_path.data());

    // Check if parsing succeeded
    if (!parse_result.is_ok()) {
        const auto &errors = parse_result.unwrap_err();
        // Use the first error and convert it to string
        const std::string error_msg = format_error(errors.at(0));
        return std::unexpected(
            std::format("Failed to parse config file -> {}", error_msg));
    }

    // Extract the root TOML value
    const auto &root = parse_result.unwrap();

    // Create an empty configuration object
    Config config{};

    // Load window related configuration
    auto window_res = config.LoadWindowConfig(root);
    if (!window_res) {
        return std::unexpected(window_res.error());
    }

    // Load shader related configuration
    auto shader_res = config.LoadShaderConfig(root);
    if (!shader_res) {
        return std::unexpected(shader_res.error());
    }

    // Configuration loaded successfully
    return config;
}

std::expected<void, std::string>
Config::LoadWindowConfig(const tomlRoot &root) {
    // Table with window config
    const auto *const table_name = WINDOW_TABLE_NAME.data();

    // Iterate over all expected window configuration options
    for (size_t i = 0; i < WINDOW_OPTIONS_COUNT; i++) {
        const auto *const option_name = WINDOW_OPTIONS_STR.at(i).data();

        // Try to find the option
        auto find_value =
            toml::find<std::optional<int>>(root, table_name, option_name);

        // If the option is missing return error
        if (!find_value.has_value()) {
            return std::unexpected(
                std::format("Missing window config option -> {}", option_name));
        }

        // Get value
        int value = *find_value;

        constexpr std::string_view validate_error_msg{
            "Window config option {} out of range [{}..{}] -> {}"};

        // Validate value
        switch (static_cast<WindowOption>(i)) {
        // Cases for width and height share the same validation rules
        case WindowOption::Width:
        case WindowOption::Height:
            if (value < WINDOW_SIZE_MIN || value > WINDOW_SIZE_MAX) {
                return std::unexpected(std::format(validate_error_msg,
                                                   option_name, WINDOW_SIZE_MIN,
                                                   WINDOW_SIZE_MAX, value));
            }
            break;
        case WindowOption::Fps:
            if (value < WINDOW_FPS_MIN || value > WINDOW_FPS_MAX) {
                return std::unexpected(std::format(validate_error_msg,
                                                   option_name, WINDOW_FPS_MIN,
                                                   WINDOW_FPS_MAX, value));
            }
            break;
        }

        // Set value
        window_config_.at(i) = value;
        TraceLog(LOG_INFO, "MANDELBROT_SET: Setting %s %s -> %d", table_name,
                 option_name, value);
    }
    return {};
}

std::expected<void, std::string>
Config::LoadShaderConfig(const tomlRoot &root) {
    // Table with shader paths
    const auto *const table_name = SHADER_TABLE_NAME.data();

    // Iterate over all expected shader configuration options
    for (size_t i = 0; i < SHADER_TYPES_COUNT; i++) {
        const auto *const option_name = SHADER_TYPES_STR.at(i).data();

        // Try to find the option
        auto find_val = toml::find<std::optional<std::string>>(root, table_name,
                                                               option_name);

        // If the option is missing, return an error
        if (!find_val.has_value()) {
            return std::unexpected(
                std::format("Missing shader path -> {}", option_name));
        }

        // Crate path
        auto shader_path = CreateShaderPath(*find_val);

        // If path is not empty, check if file exists
        if (!shader_path.empty() && !std::filesystem::exists(shader_path)) {
            return std::unexpected(std::format(
                "Shader file does not exist -> {}", shader_path.string()));
        }

        // Store the path in the array
        shader_paths_.at(i) = std::move(shader_path);
        TraceLog(LOG_INFO, "MANDELBROT_SET: Setting %s %s -> %s", table_name,
                 option_name, shader_paths_.at(i).c_str());
    }
    return {};
}

std::filesystem::path
Config::CreateShaderPath(std::string_view shader_file_name) {
    // NOTE: Passing an empty string means "no shader" for that stage
    if (shader_file_name.empty()) {
        return {};  // empty path
    }
    std::filesystem::path root{ROOT_SV};
    std::filesystem::path shader_path{root / shader_file_name};

    return shader_path;
}

// NOTE: Assumes the caller uses a valid option and
// does not attempt to access Count_
int Config::GetWindowValue(WindowOption option) const {
    const auto index = static_cast<size_t>(option);
    assert(index < window_config_.size());
    const auto value = window_config_.at(index);
    return value;
}

// NOTE: Assumes the caller uses a valid option and
// does not attempt to access Count_
const std::filesystem::path &Config::GetShaderPath(ShaderType type) const {
    const auto index = static_cast<size_t>(type);
    assert(index < shader_paths_.size());
    const auto &value = shader_paths_.at(index);
    return value;
}
