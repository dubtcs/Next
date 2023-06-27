
#include "Model.h"

#include <nxt/core/log/Log.h>

#include <tiny_gltf.h>
#include <vector>

namespace nxt
{

	namespace gltf = tinygltf;
	using namespace buffers;

	// temp
	static std::vector<Shared<buffers::VertexBuffer>> gVBuffers;
	static std::vector<Shared<buffers::ElementBuffer>> gEBuffers;

	static Shared<ArrayObject> mArrayObject;
	static std::vector<Shared<DataBuffer>> gBuffers;

	Model::Model(const std::filesystem::path& filepath)
	{
		gltf::TinyGLTF loader;
		std::string msgError;
		std::string msgWarning;
		bool pass{ loader.LoadASCIIFromFile(&mModel, &msgError, &msgWarning, filepath.string()) };
		NXT_LOG_TRACE("Loading model: {0}", filepath.string());
		if (!msgWarning.empty())
		{
			NXT_LOG_WARN("Model load warning: {0}", msgWarning);
		}
		if (!msgError.empty())
		{
			NXT_LOG_CRIT("Model load error: {0}", msgError);
		}
		mArrayObject = ArrayObject::Create();
		mArrayObject->Bind();

		RegisterModel();
	}

	void Model::Draw()
	{
		mArrayObject->Bind();
		gltf::Scene& scene{ mModel.scenes[mModel.defaultScene] };
		for (int32_t& i : scene.nodes)
		{
			DrawNode(mModel.nodes[i]);
		}
	}

	void Model::DrawNode(gltf::Node& node)
	{
		if (node.mesh >= 0)
		{
			DrawMesh(mModel.meshes[node.mesh]);
		}
		for (int32_t& i : node.children)
		{
			DrawNode(mModel.nodes[i]);
		}
	}

	void Model::DrawMesh(gltf::Mesh& mesh)
	{
		for (gltf::Primitive& primitive : mesh.primitives)
		{
			gltf::Accessor& ac{ mModel.accessors[primitive.indices] };
			Shared<DataBuffer> buffer{ gBuffers[ac.bufferView] };
			buffer->Bind();
			buffer->Draw(static_cast<DRAW_MODE_>(primitive.mode), ac.count, ac.byteOffset, static_cast<DATA_TYPE_>(ac.componentType));
		}
	}

	void Model::RegisterModel()
	{
		using namespace buffers;
		for (gltf::BufferView& view : mModel.bufferViews)
		{
			//Shared<VertexBuffer> vBuffer{ VertexBuffer::Create(view.byteLength) };
			//gVBuffers.push_back(vBuffer);
			gltf::Buffer& buffer{ mModel.buffers[view.buffer] };
			
			NXT_LOG_TRACE("{0}", view.target == BUFFER_TARGET_ARRAY_BUFFER ? "VBuffer" : "EBuffer");
			Shared<DataBuffer> dBuffer{ DataBuffer::Create(buffer.data.size(), &buffer.data.at(0) + view.byteOffset, static_cast<BUFFER_TARGET_>(view.target)) };
			gBuffers.push_back(dBuffer);

		}
		
		gltf::Scene scene{ mModel.scenes[mModel.defaultScene] };
		for (int32_t& nodeIndex : scene.nodes)
		{
			RegisterNode(mModel.nodes[nodeIndex]);
		}

		// Not using because we need to keep the exact indices of the EBOs
		/*using buffer_iter = std::vector<Shared<DataBuffer>>::iterator;
		for (int32_t i{ 0 }; i < gBuffers.size();)
		{
			if (gBuffers[i]->GetTarget() == BUFFER_TARGET_ARRAY_BUFFER)
			{
				gBuffers.erase(gBuffers.begin() + i);
				continue;
			}
			i++;
		}*/
	}

	void Model::RegisterNode(gltf::Node& node)
	{
		if (node.mesh >= 0)//< mModel.meshes.size())
		{
			RegisterMesh(mModel.meshes[node.mesh]);
		}
		for (int32_t& i : node.children)
		{
			RegisterNode(mModel.nodes[i]);
		}
	}

	void Model::RegisterMesh(gltf::Mesh& mesh)
	{
		for (gltf::Primitive& primitive : mesh.primitives)
		{
			gltf::Accessor& ac1{ mModel.accessors[primitive.indices] };
			for (auto& attribute : primitive.attributes)
			{
				gltf::Accessor& ac2{ mModel.accessors[attribute.second] };
				int32_t stride{ ac2.ByteStride(mModel.bufferViews[ac2.bufferView]) };
				Shared<DataBuffer>& dBuffer{ gBuffers[ac2.bufferView] };
				dBuffer->Bind();

				int32_t amount{ 1 };
				if (ac2.type != TINYGLTF_TYPE_SCALAR)
				{
					amount = ac2.type;
				}
				NXT_LOG_TRACE("Amount: {0}", amount);

				if (attribute.first == "POSITION")
				{
					mArrayObject->SetLayoutPosition(0, amount, static_cast<DATA_TYPE_>(ac2.componentType), stride, ac2.byteOffset, ac2.normalized);
				}
			}
		}
	}

}

/*

if (view.target == BUFFER_TARGET_ARRAY_BUFFER)
			{
				Shared<VertexBuffer> vBuffer{ VertexBuffer::Create(buffer.data.size(), &buffer.data.at(0) + view.byteOffset, BUFFER_USAGE_STATIC) };
				gVBuffers.push_back(vBuffer);
			}
			else if (view.target == BUFFER_TARGET_ELEMENT_ARRAY)
			{
				Shared<ElementBuffer> eBuffer{ ElementBuffer::Create(buffer.data.size(), &buffer.data.at(0) + view.byteOffset, BUFFER_USAGE_STATIC) };
				gEBuffers.push_back(eBuffer);
			}
			else
			{
				NXT_LOG_WARN("Buffer Target not supported: {0}", view.target);
			}

*/


/*using namespace buffers;
		for (gltf::BufferView& view : mModel.bufferViews)
		{
			vertex_buffer vBuffer{ VertexBuffer::Create(view.byteLength) };
			gVBuffers.push_back(vBuffer);
		}
		for (gltf::Accessor& accessor : mModel.accessors)
		{
			vertex_buffer& vbuffer{ gVBuffers[accessor.bufferView] };
			int32_t stride{ accessor.ByteStride(mModel.bufferViews[accessor.bufferView]) };
			if(accessor.name)
			vbuffer->SetLayoutPosition(0, accessor.count, static_cast<DATA_TYPE_>(accessor.componentType), static_cast<uint32_t>(stride), accessor.byteOffset, accessor.normalized);
		}*/
