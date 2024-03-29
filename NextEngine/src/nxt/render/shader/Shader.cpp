
#include "Shader.h"

#include <nxt/core/log/Log.h>

#include <fstream>
#include <filesystem>

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

static uint32_t nxtGLSLVersion{ 460 };

namespace nxt
{

	// for variable declaration before compilation?
	template<typename... args>
	std::string FormatShaderString(std::string& shader, args... params)
	{
		return std::format(shader, params...);
	}

	enum class nxtShaderType
	{
		None = 0,
		Vertex,
		Pixel,
		Geometry
	};

	//using shader_pair = std::pair<std::string, nxtShaderType>;
	using shader_pair = std::pair<std::string, nxtShaderType>;

	void Shader::SetGLSLVersion(uint32_t version)
	{
		nxtGLSLVersion = version;
	}

	static bool ReadShaderFile(shader_pair* pair, const std::filesystem::path& filepath)
	{
		std::ifstream file;
		std::string& output{ pair->first };
		nxtShaderType& t{ pair->second };
		t = nxtShaderType::None;
		file.open(filepath);
		NXT_ASSERT(file.good(), "Shader read failure: \"{0}\"", filepath.string());
		if (file.good())
		{
			std::stringstream stream;
			stream << file.rdbuf();
			output = stream.str();

			std::string extension{ filepath.extension().string() };
			if (extension == ".vert")
			{
				t = nxtShaderType::Vertex;
			}
			else if (extension == ".frag")
			{
				t = nxtShaderType::Pixel;
			}
			else if (extension == ".geom")
			{
				t = nxtShaderType::Geometry;
			}

		}
		else
		{
			NXT_LOG_CRIT("Shader read failure: \"{0}\"", filepath.string());
		}
		file.close();
		return (t != nxtShaderType::None);
		//return { output, t };
	}
	
	Shader::Shader(const std::initializer_list<std::filesystem::path>& shader_locations)
	{
		std::vector<uint32_t> shaderIds{};
		for (const std::filesystem::path& path : shader_locations)
		{
			shader_pair subShader{};
			ReadShaderFile(&subShader, path);

			uint32_t id{ 0 };
			switch (subShader.second)
			{
				case(nxtShaderType::None): break;
				case(nxtShaderType::Vertex):
				{
					id = glCreateShader(GL_VERTEX_SHADER);
					break;
				}
				case(nxtShaderType::Pixel):
				{
					id = glCreateShader(GL_FRAGMENT_SHADER);
					break;
				}
				case(nxtShaderType::Geometry):
				{
					id = glCreateShader(GL_GEOMETRY_SHADER);
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

	void Shader::SetUniformBlock(const std::string& name, int32_t value)
	{
		uint32_t loc{ glGetUniformBlockIndex(mID, name.c_str()) };
		glUniformBlockBinding(mID, loc, value);
	}

	void Shader::SetArrayValue(const std::string& name, const std::vector<int32_t>& values)
	{
		int32_t loc{ glGetUniformLocation(mID, name.c_str()) };
		glUniform1iv(loc, values.size(), &values.front());
	}

	void Shader::SetArrayValue(const std::string& name, const std::vector<glm::vec3>& values)
	{
		int32_t loc{ glGetUniformLocation(mID, name.c_str()) };
		glUniform3fv(loc, values.size(), glm::value_ptr(values.front()));
	}

	void Shader::SetArrayValue(const std::string& name, const std::vector<glm::mat4>& values)
	{
		int32_t loc{ glGetUniformLocation(mID, name.c_str()) };
		glUniformMatrix4fv(loc, values.size(), GL_FALSE, glm::value_ptr(values.front()));
	}

	void Shader::SetValue(const std::string& name, int32_t value)
	{
		int32_t loc{ glGetUniformLocation(mID, name.c_str()) };
		glUniform1i(loc, value);
	}

	void Shader::SetValue(const std::string& name, float value)
	{
		int32_t loc{ glGetUniformLocation(mID, name.c_str()) };
		glUniform1f(loc, value);
	}

	void Shader::SetValue(const std::string& name, const glm::mat4& matrix)
	{
		int32_t loc{ glGetUniformLocation(mID, name.c_str()) };
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::SetValue(const std::string& name, const glm::vec3& vec)
	{
		int32_t loc{ glGetUniformLocation(mID, name.c_str()) };
		glUniform3fv(loc, 1, glm::value_ptr(vec));
	}

}
