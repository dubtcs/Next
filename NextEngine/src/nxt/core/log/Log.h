#pragma once

#include <spdlog/spdlog.h>

namespace nxt::log
{

	void Init();
	
	std::shared_ptr<spdlog::logger> GetEngineLog();
	std::shared_ptr<spdlog::logger> GetAppLog();

}

// ???
#ifndef NXT_LOGGING_ENABLED
	#if defined(NXT_DEBUG) || defined(NXT_RELEASE)
		#define NXT_LOGGING_ENABLED
	#endif
#endif

#ifdef NXT_LOGGING_ENABLED
	#ifdef NXT_BUILD_ENGINE
		#define NXT_LOG_TRACE(...) nxt::log::GetEngineLog()->log(spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, spdlog::level::trace, __VA_ARGS__)
		#define NXT_LOG_DEBUG(...) nxt::log::GetEngineLog()->log(spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, spdlog::level::debug, __VA_ARGS__)
		#define NXT_LOG_INFO(...) nxt::log::GetEngineLog()->log(spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, spdlog::level::info, __VA_ARGS__)
		#define NXT_LOG_WARN(...) nxt::log::GetEngineLog()->log(spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, spdlog::level::warn, __VA_ARGS__)
		#define NXT_LOG_CRIT(...) nxt::log::GetEngineLog()->log(spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, spdlog::level::critical, __VA_ARGS__)
		#ifdef NXT_ENABLE_ASSERTS
			#define NXT_ASSERT(conditional, ...) if(!(conditional)) { NXT_LOG_CRIT(__VA_ARGS__); std::exit(EXIT_FAILURE); }
			// could probably use quick_exit too
		#else
			#define NXT_ASSERT(...)
		#endif
	#else
		#define NXT_LOG_TRACE(...) nxt::log::GetAppLog()->log(spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, spdlog::level::trace, __VA_ARGS__)
		#define NXT_LOG_DEBUG(...) nxt::log::GetAppLog()->log(spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, spdlog::level::debug, __VA_ARGS__)
		#define NXT_LOG_INFO(...) nxt::log::GetAppLog()->log(spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, spdlog::level::info, __VA_ARGS__)
		#define NXT_LOG_WARN(...) nxt::log::GetAppLog()->log(spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, spdlog::level::warn, __VA_ARGS__)
		#define NXT_LOG_CRIT(...) nxt::log::GetAppLog()->log(spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, spdlog::level::critical, __VA_ARGS__)
		#define NXT_ASSERT(...)
	#endif
#endif
