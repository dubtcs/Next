#pragma once

#include <nxt/EngineCore.h>

#include <nxt/render/buffers/ArrayObject.h>
#include <nxt/render/buffers/DataBuffer.h>

#include <nxt/render/texture/Texture.h>

#include <filesystem>

namespace nxt
{

	struct NXT_API Primitive
	{
		buffers::SDataBuffer buffer;
		nxtDrawMode mode;
		uint32_t count;
		size_t byteOffset;
		nxtDataType componentType;
	};

	struct NXT_API Mesh // also a node
	{
		std::vector<Primitive> primitives;
		std::vector<Mesh> children;
	};

	class NXT_API Model
	{
	public:
		static Shared<Model> Create(const std::filesystem::path& filepath);
		Model(const std::filesystem::path& filepath);

		// TEMP FOR ECS
		void Bind() const;
		const std::vector<Mesh>& GetMeshes() const;
		const std::vector<STexture>& GetTextures() const;
	protected:
		Shared<buffers::ArrayObject> mArrayObject{ buffers::ArrayObject::Create() };
		std::vector<Mesh> mMeshes;
		std::vector<STexture> mTextures;
	};

	using SModel = Shared<Model>;

}
