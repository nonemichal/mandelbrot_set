#include <string>
#include <string_view>

#include "raylib-cpp.hpp"

#include "app.hpp"

#define TITLE "Mandelbrot Set"
#define CONFIG_FILE "config.toml"
#define CONFIG_PATH                                                            \
    PROJECT_ROOT_PATH "/" CONFIG_FILE  // PROJECT_ROOT_PATH is defined in cmake

constexpr std::string TITLE_STR{
    TITLE};  //  NOTE: Raylib window requires title as string
constexpr std::string_view CONFIG_PATH_SV{
    CONFIG_PATH};  // Config path as string_view

int main() {
    // Create the app instance
    auto app_result = App::New(TITLE_STR, CONFIG_PATH_SV);
    if (!app_result) {
        const auto *error_msg = app_result.error().c_str();
        TraceLog(LOG_ERROR,
                 "MANDELBROT_SET: Failed to create the app instance -> %s",
                 error_msg);
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
