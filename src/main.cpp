#include <string>

#include "raylib-cpp.hpp"

#include "app.hpp"

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;
constexpr std::string TITLE = "Mandelbrot Set";
constexpr int FPS = 60;

int main() {
    // Initialization
    App app(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
    // Run the app
    app.Run(FPS);

    return 0;
}
