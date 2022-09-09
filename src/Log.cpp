#include "Log.h"

namespace logger {
    inline bool IsDebug() {
#ifdef _DEBUG
        return true;
#else
        return false;
#endif
    }

    void InitializeLogging() {
        auto log = std::make_shared<spdlog::logger>(
            "Global", std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        if (IsDebug()) {
            log->set_level(spdlog::level::debug);
        } else {
            log->set_level(spdlog::level::info);
        }
        log->flush_on(spdlog::level::level_enum::trace);

        spdlog::set_default_logger(std::move(log));
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%t] [%s:%#] %v");
    }
}