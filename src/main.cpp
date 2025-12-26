#include <print>
#include <string>
#include <string_view>

#include "raylib-cpp.hpp"
#include "raylib.h"

#include "app.hpp"
#include "config.hpp"

#define CONFIG_FILE "/config.toml"
#define CONFIG_PATH                                                            \
    PROJECT_ROOT_PATH CONFIG_FILE  // PROJECT_ROOT_PATH is defined in cmake
#define TITLE "Mandelbrot Set"

constexpr std::string_view CONFIG_PATH_SV{CONFIG_PATH};

int main() {
    // Load the config
    TraceLog(LOG_INFO, "MANDELBROT_SET: Loading config file: %s",
             CONFIG_PATH_SV.data());
    auto config_result = Config::Load(CONFIG_PATH_SV);
    if (!config_result) {
        const auto *error_msg = config_result.error().c_str();
        TraceLog(LOG_ERROR, "MANDELBROT_SET: Failed to load configuration: %s",
                 error_msg);
        return 1;
    }
    TraceLog(LOG_INFO, "MANDELBROT_SET: File loaded correctly");
    const auto &config = config_result.value();

    // App initialization
    App app(config, TITLE);

    // Run the app
    app.Run();

    return 0;
}
