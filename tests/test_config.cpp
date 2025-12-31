#include "doctest.h"

#include "config.hpp"

TEST_CASE("01 - Config::Load - valid config loads correctly") {
    SUBCASE("Valid config") {
        auto result = Config::Load("tests/configs/config_valid1.toml");

        REQUIRE(result.has_value());

        const auto &config = result.value();

        CHECK_GT(config.GetWindowValue(Config::WindowOption::Width), 0);
        CHECK_GT(config.GetWindowValue(Config::WindowOption::Height), 0);
        CHECK_GT(config.GetWindowValue(Config::WindowOption::Fps), 0);

        CHECK(config.GetShaderPath(Config::ShaderType::Vertex).empty());
        CHECK_FALSE(config.GetShaderPath(Config::ShaderType::Fragment).empty());
    }
    SUBCASE("Valid Config with additional settings") {
        auto result = Config::Load("tests/configs/config_valid2.toml");

        REQUIRE(result.has_value());

        const auto &config = result.value();

        CHECK_GT(config.GetWindowValue(Config::WindowOption::Width), 0);
        CHECK_GT(config.GetWindowValue(Config::WindowOption::Height), 0);
        CHECK_GT(config.GetWindowValue(Config::WindowOption::Fps), 0);

        CHECK(config.GetShaderPath(Config::ShaderType::Vertex).empty());
        CHECK_FALSE(config.GetShaderPath(Config::ShaderType::Fragment).empty());
    }
}

TEST_CASE("02 - Config::Load - missing config file") {
    auto result = Config::Load("tests/configs/does_not_exist.toml");

    REQUIRE_FALSE(result.has_value());

    const auto &error = result.error();
    CHECK_EQ(error.GetCode(), MandelbrotError::Code::FileNotFound);
    MESSAGE(error.GetMessage());
}

TEST_CASE("03 - Config::Load - config file not parsable") {
    auto result = Config::Load("tests/configs/config_no_parse.toml");

    REQUIRE_FALSE(result.has_value());

    const auto &error = result.error();
    CHECK_EQ(error.GetCode(), MandelbrotError::Code::ParseError);
    MESSAGE(error.GetMessage());
}

TEST_CASE("04 - Config::Load - missing window section") {
    auto result = Config::Load("tests/configs/config_no_window.toml");

    REQUIRE_FALSE(result.has_value());

    const auto &error = result.error();
    CHECK_EQ(error.GetCode(), MandelbrotError::Code::ParseError);
    MESSAGE(error.GetMessage());
}

TEST_CASE("05 - Config::Load - missing shader section") {
    auto result = Config::Load("tests/configs/config_no_shader.toml");

    REQUIRE_FALSE(result.has_value());

    const auto &error = result.error();
    CHECK_EQ(error.GetCode(), MandelbrotError::Code::ParseError);
    MESSAGE(error.GetMessage());
}

TEST_CASE("06 - Config::Load - invalid config layout") {
    SUBCASE("Wrong table names") {
        auto result = Config::Load("tests/configs/config_invalid_layout1.toml");

        REQUIRE_FALSE(result.has_value());

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::ParseError);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Nested tables") {
        auto result = Config::Load("tests/configs/config_invalid_layout2.toml");

        REQUIRE_FALSE(result.has_value());

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::MissingOption);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Window as array of tables") {
        auto result = Config::Load("tests/configs/config_invalid_layout3.toml");

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::ParseError);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Shaders as array of tables") {
        auto result = Config::Load("tests/configs/config_invalid_layout4.toml");

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::ParseError);
        MESSAGE(error.GetMessage());
    }
}

TEST_CASE("07 - Config::Load - invalid shaders config") {
    SUBCASE("Invalid fragment path") {
        auto result =
            Config::Load("tests/configs/config_invalid_shaders1.toml");

        REQUIRE_FALSE(result.has_value());

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::FileNotFound);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Fragment as int") {
        auto result =
            Config::Load("tests/configs/config_invalid_shaders2.toml");

        REQUIRE_FALSE(result.has_value());

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Wrong shaders options") {
        auto result =
            Config::Load("tests/configs/config_invalid_shaders3.toml");

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::MissingOption);
        MESSAGE(error.GetMessage());
    }
}

TEST_CASE("08 - Config::Load - invalid window config") {
    SUBCASE("Width too large") {
        auto result = Config::Load("tests/configs/config_invalid_window1.toml");

        REQUIRE_FALSE(result.has_value());

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Height too large") {
        auto result = Config::Load("tests/configs/config_invalid_window2.toml");

        REQUIRE_FALSE(result.has_value());

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("FPS too high") {
        auto result = Config::Load("tests/configs/config_invalid_window3.toml");

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Width too small") {
        auto result = Config::Load("tests/configs/config_invalid_window4.toml");

        REQUIRE_FALSE(result.has_value());

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Height too small") {
        auto result = Config::Load("tests/configs/config_invalid_window5.toml");

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("FPS too low") {
        auto result = Config::Load("tests/configs/config_invalid_window6.toml");

        REQUIRE_FALSE(result.has_value());

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Width as float") {
        auto result = Config::Load("tests/configs/config_invalid_window7.toml");

        REQUIRE_FALSE(result.has_value());

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Height as float") {
        auto result = Config::Load("tests/configs/config_invalid_window8.toml");

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("FPS as float") {
        auto result = Config::Load("tests/configs/config_invalid_window9.toml");

        REQUIRE_FALSE(result.has_value());

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Width as string") {
        auto result =
            Config::Load("tests/configs/config_invalid_window10.toml");

        REQUIRE_FALSE(result.has_value());

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Height as string") {
        auto result =
            Config::Load("tests/configs/config_invalid_window11.toml");

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("FPS as string") {
        auto result =
            Config::Load("tests/configs/config_invalid_window12.toml");

        REQUIRE_FALSE(result.has_value());

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::InvalidValue);
        MESSAGE(error.GetMessage());
    }
    SUBCASE("Wrong window options") {
        auto result =
            Config::Load("tests/configs/config_invalid_window13.toml");

        const auto &error = result.error();
        CHECK_EQ(error.GetCode(), MandelbrotError::Code::MissingOption);
        MESSAGE(error.GetMessage());
    }
}
