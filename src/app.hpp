#pragma once

#include <cmath>
#include <string_view>

#include "raylib-cpp.hpp"
#include "raylib.h"

#include "RenderTexture.hpp"
#include "Window.hpp"
#include "config.hpp"
#include "mandelbrot_error.hpp"
#include "rgb.hpp"
#include "ui.hpp"

class App {
  public:
    explicit App(const std::string &title, const Config &config);

    // Return the application singleton instance, initializing it on first use
    // On failure, return an error
    static std::expected<App *, MandelbrotError>
    Instance(const std::string &title, std::string_view config_file);

    // Delete copy operations
    App(const App &) = delete;
    App &operator=(const App &) = delete;

    // Delete move operations
    App(App &&) noexcept = delete;
    App &operator=(App &&) = delete;

    ~App() = default;

    // Run the app
    void Run();

  private:
    // Window
    int fps;
    raylib::Window window;
    raylib::Shader shader;
    // Texture
    raylib::RenderTexture render_texture;
    raylib::Texture texture;
    // UI section
    UI ui;
    // Render
    std::size_t render_width;
    std::size_t render_height;
    int base_iter;
    int bailout_power;
    float color_detail = 1.0F;
    bool full_color_palette;

    // Color palette
    const std::array<RGB, Config::COLOR_PALETTE_SIZE> GENERATED_PALETTE{
        RGB::GenPaletteLCH<Config::COLOR_PALETTE_SIZE>()};
    std::array<raylib::Color, Config::COLOR_PALETTE_SIZE> color_palette;
    raylib::Texture palette_texture;

    void PrepareTexture();
    void RenderShader();
    void Draw();
};
