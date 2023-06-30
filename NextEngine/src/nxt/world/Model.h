#pragma once

#include <nxt/EngineCore.h>

#include <nxt/render/buffers/ArrayObject.h>
#include <nxt/render/buffers/DataBuffer.h>

#include <filesystem>
#include <tiny_gltf.h>

namespace nxt
{

	struct NXT_API Primitive
	{
		Shared<buffers::DataBuffer> buffer;
		buffers::DRAW_MODE_ mode;
		uint32_t count;
		uint32_t byteOffset;
		DATA_TYPE_ componentType;
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
		//void RegisterModel(tinygltf::Model& model);
		//void RegisterNode(tinygltf::Model& model, tinygltf::Node& node);
		//void RegisterMesh(tinygltf::Model& model, tinygltf::Mesh& mesh);
	protected:
		Shared<buffers::ArrayObject> mArrayObject{ buffers::ArrayObject::Create() };
		std::vector<Mesh> mMeshes;

		std::vector<Shared<buffers::DataBuffer>> mBuffers;
	};

}
