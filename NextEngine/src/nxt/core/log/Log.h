#pragma once

#include <spdlog/spdlog.h>

namespace nxt::log
{

	void Init();
	
	std::shared_ptr<spdlog::logger> GetEngineLog();
	std::shared_ptr<spdlog::logger> GetAppLog();

}

#ifdef NXT_BUILD_ENGINE
#define NXT_LOG_TRACE(...) nxt::log::GetEngineLog()->trace(__VA_ARGS__)
#define NXT_LOG_INFO(...) nxt::log::GetEngineLog()->info(__VA_ARGS__)
#define NXT_LOG_WARN(...) nxt::log::GetEngineLog()->warn(__VA_ARGS__)
#define NXT_LOG_CRIT(...) nxt::log::GetEngineLog()->critical(__VA_ARGS__)
#else
#define NXT_LOG_TRACE(...) nxt::log::GetAppLog()->trace(__VA_ARGS__)
#define NXT_LOG_INFO(...) nxt::log::GetAppLog()->info(__VA_ARGS__)
#define NXT_LOG_WARN(...) nxt::log::GetAppLog()->warn(__VA_ARGS__)
#define NXT_LOG_CRIT(...) nxt::log::GetAppLog()->critical(__VA_ARGS__)
#endif
