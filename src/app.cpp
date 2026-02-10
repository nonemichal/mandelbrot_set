#include "app.hpp"

std::expected<App *, MandelbrotError>
App::Instance(const std::string &title, std::string_view config_file) {
    static std::optional<App> instance;

    // If the app is initialized
    if (instance.has_value()) {
        TraceLog(LOG_INFO, "MANDELBROT_SET: The app instance already exists -> "
                           "returning the existing instance");
        return &*instance;
    }

    // Create config path
    const std::filesystem::path config_path =
        std::filesystem::path(PROJECT_ROOT_PATH) / config_file;

    // Load the config file
    TraceLog(LOG_INFO, "MANDELBROT_SET: Loading config file -> %s",
             config_path.c_str());
    auto config_result = Config::Load(config_path);

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
             config.GetShaderPath(Config::ShaderType::Fragment)),

      ui(config),
      // NOTE: The shader is designed to work best with a 7:5 (width/height)
      // aspect ratio. Assuming the screen itself has a ~7:6 ratio, limiting
      // the render height to 5/6 of the window keeps the shader rendering
      // area close to 7:5. The remaining space is reserved for UI elements.
      render_width(static_cast<std::size_t>(window.GetWidth())),
      render_height(static_cast<std::size_t>(window.GetHeight() * 5 / 6)),
      base_iter(config.GetRenderValue(Config::RenderOption::BaseIter)),
      bailout_power(config.GetRenderValue(Config::RenderOption::BailoutPower)),
      full_color_palette(ui.GetFullColorPaletteValue()) {
    window.SetTargetFPS(fps);
    // Create a texture to be used for render
    // NOTE: "Rectangle uses font white character texture coordinates,
    // So shader can not be applied here directly because input vertexTexCoord
    // Do not represent full screen coordinates (space where want to apply
    // shader)"
    // https://github.com/raysan5/raylib/blob/master/examples/shaders/shaders_mandelbrot_set.c#L190
    render_texture = raylib::RenderTexture::Load(
        static_cast<int>(render_width), static_cast<int>(render_height));
    texture = render_texture.GetTexture();

    // Prepare color palette scaled to [0, 255]
    static constexpr unsigned char color_max_value = 255;
    for (size_t i = 0; i < PALETTE_SIZE; ++i) {
        color_palette.at(i) =
            Color{static_cast<unsigned char>(GENERATED_PALETTE.at(i).r *
                                             color_max_value),
                  static_cast<unsigned char>(GENERATED_PALETTE.at(i).g *
                                             color_max_value),
                  static_cast<unsigned char>(GENERATED_PALETTE.at(i).b *
                                             color_max_value),
                  color_max_value};
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
    const auto width_float = static_cast<float>(render_width);
    const auto height_float = static_cast<float>(render_height);
    static const raylib::Rectangle rectangle(0, 0, width_float, height_float);
    rectangle.Draw(BLACK);
    render_texture.EndMode();
}

void App::RenderShader() {
    constexpr float zoom = 1.0;
    // Update max iter based on base iter and current zoom
    const int max_iter =
        static_cast<int>(static_cast<float>(base_iter) * std::pow(zoom, 0.1));
    shader.BeginMode();
    shader.SetValue(shader.GetLocation("uColorPalette"), palette_texture);

    shader.SetValue(shader.GetLocation("zoom"), &zoom, SHADER_UNIFORM_FLOAT);
    shader.SetValue(shader.GetLocation("maxIter"), &max_iter,
                    SHADER_UNIFORM_INT);
    shader.SetValue(shader.GetLocation("bailoutPower"), &bailout_power,
                    SHADER_UNIFORM_INT);
    shader.SetValue(shader.GetLocation("paletteSize"), &PALETTE_SIZE,
                    SHADER_UNIFORM_INT);
    shader.SetValue(shader.GetLocation("scaleIter"), &full_color_palette,
                    SHADER_UNIFORM_INT);
    texture.Draw();
    shader.EndMode();
}

// Draw the saved texture and render shaders
void App::Draw() {
    window.BeginDrawing();
    window.ClearBackground(UI::BACKGROUND_COLOR);
    RenderShader();
    ui.Update();
    // Update values from UI
    base_iter = static_cast<int>(ui.GetBaseIterValue());
    bailout_power = static_cast<int>(ui.GetBailoutPowerValue());
    full_color_palette = ui.GetFullColorPaletteValue();
    window.EndDrawing();
}
