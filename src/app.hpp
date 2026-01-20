#pragma once

#include <mandelbrot_error.hpp>
#include <string_view>

#include "raylib-cpp.hpp"

#include "config.hpp"
#include "rgb.hpp"

// Class to run the app
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
    int fps_;
    raylib::Window window_;
    raylib::Shader shader_;
    // Texture
    raylib::RenderTexture render_texture_;
    raylib::Texture texture_;

    void PrepareTexture();
    void Draw();

    // Color palette
    static constexpr std::size_t PALETTE_SIZE = 1024;
    static constexpr auto HSV_PALETTE = RGB::GenPaletteHSV<PALETTE_SIZE>();
    std::array<raylib::Color, PALETTE_SIZE> color_palette_;
    raylib::Texture palette_texture_;
};
