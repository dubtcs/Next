#pragma once

#include <nxt/EngineCore.h>

#include "materials/Material.h"
#include "Primitive.h"
#include "Animation.h"

#include <nxt/render/buffers/ArrayObject.h>
#include <nxt/render/buffers/DataBuffer.h>

#include <nxt/render/texture/Texture.h>

#include <filesystem>

namespace nxt
{

	class NXT_API Model
	{
	public:
		static Shared<Model> Create(const std::filesystem::path& filepath);
		Model(const std::filesystem::path& filepath);
		~Model();

		// TEMP FOR ECS
		const std::vector<Mesh>& GetMeshes() const;
		const std::vector<STexture>& GetTextures() const;
		const std::vector<SMaterial>& GetMaterials() const;
	protected:
		//Shared<buffers::ArrayObject> mArrayObject{ buffers::ArrayObject::Create() };
		std::vector<Mesh> mMeshes;
		std::vector<SMaterial> mMaterials;
		std::vector<STexture> mTextures;
	};

	using SModel = Shared<Model>;

	// Use a ModelInstance to switch the mesh of a model without needed to reassign multiple components
	struct ModelInstance
	{
		SModel Model;
	};

	using SModelInstance = Shared<ModelInstance>;

}
