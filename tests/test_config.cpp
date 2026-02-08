#include <filesystem>
#include <string_view>

#include "doctest.h"

#include "config.hpp"

static std::filesystem::path CreateConfigPath(std::string_view config_file) {
    std::filesystem::path config_path =
        std::filesystem::path(PROJECT_ROOT_PATH) / config_file;
    return config_path;
}

TEST_CASE("01 - Config::Load - valid config loads correctly") {
    SUBCASE("Valid config") {
        auto config_path = CreateConfigPath("tests/configs/config_valid1.toml");
        auto result = Config::Load(config_path);

        REQUIRE(result.has_value());

        const auto &config = result.value();

        CHECK_EQ(config.GetWindowValue(Config::WindowOption::Width), 1280);
        CHECK_EQ(config.GetWindowValue(Config::WindowOption::Height), 720);
        CHECK_EQ(config.GetWindowValue(Config::WindowOption::Fps), 60);

        CHECK(config.GetShaderPath(Config::ShaderType::Vertex).empty());

        const auto expected_path = std::filesystem::canonical(
            std::filesystem::path(PROJECT_ROOT_PATH) /
            "tests/configs/shader_valid.frag");
        const auto actual_path = std::filesystem::canonical(
            config.GetShaderPath(Config::ShaderType::Fragment));

        CHECK_EQ(actual_path, expected_path);

        CHECK_EQ(config.GetRenderValue(Config::RenderOption::BaseIter), 500);
    }
    SUBCASE("Valid Config with additional settings") {
        auto config_path = CreateConfigPath("tests/configs/config_valid2.toml");
        auto result = Config::Load(config_path);

        REQUIRE(result.has_value());

        const auto &config = result.value();

        CHECK_EQ(config.GetWindowValue(Config::WindowOption::Width), 1280);
        CHECK_EQ(config.GetWindowValue(Config::WindowOption::Height), 720);
        CHECK_EQ(config.GetWindowValue(Config::WindowOption::Fps), 60);

        CHECK(config.GetShaderPath(Config::ShaderType::Vertex).empty());

        const auto expected_path = std::filesystem::canonical(
            std::filesystem::path(PROJECT_ROOT_PATH) /
            "tests/configs/shader_valid.frag");
        const auto actual_path = std::filesystem::canonical(
            config.GetShaderPath(Config::ShaderType::Fragment));

        CHECK_EQ(actual_path, expected_path);

        CHECK_EQ(config.GetRenderValue(Config::RenderOption::BaseIter), 500);
    }
}

TEST_CASE("02 - Config::Load - missing config file") {
    auto config_path = CreateConfigPath("tests/configs/does_not_exist.toml");
    auto result = Config::Load(config_path);

    REQUIRE_FALSE(result.has_value());

    const auto &error = result.error();
    CHECK_EQ(error.GetCode(), MandelbrotError::Code::FileNotFound);
    MESSAGE(error.GetMessage());
}

TEST_CASE("03 - Config::Load - config file not parsable") {
    auto config_path = CreateConfigPath("tests/configs/config_no_parse.toml");
    auto result = Config::Load(config_path);

    REQUIRE_FALSE(result.has_value());

    const auto &error = result.error();
    CHECK_EQ(error.GetCode(), MandelbrotError::Code::ParseError);
    MESSAGE(error.GetMessage());
}

TEST_CASE("04 - Config::Load - missing window section") {
    auto config_path = CreateConfigPath("tests/configs/config_no_window.toml");
    auto result = Config::Load(config_path);

    REQUIRE_FALSE(result.has_value());

    const auto &error = result.error();
    CHECK_EQ(error.GetCode(), MandelbrotError::Code::ParseError);
    MESSAGE(error.GetMessage());
}

TEST_CASE("05 - Config::Load - missing shader section") {
    auto config_path = CreateConfigPath("tests/configs/config_no_shader.toml");
    auto result = Config::Load(config_path);

    REQUIRE_FALSE(result.has_value());

    const auto &error = result.error();
    CHECK_EQ(error.GetCode(), MandelbrotError::Code::ParseError);
    MESSAGE(error.GetMessage());
}

TEST_CASE("06 - Config::Load - missing render section") {
    auto config_path = CreateConfigPath("tests/configs/config_no_render.toml");
    auto result = Config::Load(config_path);

    REQUIRE_FALSE(result.has_value());

    const auto &error = result.error();
    CHECK_EQ(error.GetCode(), MandelbrotError::Code::ParseError);
    MESSAGE(error.GetMessage());
}

TEST_CASE("07 - Config::Load - invalid config layout") {
    SUBCASE("Wrong table names") {
        auto config_path =
            CreateConfigPath("tests/configs/config_invalid_layout1.toml");
        auto result = Config::Load(config_path);

        REQUIRE_FALSE(result.has_value());

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::ParseError);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Nested tables") {
        auto config_path =
            CreateConfigPath("tests/configs/config_invalid_layout2.toml");
        auto result = Config::Load(config_path);

        REQUIRE_FALSE(result.has_value());

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::MissingOption);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Window as array of tables") {
        auto config_path =
            CreateConfigPath("tests/configs/config_invalid_layout3.toml");
        auto result = Config::Load(config_path);

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::ParseError);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Shaders as array of tables") {
        auto config_path =
            CreateConfigPath("tests/configs/config_invalid_layout4.toml");
        auto result = Config::Load(config_path);

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::ParseError);
        MESSAGE(error.GetMessage());
    }
}

TEST_CASE("08 - Config::Load - invalid shaders config") {
    SUBCASE("Invalid fragment path") {
        auto config_path =
            CreateConfigPath("tests/configs/config_invalid_shaders1.toml");
        auto result = Config::Load(config_path);

        REQUIRE_FALSE(result.has_value());

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::FileNotFound);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Fragment not a path") {
        auto config_path =
            CreateConfigPath("tests/configs/config_invalid_shaders2.toml");
        auto result = Config::Load(config_path);

        REQUIRE_FALSE(result.has_value());

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Wrong shader options") {
        auto config_path =
            CreateConfigPath("tests/configs/config_invalid_shaders3.toml");
        auto result = Config::Load(config_path);

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::MissingOption);
        MESSAGE(error.GetMessage());
    }
}

TEST_CASE("09 - Config::Load - invalid window config") {
    SUBCASE("Width too large") {
        auto config_path =
            CreateConfigPath("tests/configs/config_invalid_window1.toml");
        auto result = Config::Load(config_path);

        REQUIRE_FALSE(result.has_value());

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Height too large") {
        auto config_path =
            CreateConfigPath("tests/configs/config_invalid_window2.toml");
        auto result = Config::Load(config_path);

        REQUIRE_FALSE(result.has_value());

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("FPS too high") {
        auto config_path =
            CreateConfigPath("tests/configs/config_invalid_window3.toml");
        auto result = Config::Load(config_path);

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Width too small") {
        auto config_path =
            CreateConfigPath("tests/configs/config_invalid_window4.toml");
        auto result = Config::Load(config_path);

        REQUIRE_FALSE(result.has_value());

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Height too small") {
        auto config_path =
            CreateConfigPath("tests/configs/config_invalid_window5.toml");
        auto result = Config::Load(config_path);

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("FPS too low") {
        auto config_path =
            CreateConfigPath("tests/configs/config_invalid_window6.toml");
        auto result = Config::Load(config_path);

        REQUIRE_FALSE(result.has_value());

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Width as float") {
        auto config_path =
            CreateConfigPath("tests/configs/config_invalid_window7.toml");
        auto result = Config::Load(config_path);

        REQUIRE_FALSE(result.has_value());

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Height as float") {
        auto config_path =
            CreateConfigPath("tests/configs/config_invalid_window8.toml");
        auto result = Config::Load(config_path);

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("FPS as float") {
        auto config_path =
            CreateConfigPath("tests/configs/config_invalid_window9.toml");
        auto result = Config::Load(config_path);

        REQUIRE_FALSE(result.has_value());

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Width as string") {
        auto config_path =
            CreateConfigPath("tests/configs/config_invalid_window10.toml");
        auto result = Config::Load(config_path);

        REQUIRE_FALSE(result.has_value());

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Height as string") {
        auto config_path =
            CreateConfigPath("tests/configs/config_invalid_window11.toml");
        auto result = Config::Load(config_path);

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("FPS as string") {
        auto config_path =
            CreateConfigPath("tests/configs/config_invalid_window12.toml");
        auto result = Config::Load(config_path);

        REQUIRE_FALSE(result.has_value());

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Wrong window options") {
        auto config_path =
            CreateConfigPath("tests/configs/config_invalid_window13.toml");
        auto result = Config::Load(config_path);

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::MissingOption);
        MESSAGE(error.GetMessage());
    }
}

TEST_CASE("10 - Config::Load - invalid render config") {
    SUBCASE("Base iter value not positive") {
        auto config_path =
            CreateConfigPath("tests/configs/config_invalid_render1.toml");
        auto result = Config::Load(config_path);

        REQUIRE_FALSE(result.has_value());

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Base iter value not an int") {
        auto config_path =
            CreateConfigPath("tests/configs/config_invalid_render2.toml");
        auto result = Config::Load(config_path);

        REQUIRE_FALSE(result.has_value());

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Wrong render options") {
        auto config_path =
            CreateConfigPath("tests/configs/config_invalid_render3.toml");
        auto result = Config::Load(config_path);

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::MissingOption);
        MESSAGE(error.GetMessage());
    }
}
