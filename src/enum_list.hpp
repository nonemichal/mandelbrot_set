#pragma once

// This file contains macros for all enums that also have string
// representations, as well as a macro for calculating the number of elements in
// an enum

// Macro defining all window options
#define WINDOW_OPTION_LIST(X)                                                  \
    X(Width, "width")                                                          \
    X(Height, "height")                                                        \
    X(Fps, "fps")

// Macro defining all shader types
#define SHADER_TYPE_LIST(X)                                                    \
    X(Vertex, "vertex")                                                        \
    X(Fragment, "fragment")

// Macro defining all error codes types
#define ERROR_CODE_LIST(X)                                                     \
    /* Referenced file does not exist */                                       \
    X(FileNotFound, "FileNotFound")                                            \
    /* Configuration file could not be parsed */                               \
    X(ParseError, "ParseError")                                                \
    /* Required configuration option is missing */                             \
    X(MissingOption, "MissingOption")                                          \
    /* Configuration value is outside the allowed range */                     \
    X(InvalidValue, "InvalidValue")

// Macro used to count number of elements in a list
// NOTE: Expands each element to +1, sum gives total count
#define X_ENUM_COUNT(name, str) +1
