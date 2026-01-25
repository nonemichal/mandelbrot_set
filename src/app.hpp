#pragma once

#include <string_view>

#include "raylib-cpp.hpp"

#include "config.hpp"
#include "mandelbrot_error.hpp"
#include "rgb.hpp"

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
    int fps;
    raylib::Window window;
    raylib::Shader shader;
    // Texture
    raylib::RenderTexture render_texture;
    raylib::Texture texture;

    void PrepareTexture();
    void Draw();

    // Color palette
    static constexpr std::size_t PALETTE_SIZE = 1024;
    static constexpr auto HSV_PALETTE = RGB::GenPaletteHSV<PALETTE_SIZE>();
    std::array<raylib::Color, PALETTE_SIZE> color_palette;
    raylib::Texture palette_texture;
};
