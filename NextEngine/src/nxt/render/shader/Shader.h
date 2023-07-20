#pragma once

#include <nxt/EngineCore.h>

#include <filesystem>
#include <string>

#include <glm/glm.hpp>

namespace nxt
{

	// Add option to append precompile definitions to shader
	// So we can change LIGHT_MAX or somn as we want

	class NXT_API Shader
	{
	public:
		static void SetGLSLVersion(uint32_t version);
		Shader() = default;
		Shader(const std::initializer_list<std::filesystem::path>& shader_locations);
		~Shader();
		void Bind();
		void SetValue(const std::string& name, int32_t value);
		void SetValue(const std::string& name, float value);
		void SetValue(const std::string& name, const glm::vec3& vector3);
		void SetValue(const std::string& name, const glm::mat4& matrix);
		void SetArrayValue(const std::string& name, const std::vector<glm::mat4>& values);
		void SetUniformBlock(const std::string& name, int32_t value);
	protected:
		std::string mName;
		uint32_t mID{ 0 };
	};

	using SShader = Shared<Shader>;

}
