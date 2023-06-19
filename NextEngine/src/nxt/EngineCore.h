#pragma once

#ifdef _WIN32
	#ifdef _WIN64
		#define NXT_PLATFORM_WINDOWS

		#ifndef NOMINMAX
			#define NOMINMAX
		#endif
		#include <Windows.h>

		#ifdef NXT_AS_DLL
			#ifdef NXT_DLL_EXPORT
				#define NXT_API __declspec(dllexport)
			#elif
				#define NXT_API __declspec(dllimport)
			#else
				#error "Flagged as DLL but no DLL import/export flags found"
			#endif
		#else
			#define NXT_API
		#endif

	#else
		#error "Only 64 bit supported"
	#endif
#else
	#error "Only windows supported"
#endif

#include <memory>

#define BIT(x) 1 << x

// types
#ifdef NXT_DEFINE_TYPES
#define I32 int32_t
#define U32 uint32_t
#define F32 float
#endif

// Use for binding event callbacks
#define NXT_CALLBACK(function_name) [this](auto&&... arg) { return this->function_name(std::forward<decltype(arg)>(arg)...); }

// Deprecated
//#define NXT_BIND_FN(x) std::bind(&x, this, std::placeholders::_1)

// NXT Namespace
#define NXT_NS nxt

namespace nxt
{

	template<typename T>
	using Unique = std::unique_ptr<T>;

	template<typename T, typename ...A>
	constexpr Unique<T> NewUnique(A&&... a)
	{
		return std::make_unique<T>(std::forward<A>(a)...);
	}

	template<typename T>
	using Shared = std::shared_ptr<T>;

	template<typename T, typename ...A>
	constexpr Shared<T> NewShared(A&&... a)
	{
		return std::make_shared<T>(std::forward<A>(a)...);
	}

}
