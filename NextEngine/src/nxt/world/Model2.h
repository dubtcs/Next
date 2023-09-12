#pragma once

#include <nxt/EngineCore.h>

#include "materials/Material.h"
#include "Primitive.h"
#include "Animation.h"

#include <nxt/render/texture/FrameTexture.h>

#include <filesystem>

namespace nxt
{

	class NXT_API Model2
	{
	public:
		using Scene = std::vector<int32_t>;
	public:
		Model2(const std::filesystem::path& filepath);
		~Model2();
		std::vector<Mesh2>& GetMeshes();
		std::vector<Scene>& GetScenes();
		std::vector<Animation>& GetAnimations();
		std::vector<Shared<Material>>& GetMaterials();
		std::vector<Shared<FrameTexture>>& GetTextures();
	public:
		int32_t mRootScene{ 0 };
	protected:
		std::vector<Mesh2> mMeshes;
		std::vector<Scene> mScenes;
		std::vector<Animation> mAnimations;
		std::vector<Shared<Material>> mMaterials;
		std::vector<Shared<FrameTexture>> mTextures;
	};

	struct NXT_API Model2Instance
	{
		Shared<Model2> Model;
	};

}
