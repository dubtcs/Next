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
			#elif defined(NXT_DLL_IMPORT)
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

/*
No bits?
*insert megamind no-bitches? meme*

* This will self implode if someone flags this
* so I just made it default
* It made me laugh too hard to remove :)
*/
//#ifndef NXT_NO_BITS
	#define BIT(x) 1 << x
	#define SETBIT(value, index) value << index
	#define CHECKBIT(value, bit) ((value >> bit) & 1U) == 1
//#endif

using nxt_word = uint16_t; // 16 bit unsigned
using nxt_dword = uint32_t; // 32 bit unsigned

#define NXT_WORD_MASK (0xffff)

// Grab the first 16 bits of the variable
#define NXT_LO(x) (((nxt_word)x) & NXT_WORD_MASK)
#define NXT_HI(x) ((nxt_word)((x >> 16) & NXT_WORD_MASK))

// Create a dword from two raw values
#define NXT_MAKE_DWORD(lo, hi) ((NXT_LO(lo)) | (NXT_LO(hi) << 16))

// Use for binding event callbacks
#define NXT_CALLBACK(function_name) [this](auto&&... arg) { return this->function_name(std::forward<decltype(arg)>(arg)...); }
#define NXT_CALLBACK_STATIC(function_name) [](auto&&... arg) { return function_name(std::forward<decltype(arg)>(arg)...); }

// Deprecated
//#define NXT_BIND_FN(x) std::bind(&x, this, std::placeholders::_1)

// NXT Namespace
#define NXT_NS nxt

// Logging if you want that
#define NXT_ENGINE_HEADER_INCLUDE_LOG
#ifdef NXT_ENGINE_HEADER_INCLUDE_LOG
#include <nxt/core/log/Log.h>
#endif

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
