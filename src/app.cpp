#include "app.hpp"

#include <format>
#include <string_view>

#include "raylib-cpp.hpp"
#include "raylib.h"

#include "config.hpp"
#include "drawing_scope.hpp"

// Class to run the app
App::App(const Config &config, std::string_view title)
    : window_(config.GetWindowValue(Config::WindowOption::Width),
              config.GetWindowValue(Config::WindowOption::Height),
              title.data()),
      fps_(config.GetWindowValue(Config::WindowOption::Fps)),
      shaders_{.vertex_file_name_ =
                   config.GetShaderValue(Config::ShaderOption::Vertex),
               .fragment_file_name_ =
                   config.GetShaderValue(Config::ShaderOption::Fragment)} {}

void App::Run() {
    window_.SetTargetFPS(fps_);

    // Construct shader paths
    // NOTE: Passing an empty string to Shader means "no shader" for that stage
    const std::string vertex_path =
        shaders_.vertex_file_name_.empty()
            ? std::string{}
            : std::format("{}/{}", ROOT_SV, shaders_.vertex_file_name_);
    const std::string fragment_path =
        shaders_.fragment_file_name_.empty()
            ? std::string{}
            : std::format("{}/{}", ROOT_SV, shaders_.fragment_file_name_);

    // Initialize shaders
    raylib::Shader shader(vertex_path, fragment_path);

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
        shader.SetValue(shader.GetLocation("u_color"), &color,
                        SHADER_UNIFORM_VEC3);

        // Draw
        {
            DrawingScope drawing_scope(window_);
            window_.ClearBackground(RAYWHITE);

            shader.BeginMode();

            DrawRectangle(0, 0, window_.GetWidth(), window_.GetHeight(),
                          RAYWHITE);

            shader.EndMode();
        }
    }
}
