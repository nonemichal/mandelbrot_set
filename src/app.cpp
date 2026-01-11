#include "app.hpp"

#include <memory>
#include <string_view>

#include "raylib-cpp.hpp"
#include "raylib.h"

#include "Window.hpp"
#include "config.hpp"
#include "drawing_scope.hpp"
#include "mandelbrot_error.hpp"

std::expected<std::unique_ptr<App>, MandelbrotError>
App::New(const std::string &title, std::string_view config_file) {
    // Load the config file
    TraceLog(LOG_INFO, "MANDELBROT_SET: Loading config file -> %s",
             config_file.data());
    auto config_result = Config::Load(config_file);

    // If parsing did not succeed
    if (!config_result) {
        return std::unexpected(std::move(config_result.error()));
    }

    // Parsing succeeded
    TraceLog(LOG_INFO, "MANDELBROT_SET: Config file loaded correctly");
    auto &config = config_result.value();

    // Create and initialize the app instance
    // NOTE: cannot use make_unique with private constructor
    std::unique_ptr<App> instance(new App(title, config));

    return instance;
}

App::App(const std::string &title, const Config &config)
    : fps_(config.GetWindowValue(Config::WindowOption::Fps)),
      window_(config.GetWindowValue(Config::WindowOption::Width),
              config.GetWindowValue(Config::WindowOption::Height),
              title),  // NOTE: Raylib window requires title as string
      shader_(config.GetShaderPath(Config::ShaderType::Vertex),
              config.GetShaderPath(Config::ShaderType::Fragment)) {
    // Set FPS
    window_.SetTargetFPS(fps_);
}

void App::Run() {
    float delta_red = 0.002;
    float delta_green = 0.005;
    float delta_blue = 0.007;
    float red = 1.0;
    float green = 0.6;
    float blue = 0.3;

    // Main loop
    while (!window_.ShouldClose()) {  // Detect window close button or ESC key
        // Update
        red += delta_red;
        green += delta_green;
        blue += delta_blue;

        delta_red = (red >= 1 || red <= 0) ? (delta_red * (-1)) : delta_red;
        delta_green =
            (green >= 1 || green <= 0) ? (delta_green * (-1)) : delta_green;
        delta_blue =
            (blue >= 1 || blue <= 0) ? (delta_blue * (-1)) : delta_blue;

        Vector3 color{red, green, blue};
        shader_.SetValue(shader_.GetLocation("u_color"), &color,
                         SHADER_UNIFORM_VEC3);

        // Draw
        {
            DrawingScope drawing_scope(window_);
            window_.ClearBackground(RAYWHITE);

            shader_.BeginMode();

            DrawRectangle(0, 0, window_.GetWidth(), window_.GetHeight(),
                          RAYWHITE);

            shader_.EndMode();
        }
    }
}
