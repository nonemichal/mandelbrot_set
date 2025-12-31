#include "config.hpp"

#include <array>
#include <expected>
#include <format>
#include <string>
#include <string_view>

#include "raylib-cpp.hpp"
#include "toml.hpp"

#include "mandelbrot_error.hpp"

// Loads the configuration file
std::expected<Config, MandelbrotError>
Config::Load(std::string_view config_file) {
    // Create config path
    const std::filesystem::path config_path =
        std::filesystem::path(PROJECT_ROOT_PATH) / config_file;

    // Validate config path
    if (!std::filesystem::exists(config_path)) {
        auto error_msg = std::format("Config file does not exist -> {}",
                                     config_path.string());
        return std::unexpected(
            MandelbrotError(MandelbrotError::Code::FileNotFound, error_msg));
    }

    // Parse config file
    auto parse_result = toml::try_parse(config_path);

    // Check if parsing succeeded
    if (!parse_result.is_ok()) {
        const auto &errors = parse_result.unwrap_err();
        // Use the first error and convert it to string
        std::string first_msg = format_error(errors.at(0));
        auto error_msg =
            std::format("Failed to parse config file -> {}", first_msg);
        return std::unexpected(
            MandelbrotError(MandelbrotError::Code::ParseError, error_msg));
    }

    // Extract the root TOML value
    const auto &root = parse_result.unwrap();

    // Template error message
    constexpr std::string_view has_value_error_msg{
        "Config file must contain a table [{}]"};
    // Check if window table exists
    if (!HasTable(root, WINDOW_TABLE_NAME)) {
        auto error_msg = std::format(has_value_error_msg, WINDOW_TABLE_NAME);
        return std::unexpected(
            MandelbrotError(MandelbrotError::Code::ParseError, error_msg));
    }
    // Check if shader table exists
    if (!HasTable(root, SHADER_TABLE_NAME)) {
        auto error_msg = std::format(has_value_error_msg, SHADER_TABLE_NAME);
        return std::unexpected(
            MandelbrotError(MandelbrotError::Code::ParseError, error_msg));
    }

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

bool Config::HasTable(const tomlRoot &root, std::string_view table_name) {
    // Check if root contains the key
    if (!root.contains(table_name.data())) {
        return false;
    }
    const auto &table_value = root.at(table_name.data());
    // Validate that the value is a table
    return table_value.is(toml::value_t::table);
}

std::expected<void, MandelbrotError>
Config::LoadWindowConfig(const tomlRoot &root) {
    // Table with window config
    const auto *const table_name = WINDOW_TABLE_NAME.data();

    // Iterate over all expected window configuration options
    for (size_t i = 0; i < WINDOW_OPTIONS_COUNT; i++) {
        const auto *const option_name = WINDOW_OPTIONS_STR.at(i).data();

        // Try to find the option
        // NOTE: type_error is thrown when the value has an invalid type
        std::optional<int> find_value;
        try {
            find_value =
                toml::find<std::optional<int>>(root, table_name, option_name);
        } catch (const toml::type_error &) {
            auto error_msg = std::format(
                "Invalid type for option '{}', expected int", option_name);
            return std::unexpected(MandelbrotError(
                MandelbrotError::Code::InvalidValue, error_msg));
        }

        // If the option is missing return error
        if (!find_value.has_value()) {
            auto error_msg =
                std::format("Missing window config option -> {}", option_name);
            return std::unexpected(MandelbrotError(
                MandelbrotError::Code::MissingOption, error_msg));
        }

        // Get value
        int value = *find_value;

        // Common error message template
        constexpr std::string_view validate_error_msg{
            "Window config option {} out of range [{}..{}] -> {}"};

        // Validate value
        switch (static_cast<WindowOption>(i)) {
        // Cases for width and height share the same validation rules
        case WindowOption::Width:
        case WindowOption::Height:
            if (value < WINDOW_SIZE_MIN || value > WINDOW_SIZE_MAX) {
                auto error_msg =
                    std::format(validate_error_msg, option_name,
                                WINDOW_SIZE_MIN, WINDOW_SIZE_MAX, value);
                return std::unexpected(MandelbrotError(
                    MandelbrotError::Code::InvalidValue, error_msg));
            }
            break;
        case WindowOption::Fps:
            if (value < WINDOW_FPS_MIN || value > WINDOW_FPS_MAX) {
                auto error_msg =
                    std::format(validate_error_msg, option_name, WINDOW_FPS_MIN,
                                WINDOW_FPS_MAX, value);
                return std::unexpected(MandelbrotError(
                    MandelbrotError::Code::InvalidValue, error_msg));
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

std::expected<void, MandelbrotError>
Config::LoadShaderConfig(const tomlRoot &root) {
    // Table with shader paths
    const auto *const table_name = SHADER_TABLE_NAME.data();

    // Iterate over all expected shader configuration options
    for (size_t i = 0; i < SHADER_TYPES_COUNT; i++) {
        const auto *const option_name = SHADER_TYPES_STR.at(i).data();

        // Try to find the option
        // NOTE: type_error is thrown when the value has an invalid type
        std::optional<std::string> find_value;
        try {
            find_value = toml::find<std::optional<std::string>>(
                root, table_name, option_name);
        } catch (const toml::type_error &) {
            auto error_msg = std::format(
                "Invalid type for option '{}', expected string", option_name);
            return std::unexpected(MandelbrotError(
                MandelbrotError::Code::InvalidValue, error_msg));
        }

        // If the option is missing, return an error
        if (!find_value.has_value()) {
            auto error_msg =
                std::format("Missing shader path -> {}", option_name);
            return std::unexpected(MandelbrotError(
                MandelbrotError::Code::MissingOption, error_msg));
        }

        // Crate path
        auto shader_path = CreateShaderPath(*find_value);

        // If path is not empty, check if file exists
        if (!shader_path.empty() && !std::filesystem::exists(shader_path)) {
            auto error_msg = std::format("Shader file does not exist -> {}",
                                         shader_path.string());
            return std::unexpected(MandelbrotError(
                MandelbrotError::Code::FileNotFound, error_msg));
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
// does not attempt to access COUNT_
int Config::GetWindowValue(WindowOption option) const {
    const auto index = static_cast<size_t>(option);
    assert(index < window_config_.size());
    const auto value = window_config_.at(index);
    return value;
}

// NOTE: Assumes the caller uses a valid option and
// does not attempt to access COUNT_
const std::filesystem::path &Config::GetShaderPath(ShaderType type) const {
    const auto index = static_cast<size_t>(type);
    assert(index < shader_paths_.size());
    const auto &value = shader_paths_.at(index);
    return value;
}
