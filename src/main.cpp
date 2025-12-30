#include <string>
#include <string_view>

#include "raylib-cpp.hpp"

#include "app.hpp"
#include "mandelbrot_error.hpp"

#define TITLE "Mandelbrot Set"
#define CONFIG_FILE "config.toml"

constexpr std::string TITLE_STR{
    TITLE};  //  NOTE: Raylib window requires title as string
constexpr std::string_view CONFIG_FILE_SV{
    CONFIG_FILE};  // Config path as string_view

int main() {
    // Create the app instance
    auto app_result = App::New(TITLE_STR, CONFIG_FILE_SV);
    if (!app_result) {
        const auto &error = app_result.error();
        TraceLog(LOG_ERROR, "MANDELBROT_SET: [%s] %s", error.GetCodeString(),
                 error.GetMessage().data());
        return 1;
    }
    TraceLog(LOG_INFO, "MANDELBROT_SET: The app instance created correctly");

    // Creating succeeded
    auto &app = app_result.value();

    // Run the app
    TraceLog(LOG_INFO, "MANDELBROT_SET: Running the app");
    app->Run();

    return 0;
}
