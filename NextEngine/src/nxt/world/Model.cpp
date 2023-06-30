
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
		RegisterModel(model);
	}

	void Model::Draw()
	{
		for (Mesh& mesh : mMeshes)
		{
			for (Primitive& p : mesh.primitives)
			{
				Shared<DataBuffer> buffer{ p.buffer };
				buffer->Bind();
				buffer->Draw(DRAW_MODE_TRIANGLES, p.count, p.byteOffset, static_cast<DATA_TYPE_>(p.componentType));
			}
		}
	}

	void Model::RegisterModel(tinygltf::Model& model)
	{
		// Create data buffers for each view
		for (tinygltf::BufferView& view : model.bufferViews)
		{
			tinygltf::Buffer& buffer{ model.buffers[view.buffer] };
			Shared<DataBuffer> dBuffer{ DataBuffer::Create(view.byteLength, &buffer.data.at(0) + view.byteOffset, static_cast<BUFFER_TARGET_>(view.target)) };
			mBuffers.push_back(dBuffer);
		}

		// Cycle nodes
		for (int32_t& i : model.scenes[model.defaultScene].nodes)
		{
			RegisterNode(model, model.nodes[i]);
		}
	}

	/*
	NEED TO UPDATE TO WORK WITH RECURSIVE CHILDREN ADDITIONS
	*/
	void Model::RegisterNode(tinygltf::Model& model, tinygltf::Node& node)
	{
		if (node.mesh >= 0)
		{
			RegisterMesh(model, model.meshes[node.mesh]);
		}
		for (int32_t& i : node.children)
		{
			RegisterNode(model, model.nodes[i]);
		}
	}

	void Model::RegisterMesh(tinygltf::Model& model, tinygltf::Mesh& mesh)
	{
		Mesh addMesh{};
		for (tinygltf::Primitive& primitive : mesh.primitives)
		{
			tinygltf::Accessor& indexAccessor{ model.accessors[primitive.indices] };
			Primitive addPrimitive{};

			addPrimitive.buffer = mBuffers[primitive.indices];
			addPrimitive.mode = static_cast<DRAW_MODE_>(primitive.mode);
			addPrimitive.count = indexAccessor.count;
			addPrimitive.byteOffset = indexAccessor.byteOffset;
			addPrimitive.componentType = static_cast<DATA_TYPE_>(indexAccessor.componentType);

			for (auto& attribute : primitive.attributes)
			{
				// Binding buffers for attribute locationd data
				tinygltf::Accessor& accessor{ model.accessors[attribute.second] };
				Shared<DataBuffer>& dBuffer{ mBuffers[accessor.bufferView] };
				dBuffer->Bind();
				int32_t amount{ accessor.type == TINYGLTF_TYPE_SCALAR ? 1 : accessor.type };
				NXT_LOG_TRACE("Attribute amount: {0}", amount);

				int32_t stride{ accessor.ByteStride(model.bufferViews[accessor.bufferView]) };
				if (attribute.first == "POSITION")
				{
					NXT_LOG_TRACE("Setting POSITION");
					mArrayObject->SetLayoutPosition(0, amount, static_cast<DATA_TYPE_>(accessor.componentType), stride, accessor.byteOffset, accessor.normalized);
				}
			}

			addMesh.primitives.push_back(addPrimitive);
		}
		mMeshes.push_back(addMesh);
	}

}
