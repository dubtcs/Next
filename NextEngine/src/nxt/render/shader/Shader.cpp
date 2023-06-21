
#include "Shader.h"

#include <nxt/core/log/Log.h>

#include <fstream>
#include <filesystem>

#include <glad/glad.h>

namespace nxt
{

	//using shader_pair = std::pair<std::string, ShaderType>;
	using shader_pair = std::pair<std::string, ShaderType>;

	static shader_pair ReadShaderFile(const std::filesystem::path& filepath)
	{
		std::string output;
		std::ifstream file;
		ShaderType t{ ShaderType::None };
		file.open(filepath);
		NXT_ASSERT(file.good(), "Shader read failure: \"{0}\"", filepath.string());
		if (file.good())
		{
			std::stringstream stream;
			stream << file.rdbuf();
			output = stream.str();

			if (filepath.extension().string() == ".vert")
			{
				t = ShaderType::Vertex;
			}
			else if (filepath.extension().string() == ".frag")
			{
				t = ShaderType::Pixel;
			}

		}
		else
		{
			NXT_LOG_CRIT("Shader read failure: \"{0}\"", filepath.string());
		}
		file.close();
		return { output, t};
	}
	
	Shader::Shader(const std::initializer_list<std::filesystem::path>& shader_locations)
	{
		std::vector<uint32_t> shaderIds{};
		for (const std::filesystem::path& path : shader_locations)
		{
			shader_pair subShader{ ReadShaderFile(path) };

			uint32_t id{ 0 };
			switch (subShader.second)
			{
				case(ShaderType::None): break;
				case(ShaderType::Vertex):
				{
					id = glCreateShader(GL_VERTEX_SHADER);
					break;
				}
				case(ShaderType::Pixel):
				{
					id = glCreateShader(GL_FRAGMENT_SHADER);
					break;
				}
			}
			if (id)
			{
				const char* subShaderCstr{ subShader.first.c_str() };

				NXT_LOG_TRACE("Creating shader \"{0}\"", path.filename().string());
				glShaderSource(id, 1, &subShaderCstr, 0);
				glCompileShader(id);
				int32_t pass{ 0 };
				char info[512];
				glGetShaderiv(id, GL_COMPILE_STATUS, &pass);
				if (pass)
				{
					shaderIds.push_back(id);
				}
				else
				{
					glGetShaderInfoLog(id, 512, NULL, info);
					NXT_LOG_WARN("Shader compilation failure: \"{0}\" {1}", path.filename().string(), info);
					glDeleteShader(id);
				}
			}
		}

		mID = glCreateProgram();
		for (uint32_t& id : shaderIds)
		{
			glAttachShader(mID, id);
		}
		glLinkProgram(mID);
		char info[512];
		int32_t pass{ 0 };
		glGetProgramiv(mID, GL_LINK_STATUS, &pass);
		if (pass)
		{
			glUseProgram(mID);
		} 
		else
		{
			glGetProgramInfoLog(mID, 512, 0, info);
			NXT_LOG_CRIT("Shader program link failure: {0}", info);
			glDeleteProgram(mID);
		}

		for (uint32_t& id : shaderIds)
		{
			glDeleteShader(id);
		}
	}

	Shader::~Shader()
	{
		glDeleteProgram(mID);
	}

	void Shader::Bind()
	{
		glUseProgram(mID);
	}

	void Shader::SetValue(const std::string& name, int32_t value)
	{
		int32_t loc{ glGetUniformLocation(mID, name.c_str()) };
		glUniform1i(loc, value);
	}

}
