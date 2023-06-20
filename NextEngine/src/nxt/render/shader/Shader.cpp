
#include "Shader.h"

#include <nxt/core/log/Log.h>

#include <fstream>
#include <filesystem>

namespace nxt
{

	std::string ReadShaderFile(const std::filesystem::path& filepath)
	{
		std::string output;
		std::ifstream file;
		file.open(filepath);
		NXT_ASSERT(file.good(), "Shader read failure: \"{0}\"", filepath.string());
		if (file.good())
		{
			std::stringstream stream;
			stream << file.rdbuf();
			output = stream.str();
		}
		else
		{
			NXT_LOG_CRIT("Shader read failure: \"{0}\"", filepath.string());
		}
		file.close();
		return output;
	}
	
	Shader::Shader(const std::filesystem::path& filepath)
	{
		std::string shader{ ReadShaderFile(filepath) };

	}

}
