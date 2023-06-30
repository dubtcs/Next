#pragma once

#include <nxt/EngineCore.h>

#include <nxt/render/buffers/ArrayObject.h>
#include <nxt/render/buffers/DataBuffer.h>

#include <nxt/render/buffers/VertexBuffer.h>
#include <nxt/render/buffers/ElementBuffer.h>

#include <filesystem>

#include <tiny_gltf.h>

namespace nxt
{

	class NXT_API OldModel
	{
	public:
		OldModel(const std::filesystem::path& filepath);
	public:
		void Draw();
	protected:
		void RegisterModel();
		void RegisterNode(tinygltf::Node& node);
		void RegisterMesh(tinygltf::Mesh& mesh);

		// Placeholders before ECS
		void DrawNode(tinygltf::Node& node);
		void DrawMesh(tinygltf::Mesh& mesh);
	protected:
		class Mesh
		{
		friend class Model;
		public:
			Mesh();
		protected:
			std::vector<buffers::VertexBuffer> mVertexBuffers;
		};
	protected:
		tinygltf::Model mModel;
	};

}
