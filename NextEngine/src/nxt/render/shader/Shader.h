#pragma once

#include <nxt/EngineCore.h>

#include <filesystem>
#include <string>

namespace nxt
{

	class NXT_API Shader
	{
	public:
		Shader(const std::filesystem::path& filepath);
		void Bind();
	protected:
		std::string mName;
		uint32_t mID;
	};

}
