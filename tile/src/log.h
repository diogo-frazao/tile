#pragma once

#include <stdio.h>

// Disable for distribution builds
#define DEBUG_ENABLED 1

enum TextColor
{
    TEXT_COLOR_BLACK,
    TEXT_COLOR_RED,
    TEXT_COLOR_GREEN,
    TEXT_COLOR_YELLOW,
    TEXT_COLOR_BLUE,
    TEXT_COLOR_MAGENTA,
    TEXT_COLOR_CYAN,
    TEXT_COLOR_WHITE,
    TEXT_COLOR_BRIGHT_BLACK,
    TEXT_COLOR_BRIGHT_RED,
    TEXT_COLOR_BRIGHT_GREEN,
    TEXT_COLOR_BRIGHT_YELLOW,
    TEXT_COLOR_BRIGHT_BLUE,
    TEXT_COLOR_BRIGHT_MAGENTA,
    TEXT_COLOR_BRIGHT_CYAN,
    TEXT_COLOR_BRIGHT_WHITE,
    TEXT_COLOR_COUNT
};

enum LogType
{
    MINI,
    LOG,
    WARNING,
    ERROR
};

template <typename ...Args>
void _log(const LogType logType, const char* msg, Args... args)
{
    static const char* TextColorTable[TEXT_COLOR_COUNT] =
    {
      "\x1b[30m", // TEXT_COLOR_BLACK
      "\x1b[31m", // TEXT_COLOR_RED
      "\x1b[32m", // TEXT_COLOR_GREEN
      "\x1b[33m", // TEXT_COLOR_YELLOW
      "\x1b[34m", // TEXT_COLOR_BLUE
      "\x1b[35m", // TEXT_COLOR_MAGENTA
      "\x1b[36m", // TEXT_COLOR_CYAN
      "\x1b[37m", // TEXT_COLOR_WHITE
      "\x1b[90m", // TEXT_COLOR_BRIGHT_BLACK
      "\x1b[91m", // TEXT_COLOR_BRIGHT_RED
      "\x1b[92m", // TEXT_COLOR_BRIGHT_GREEN
      "\x1b[93m", // TEXT_COLOR_BRIGHT_YELLOW
      "\x1b[94m", // TEXT_COLOR_BRIGHT_BLUE
      "\x1b[95m", // TEXT_COLOR_BRIGHT_MAGENTA
      "\x1b[96m", // TEXT_COLOR_BRIGHT_CYAN
      "\x1b[97m", // TEXT_COLOR_BRIGHT_WHITE
    };

    const char* prefix;
    TextColor textColor;

    switch (logType)
    {
        case MINI:
        {
            prefix = "MINI:";
            textColor = TEXT_COLOR_WHITE;
            break;
        }
        case LOG:
        {
            prefix = "LOG:";
            textColor = TEXT_COLOR_GREEN;
            break;
        }
        case WARNING:
        {
            prefix = "WARNING:";
            textColor = TEXT_COLOR_YELLOW;
            break;
        }
        case ERROR:
        {
            prefix = "ERROR";
            textColor = TEXT_COLOR_RED;
            break;
        }
        default:
        {
            __debugbreak();
            return;
        }
    }

    // Init color, prefix, message and close color
    char formatBuffer[8192]{};
    sprintf_s(formatBuffer, sizeof(formatBuffer), "%s %s %s \033[0m", TextColorTable[textColor], prefix, msg);
    // Add arguments to the end of the string
    char textBuffer[8912]{};
    sprintf_s(textBuffer, sizeof(formatBuffer), formatBuffer, args...);
    // Send buffer to console
    puts(textBuffer);
}

#if DEBUG_ENABLED
#define D_LOG(logType, msg, ...) _log(logType, ##__VA_ARGS__);
#define D_ASSERT(x, msg, ...)           \
{                                       \
    if (!x)                             \
    {                                   \
        D_LOG(ERROR, msg, ##__VA_ARGS__);    \
        __debugbreak();                 \
    }                                   \
}                                       \

#endif