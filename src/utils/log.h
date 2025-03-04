#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

typedef enum {
    OFF = 0,
    FATAL,
    ERROR,
    WARN,
    INFO,
    DEBUG,
    TRACE
} LOG_LEVEL;

[[ maybe_unused ]] static LOG_LEVEL CURRENT_LOG_LEVEL = INFO;

#define SET_LOG_LEVEL(level) (CURRENT_LOG_LEVEL = level)

static const char *LOG_LEVEL_NAMES[] = {"OFF", "FATAL", "ERROR", "WARN", "INFO", "DEBUG", "TRACE"};

#define LOG(level, format, ...) \
    { \
        if (level <= CURRENT_LOG_LEVEL && level != OFF) { \
            log_message(level, __FILE__, __LINE__, format, ##__VA_ARGS__); \
        } \
    }

#define LOG_FATAL(format, ...) LOG(FATAL, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) LOG(ERROR, format, ##__VA_ARGS__)
#define LOG_WARN(format, ...)  LOG(WARN, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...)  LOG(INFO, format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) LOG(DEBUG, format, ##__VA_ARGS__)
#define LOG_TRACE(format, ...) LOG(TRACE, format, ##__VA_ARGS__)

static inline void log_message(LOG_LEVEL level, const char *file, int line, const char *format, ...) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char time_buf[20];
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);

    fprintf(stderr, "[%s] %s (%s:%d): ", time_buf, LOG_LEVEL_NAMES[level], file, line);
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
}

