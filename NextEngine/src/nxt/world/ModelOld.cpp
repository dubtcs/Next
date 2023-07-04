
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

	void DrawMesh(Mesh& mesh)
	{
		for (Primitive& p : mesh.primitives)
		{
			Shared<DataBuffer> buffer{ p.buffer };
			buffer->Bind();
			buffer->Draw(nxtDrawMode_Triangles, p.count, static_cast<uint32_t>(p.byteOffset), static_cast<nxtDataType>(p.componentType));
		}
		for (Mesh& otherMesh : mesh.children)
		{
			DrawMesh(otherMesh);
		}
	}

	// temp
	void Model::Draw()
	{
		mArrayObject->Bind();
		mTextures.front()->Bind(0);
		for (Mesh& mesh : mMeshes)
		{
			DrawMesh(mesh);
		}
	}

	std::vector<Mesh> Model::RegisterModel(tinygltf::Model& model)
	{

		// Buffers
		std::vector<Shared<DataBuffer>> buffers{};
		for (tinygltf::BufferView& view : model.bufferViews)
		{
			tinygltf::Buffer& buffer{ model.buffers[view.buffer] };
			Shared<DataBuffer> dBuffer{ DataBuffer::Create(view.byteLength, &buffer.data.at(0) + view.byteOffset, static_cast<nxtBufferTarget>(view.target)) };
			buffers.push_back(dBuffer);
		}

		// Textures
		if (model.textures.size() > 0)
		{
			for (tinygltf::Texture& texture : model.textures)
			{
				tinygltf::Image& image{ model.images[texture.source] };

				nxtTextureFormat format{ 0 };
				switch (image.component)
				{
					case(1):
					{
						format = nxtTextureFormat_R;
						break;
					}
					case(2):
					{
						format = nxtTextureFormat_RG;
						break;
					}
					case(3):
					{
						format = nxtTextureFormat_RGB;
						break;
					}
					case(4):
					{
						format = nxtTextureFormat_RGBA;
						break;
					}
				}

				nxtDataType dataType{ 0 };
				if (image.bits == 8)
				{
					dataType = nxtDataType_UByte;
				}
				else if (image.bits == 16)
				{
					dataType = nxtDataType_UShort;
				}
				else if (image.bits == 32)
				{
					dataType = nxtDataType_UInt;
				}
				else
				{
					NXT_LOG_CRIT("Image bitdepth [{0}] not support atm", image.bits);
					break;
				}

				STexture tex{ Texture::Create(image.width, image.height, format) };
				tex->SetData(format, dataType, &image.image.at(0));
				mTextures.push_back(tex);

			}
		}

		// Cycle nodes
		std::vector<Mesh> meshes;
		for (int32_t& i : model.scenes[model.defaultScene].nodes)
		{
			meshes.push_back(RegisterNode(model, model.nodes[i], buffers));
		}

		// MUST Unbind VAO before the buffers are deleted
		mArrayObject->Unbind();
		return meshes;
	}

	Mesh Model::RegisterNode(tinygltf::Model& model, tinygltf::Node& node, std::vector<Shared<DataBuffer>>& buffers)
	{
		NXT_LOG_TRACE("Register Node");
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
		NXT_LOG_TRACE("Register Mesh");
		std::vector<Primitive> primitives{};

		for (tinygltf::Primitive& primitive : mesh.primitives)
		{
			tinygltf::Accessor& indexAccessor{ model.accessors[primitive.indices] };
			Primitive addPrimitive{};

			addPrimitive.buffer = buffers[primitive.indices];
			addPrimitive.mode = static_cast<nxtDrawMode>(primitive.mode);
			addPrimitive.count = static_cast<uint32_t>(indexAccessor.count);
			addPrimitive.byteOffset = static_cast<uint32_t>(indexAccessor.byteOffset);
			addPrimitive.componentType = static_cast<nxtDataType>(indexAccessor.componentType);

			primitives.push_back(addPrimitive);

			for (auto& attribute : primitive.attributes)
			{
				// Binding buffers for attribute locationd data
				tinygltf::Accessor& accessor{ model.accessors[attribute.second] };

				Shared<DataBuffer>& dBuffer{ buffers[accessor.bufferView] };
				dBuffer->Bind();

				int32_t amount{ accessor.type == TINYGLTF_TYPE_SCALAR ? 1 : accessor.type };
				int32_t stride{ accessor.ByteStride(model.bufferViews[accessor.bufferView]) };

				int32_t layoutPosition{ 0 };
				
				if (attribute.first == "POSITION") layoutPosition = 0;
				else if (attribute.first == "NORMAL") layoutPosition = 1;
				else if (attribute.first == "TEXCOORD_0") layoutPosition = 2;

				NXT_LOG_TRACE("Setting Layout Position: {0}", layoutPosition);
				mArrayObject->SetLayoutPosition(layoutPosition, amount, static_cast<nxtDataType>(accessor.componentType), stride, static_cast<uint32_t>(accessor.byteOffset), accessor.normalized);
			}

		}

		return primitives;
	}

}
