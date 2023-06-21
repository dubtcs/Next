#pragma once

#include <nxt/EngineCore.h>

#include <filesystem>
#include <string>

namespace nxt
{

	enum class ShaderType
	{
		None = 0,
		Vertex,
		Pixel
	};

	class NXT_API Shader
	{
	public:
		Shader() = default;
		Shader(const std::initializer_list<std::filesystem::path>& shader_locations);
		~Shader();
		void Bind();
		void SetValue(const std::string& name, int32_t value);
	protected:
		std::string mName;
		uint32_t mID{ 0 };
	};

}
