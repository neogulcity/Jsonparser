#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#define logger_critical(...) SPDLOG_CRITICAL(__VA_ARGS__)
#define logger_error(...) SPDLOG_ERROR(__VA_ARGS__)
#define logger_warn(...) SPDLOG_WARN(__VA_ARGS__)
#define logger_info(...) SPDLOG_INFO(__VA_ARGS__)
#define logger_debug(...) SPDLOG_DEBUG(__VA_ARGS__)
#define logger_trace(...) SPDLOG_TRACE(__VA_ARGS__)

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace logger {
    inline bool IsDebug();
    void InitializeLogging();
}