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
      shaders_{.vertex = config.GetShaderValue(Config::ShaderOption::Vertex),
               .fragment =
                   config.GetShaderValue(Config::ShaderOption::Fragment)} {}

void App::Run() {
    window_.SetTargetFPS(fps_);

    constexpr std::string_view root_sv{PROJECT_ROOT_PATH};
    std::string fragment_path =
        std::format("{}/{}", root_sv, shaders_.fragment);
    raylib::Shader shader(shaders_.vertex.data(), fragment_path);

    Vector3 color{1.0F, 0.0F, 0.0F};
    shader.SetValue(shader.GetLocation("u_color"), &color, SHADER_UNIFORM_VEC3);

    // Main loop
    while (!window_.ShouldClose()) {  // Detect window close button or ESC key
        // Update

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
