#include <string>
#include <string_view>

#include "raylib-cpp.hpp"

#include "app.hpp"

constexpr std::string TITLE{
    "Mandelbrot Set"};  //  NOTE: Raylib window requires title as string
constexpr std::string_view CONFIG_FILE{
    "config.toml"};  // Config file path as string_view

int main() {
    // Create the app instance
    auto app_result = App::Instance(TITLE, CONFIG_FILE);
    if (!app_result) {
        const auto &error = app_result.error();
        TraceLog(LOG_ERROR, "MANDELBROT_SET: [%s] %s",
                 error.GetCodeString().data(), error.GetMessage().c_str());
        return 1;
    }

    // Creating succeeded
    auto &app = app_result.value();

    // Run the app
    TraceLog(LOG_INFO, "MANDELBROT_SET: Running the app");
    app->Run();

    return 0;
}
