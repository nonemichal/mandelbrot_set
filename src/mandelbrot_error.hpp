#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <string_view>

// Represents an error that can occur in the application
// Designed to be used as the error type for std::expected<T, MandelbrotError>
class MandelbrotError {
  public:
    enum class Code : std::uint8_t {
        SingletonAlreadyExists,  // Attempted to create a second instance of a
                                 // singleton
        FileNotFound,            // Referenced file does not exist
        ParseError,              // Configuration file could not be parsed
        MissingOption,           // Required configuration option is missing
        InvalidValue,  // Configuration value is outside the allowed range
        COUNT_,        // Number of errors
    };

    // Constructs an error with a specific code and message
    explicit MandelbrotError(Code code, std::string message)
        : code_(code), message_(std::move(message)) {}

    // Getters
    [[nodiscard]] Code GetCode() const noexcept { return code_; }
    [[nodiscard]] std::string_view GetCodeString() const noexcept {
        auto index = static_cast<size_t>(code_);
        return ERROR_CODES_STR.at(index);
    }
    [[nodiscard]] const std::string &GetMessage() const noexcept {
        return message_;
    }

  private:
    Code code_;
    std::string message_;

    // Numbers of error codes
    static constexpr size_t ERROR_CODES_COUNT{
        static_cast<size_t>(Code::COUNT_)};

    // Error codes as strings
    // NOTE: The order must exactly match the order of the enum
    static constexpr std::array<std::string_view, ERROR_CODES_COUNT>
        ERROR_CODES_STR{"SingletonAlreadyExists", "FileNotFound", "ParseError",
                        "MissingOption", "InvalidValue"};
};
