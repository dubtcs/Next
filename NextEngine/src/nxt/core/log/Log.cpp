
#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

static constexpr auto GEngineLogName{ "Engine" };
static constexpr auto GAppLogName{ "App" };

namespace nxt::log
{

	void Init()
	{
		spdlog::set_pattern("[%T] [%n] [Thread %t]: %v%$");
		std::shared_ptr<spdlog::logger> engineLog{ spdlog::stdout_color_mt(GEngineLogName) };
		engineLog->set_level(spdlog::level::trace);
		std::shared_ptr<spdlog::logger> appLog{ spdlog::stdout_color_mt(GAppLogName) };
		appLog->set_level(spdlog::level::trace);
	}

	std::shared_ptr<spdlog::logger> GetEngineLog()
	{
		return spdlog::get(GEngineLogName);
	}

	std::shared_ptr<spdlog::logger> GetAppLog()
	{
		return spdlog::get(GAppLogName);
	}

}
