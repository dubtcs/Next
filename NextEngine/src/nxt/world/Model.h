#pragma once

#include <nxt/EngineCore.h>

#include <nxt/render/buffers/ArrayObject.h>
#include <nxt/render/buffers/DataBuffer.h>

#include <nxt/render/texture/Texture.h>

#include <filesystem>
#include <tiny_gltf.h>

namespace nxt
{

	struct NXT_API Primitive
	{
		Shared<buffers::DataBuffer> buffer;
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
		Model(const std::filesystem::path& filepath);
		void Draw();
	protected:
		std::vector<Mesh> RegisterModel(tinygltf::Model& model);
		Mesh RegisterNode(tinygltf::Model& model, tinygltf::Node& node, std::vector<Shared<buffers::DataBuffer>>& buffers);
		std::vector<Primitive> RegisterMesh(tinygltf::Model& model, tinygltf::Mesh& mesh, std::vector<Shared<buffers::DataBuffer>>& buffers);
	protected:
		Shared<buffers::ArrayObject> mArrayObject{ buffers::ArrayObject::Create() };
		std::vector<Mesh> mMeshes;
		std::vector<STexture> mTextures;
	};

}
