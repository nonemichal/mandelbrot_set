#include "app.hpp"

#include <string_view>

#include "raylib-cpp.hpp"
#include "raylib.h"

#include "RenderTexture.hpp"
#include "Window.hpp"
#include "config.hpp"
#include "mandelbrot_error.hpp"

std::expected<App *, MandelbrotError>
App::Instance(const std::string &title, std::string_view config_file) {
    static std::optional<App> instance;

    // If the app is initialized
    if (instance.has_value()) {
        TraceLog(LOG_INFO, "MANDELBROT_SET: The app instance already exists -> "
                           "returning the existing instance");
        return &*instance;
    }

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
    const auto &config = config_result.value();

    // Initialize the app instance
    instance.emplace(title, config);
    TraceLog(LOG_INFO, "MANDELBROT_SET: The app instance created correctly");

    return &*instance;
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
    // Get shader locations
    window_width_loc_ = shader_.GetLocation("windowWidth");
    window_height_loc_ = shader_.GetLocation("windowHeight");
    // Set shader uniforms
    auto window_width = window_.GetWidth();
    auto window_height = window_.GetHeight();
    shader_.SetValue(window_width_loc_, &window_width, SHADER_UNIFORM_FLOAT);
    shader_.SetValue(window_height_loc_, &window_height, SHADER_UNIFORM_FLOAT);
    // Create a texture to be used for render
    // NOTE: "Rectangle uses font white character texture coordinates,
    // So shader can not be applied here directly because input vertexTexCoord
    // Do not represent full screen coordinates (space where want to apply
    // shader)"
    // https://github.com/raysan5/raylib/blob/master/examples/shaders/shaders_mandelbrot_set.c#L190
    render_texture_ =
        raylib::RenderTexture::Load(window_.GetWidth(), window_.GetHeight());
    texture_ = render_texture_.GetTexture();
}

void App::Run() {
    // Main loop
    while (!window_.ShouldClose()) {  // Detect window close button or ESC key
        PrepareTexture();
        Draw();
    }
}

// Prepare a texture to be used later as canvas
void App::PrepareTexture() {
    render_texture_.BeginMode();
    window_.ClearBackground(BLACK);
    const auto width_float = static_cast<float>(window_.GetWidth());
    const auto height_float = static_cast<float>(window_.GetHeight());
    static const raylib::Rectangle rectangle(0, 0, width_float, height_float);
    rectangle.Draw(BLACK);
    render_texture_.EndMode();
}

// Draw the saved texture and render shaders
void App::Draw() {
    window_.BeginDrawing();
    window_.ClearBackground(BLACK);
    shader_.BeginMode();
    static const raylib::Vector2 pos{0.0, 0.0};
    texture_.Draw(pos);
    shader_.EndMode();
    window_.EndDrawing();
}
