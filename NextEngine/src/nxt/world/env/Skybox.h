#pragma once

#include <nxt/EngineCore.h>

#include <nxt/world/model/Model2.h>
#include <nxt/render/shader/Shader.h>
#include <nxt/render/texture/Cubemap.h>

#include <filesystem>

namespace nxt
{

	inline constexpr int32_t gSkyboxTextureUnit{ 14 };

	class NXT_API Skybox
	{
	public:
		Skybox(const std::initializer_list<std::filesystem::path>& path);
		~Skybox();
		Shared<Model2> GetModel() const;
	public:
		Shared<Cubemap> mTexture;
		//static Shared<Model2> gModel;
	};

}
