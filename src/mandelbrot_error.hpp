#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <string_view>

#include "enum_list.hpp"

// Represents an error that can occur in the application
class MandelbrotError {
  public:
    enum class Code : std::uint8_t {
#define X(name, str) name,
        ERROR_CODE_LIST(X)
#undef X
    };

    explicit MandelbrotError(Code code, std::string message)
        : code(code), message(std::move(message)) {}

    // Getters
    [[nodiscard]] Code GetCode() const noexcept { return code; }
    [[nodiscard]] std::string_view GetCodeString() const noexcept {
        auto index = static_cast<size_t>(code);
        return ERROR_CODES_STR.at(index);
    }
    [[nodiscard]] const std::string &GetMessage() const noexcept {
        return message;
    }

  private:
    Code code;
    std::string message;

    // Numbers of error codes
    static constexpr size_t ERROR_CODES_COUNT{0 ERROR_CODE_LIST(X_ENUM_COUNT)};

    // Error codes as strings
    static constexpr std::array<std::string_view, ERROR_CODES_COUNT>
        ERROR_CODES_STR{
#define X(name, str) str,
            ERROR_CODE_LIST(X)
#undef X
        };
};
