#pragma once

#include <string_view>

#include "raylib-cpp.hpp"

#include "config.hpp"

// Class to run the app
class App {
  public:
    // Returns the only one instance of the app
    static std::expected<App *, std::string> New(const std::string &title,
                                                 std::string_view config_path);

    // Delete copy operations
    App(const App &) = delete;
    App &operator=(const App &) = delete;

    // Delete move operations
    App(App &&) = delete;
    App &operator=(App &&) = delete;

    // Getters
    [[nodiscard]] const Config &GetConfig() const;
    [[nodiscard]] int GetFps() const;
    [[nodiscard]] const raylib::Window &GetWindow() const;

    // Method to run the app
    void Run();

  private:
    // Private constructor and destructor
    explicit App(const std::string &title, const Config &config);
    ~App() = default;

    int fps_;
    raylib::Window window_;
    raylib::Shader shader_;
};
