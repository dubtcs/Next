#pragma once

#include <nxt/EngineCore.h>

#include <random>

namespace nxt::random
{

	extern std::random_device nxtgRandomDevice;
	extern std::mt19937_64 nxtgMtEngine;

	template<typename T>
	T GetInt(T min, T max)
	{
		std::uniform_int_distribution<T> gDistribution(min, max);
		return gDistribution(nxtgMtEngine);
	}

	template<typename T>
	T GetInt()
	{
		std::uniform_int_distribution<T> gDistribution;
		return gDistribution(nxtgMtEngine);
	}

	template<typename T>
	T GetNumber(T min, T max)
	{
		std::uniform_real_distribution<T> gDistribution(min, max);
		return gDistribution(nxtgMtEngine);
	}

	template<typename T>
	T GetNumber()
	{
		std::uniform_real_distribution<T> gDistribution;
		return gDistribution(nxtgMtEngine);
	}

	/*
	template<typename int32_t>
	int32_t GetNumber(int32_t min = 0, int32_t max = 10)
	{
		std::uniform_int_distribution<int32_t> gDistribution(min, max);
		return gDistribution(nxtgMtEngine);
	}

	template<typename uint32_t>
	uint32_t GetNumber(uint32_t min = 0, uint32_t max = 10)
	{
		std::uniform_int_distribution<uint32_t> gDistribution(min, max);
		return gDistribution(nxtgMtEngine);
	}*/

	//float GetNumber(float min = 0.f, float max = 1.f);
	//int32_t GetInt(int32_t min = 0, int32_t max = 1);

}
