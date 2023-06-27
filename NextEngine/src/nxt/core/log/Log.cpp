
#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

static constexpr auto GEngineLogName{ "Engine" };
static constexpr auto GAppLogName{ "App" };

static std::shared_ptr<spdlog::logger> engineLog;
static std::shared_ptr<spdlog::logger> appLog;

namespace nxt::log
{

	void Init()
	{
		spdlog::set_pattern("[%T] [%n] [%!] [Thread %t]: %v%$");
		engineLog = spdlog::stdout_color_mt(GEngineLogName);
		engineLog->set_level(spdlog::level::trace);
		appLog = spdlog::stdout_color_mt(GAppLogName);
		appLog->set_level(spdlog::level::trace);
	}

	std::shared_ptr<spdlog::logger> GetEngineLog()
	{
		return engineLog;//spdlog::get(GEngineLogName);
	}

	std::shared_ptr<spdlog::logger> GetAppLog()
	{
		return appLog;//spdlog::get(GAppLogName);
	}

}
