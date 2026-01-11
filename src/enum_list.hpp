#pragma once

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
    X(FileNotFound, "FileNotFound") /* Referenced file does not exist */       \
    X(ParseError, "ParseError") /* Configuration file could not be parsed */   \
    X(MissingOption,                                                           \
      "MissingOption") /* Required configuration option is missing */          \
    X(InvalidValue,                                                            \
      "InvalidValue") /* Configuration value is outside the allowed range */

// Macro used to count number of elements in a list
// Expands each element to +1, sum gives total count
#define X_ENUM_COUNT(name, str) +1
