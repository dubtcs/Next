#pragma once

#include <nxt/EngineCore.h>

#include "materials/Material.h"
#include "Animation.h"

#include <nxt/render/buffers/ArrayObject.h>
#include <nxt/render/buffers/DataBuffer.h>

#include <nxt/render/texture/Texture.h>

#include <filesystem>

namespace nxt
{

	struct NXT_API PrimitiveModifier
	{
		bool used{ false };
		std::vector<uint16_t> indices{};
	};

	struct NXT_API Primitive
	{
		uint32_t count;
		int32_t material{ -1 };
		size_t byteOffset;
		nxtDrawMode mode;
		nxtDataType componentType;
		bool hasTangents{ false };
		bool hasIndices{ false };
		PrimitiveModifier modifier{};
		buffers::SDataBuffer buffer;
		buffers::SArrayObject arrayObject{ NewShared<buffers::ArrayObject>() };
	};

	struct NXT_API Mesh // also a node
	{
		std::vector<Primitive> primitives;
		std::vector<Mesh> children;
		glm::mat4 matrix{ 1.f };
	};

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
