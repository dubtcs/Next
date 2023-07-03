
#include "Model.h"

#include <nxt/core/log/Log.h>
#include <vector>

namespace nxt
{

	using namespace buffers;

	Model::Model(const std::filesystem::path& filepath)
	{
		tinygltf::TinyGLTF loader;
		std::string msgError;
		std::string msgWarn;
		tinygltf::Model model;
		bool pass{ loader.LoadASCIIFromFile(&model, &msgError, &msgWarn, filepath.string()) };
		mMeshes = RegisterModel(model);
	}

	// temp
	void Model::Draw()
	{
		for (Mesh& mesh : mMeshes)
		{
			for (Primitive& p : mesh.primitives)
			{
				Shared<DataBuffer> buffer{ p.buffer };
				buffer->Bind();
				buffer->Draw(nxtDrawMode_Triangles, p.count, p.byteOffset, static_cast<nxtDataType>(p.componentType));
			}
		}
	}

	std::vector<Mesh> Model::RegisterModel(tinygltf::Model& model)
	{
		std::vector<Shared<DataBuffer>> buffers{};
		for (tinygltf::BufferView& view : model.bufferViews)
		{
			tinygltf::Buffer& buffer{ model.buffers[view.buffer] };
			Shared<DataBuffer> dBuffer{ DataBuffer::Create(view.byteLength, &buffer.data.at(0) + view.byteOffset, static_cast<nxtBufferTarget>(view.target)) };
			buffers.push_back(dBuffer);
		}

		// Cycle nodes
		std::vector<Mesh> meshes;
		for (int32_t& i : model.scenes[model.defaultScene].nodes)
		{
			meshes.push_back(RegisterNode(model, model.nodes[i], buffers));
		}

		return meshes;
	}

	Mesh Model::RegisterNode(tinygltf::Model& model, tinygltf::Node& node, std::vector<Shared<DataBuffer>>& buffers)
	{
		Mesh mesh{};
		if (node.mesh >= 0)
		{
			mesh.primitives = RegisterMesh(model, model.meshes[node.mesh], buffers);
		}
		for (int32_t& i : node.children)
		{
			mesh.children.push_back(RegisterNode(model, model.nodes[i], buffers));
		}
		return mesh;
	}

	std::vector<Primitive> Model::RegisterMesh(tinygltf::Model& model, tinygltf::Mesh& mesh, std::vector<Shared<buffers::DataBuffer>>& buffers)
	{
		std::vector<Primitive> primitives{};

		for (tinygltf::Primitive& primitive : mesh.primitives)
		{
			tinygltf::Accessor& indexAccessor{ model.accessors[primitive.indices] };
			Primitive addPrimitive{};

			addPrimitive.buffer = buffers[primitive.indices];
			addPrimitive.mode = static_cast<nxtDrawMode>(primitive.mode);
			addPrimitive.count = indexAccessor.count;
			addPrimitive.byteOffset = indexAccessor.byteOffset;
			addPrimitive.componentType = static_cast<nxtDataType>(indexAccessor.componentType);

			primitives.push_back(addPrimitive);

			for (auto& attribute : primitive.attributes)
			{
				// Binding buffers for attribute locationd data
				tinygltf::Accessor& accessor{ model.accessors[attribute.second] };
				Shared<DataBuffer>& dBuffer{ buffers[accessor.bufferView] };
				dBuffer->Bind();
				int32_t amount{ accessor.type == TINYGLTF_TYPE_SCALAR ? 1 : accessor.type };
				NXT_LOG_TRACE("Attribute amount: {0}", amount);

				int32_t stride{ accessor.ByteStride(model.bufferViews[accessor.bufferView]) };
				if (attribute.first == "POSITION")
				{
					NXT_LOG_TRACE("Setting POSITION");
					mArrayObject->SetLayoutPosition(0, amount, static_cast<nxtDataType>(accessor.componentType), stride, accessor.byteOffset, accessor.normalized);
				}
			}

		}

		return primitives;
	}

}
