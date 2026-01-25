#include "app.hpp"

#include <string_view>

#include "raylib-cpp.hpp"

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
    : fps(config.GetWindowValue(Config::WindowOption::Fps)),
      window(config.GetWindowValue(Config::WindowOption::Width),
             config.GetWindowValue(Config::WindowOption::Height),
             title),  // NOTE: Raylib window requires title as string
      shader(config.GetShaderPath(Config::ShaderType::Vertex),
             config.GetShaderPath(Config::ShaderType::Fragment)) {
    window.SetTargetFPS(fps);
    // Create a texture to be used for render
    // NOTE: "Rectangle uses font white character texture coordinates,
    // So shader can not be applied here directly because input vertexTexCoord
    // Do not represent full screen coordinates (space where want to apply
    // shader)"
    // https://github.com/raysan5/raylib/blob/master/examples/shaders/shaders_mandelbrot_set.c#L190
    render_texture =
        raylib::RenderTexture::Load(window.GetWidth(), window.GetHeight());
    texture = render_texture.GetTexture();

    // Prepare color palette
    for (size_t i = 0; i < PALETTE_SIZE; ++i) {
        color_palette.at(i) = Color{
            static_cast<unsigned char>(HSV_PALETTE.at(i).r * 255.0F),
            static_cast<unsigned char>(HSV_PALETTE.at(i).g * 255.0F),
            static_cast<unsigned char>(HSV_PALETTE.at(i).b * 255.0F), 255};
    }
    Image palette_image(color_palette.data(), static_cast<int>(PALETTE_SIZE), 1,
                        1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    palette_texture = raylib::Texture(palette_image);
}

void App::Run() {
    // Main loop
    while (!window.ShouldClose()) {  // Detect window close button or ESC key
        PrepareTexture();
        Draw();
    }
}

// Prepare a texture to be used later as canvas
void App::PrepareTexture() {
    render_texture.BeginMode();
    window.ClearBackground(BLACK);
    const auto width_float = static_cast<float>(window.GetWidth());
    const auto height_float = static_cast<float>(window.GetHeight());
    static const raylib::Rectangle rectangle(0, 0, width_float, height_float);
    rectangle.Draw(BLACK);
    render_texture.EndMode();
}

// Draw the saved texture and render shaders
void App::Draw() {
    window.BeginDrawing();
    window.ClearBackground(BLACK);
    shader.BeginMode();
    shader.SetValue(shader.GetLocation("uColorPalette"), palette_texture);
    static const raylib::Vector2 pos{0.0, 0.0};
    texture.Draw(pos);
    shader.EndMode();
    window.EndDrawing();
}
