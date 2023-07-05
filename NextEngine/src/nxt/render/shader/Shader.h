#pragma once

#include <nxt/EngineCore.h>

#include <filesystem>
#include <string>

#include <glm.hpp>

namespace nxt
{

	class NXT_API Shader
	{
	public:
		Shader() = default;
		Shader(const std::initializer_list<std::filesystem::path>& shader_locations);
		~Shader();
		void Bind();
		void SetValue(const std::string& name, int32_t value);
		void SetValue(const std::string& name, const glm::vec3& vector3);
		void SetValue(const std::string& name, const glm::mat4& matrix);
	protected:
		std::string mName;
		uint32_t mID{ 0 };
	};

}
