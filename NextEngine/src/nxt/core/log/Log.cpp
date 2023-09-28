
#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

static constexpr auto GEngineLogName{ "Engine" };
static constexpr auto GAppLogName{ "App" };

#ifdef NXT_LOGGING_ENABLED
static std::shared_ptr<spdlog::logger> engineLog{ spdlog::stdout_color_mt(GEngineLogName) };
static std::shared_ptr<spdlog::logger> appLog{ spdlog::stdout_color_mt(GAppLogName) };
#endif

namespace nxt::log
{
#ifdef NXT_LOGGING_ENABLED
	void Init()
	{
		spdlog::set_pattern("[%T] [%n] [%!] [Thread %t]: %v%$");
		engineLog->set_level(spdlog::level::trace);
		appLog->set_level(spdlog::level::trace);
	}

	std::shared_ptr<spdlog::logger> GetEngineLog()
	{
		return engineLog;
	}

	std::shared_ptr<spdlog::logger> GetAppLog()
	{
		return appLog;
	}
#else
	void Init() {}
	std::shared_ptr<spdlog::logger> GetEngineLog() { return nullptr; }
	std::shared_ptr<spdlog::logger> GetAppLog() { return nullptr; }
#endif
}
