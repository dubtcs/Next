
#include "Model.h"

#include <nxt/render/buffers/DataBuffer.h>
#include <nxt/core/log/Log.h>

#include <tiny_gltf.h>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

namespace nxt
{

	namespace gltf = tinygltf;

	struct TanBufferCache
	{
		buffers::SDataBuffer buffer;
		int32_t amount;
		int32_t stride;
	};

	static std::vector<glm::vec4> GenerateTangents(TanBufferCache& positions, TanBufferCache& normals)
	{

		return {};
	}

	static std::vector<Primitive> RegisterMesh(buffers::SArrayObject& arrayObject, tinygltf::Model& model, tinygltf::Mesh& mesh, std::vector<buffers::SDataBuffer>& buffers)
	{
		using namespace buffers;
		//NXT_LOG_TRACE("Register Mesh");
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
			addPrimitive.material = primitive.material;

			primitives.push_back(addPrimitive);

			bool tangentsFound{ false };
			
			//TanBufferCache positionData;
			//TanBufferCache normalData;

			for (auto& attribute : primitive.attributes)
			{
				// Binding buffers for attribute locationd data
				tinygltf::Accessor& accessor{ model.accessors[attribute.second] };

				Shared<DataBuffer>& dBuffer{ buffers[accessor.bufferView] };
				dBuffer->Bind();

				int32_t amount{ accessor.type == TINYGLTF_TYPE_SCALAR ? 1 : accessor.type };
				int32_t stride{ accessor.ByteStride(model.bufferViews[accessor.bufferView]) };

				int32_t layoutPosition{ -1 };

				if (attribute.first == "POSITION")
				{
					layoutPosition = 0;
					//positionData.buffer = dBuffer;
					//positionData.amount = amount;
					//positionData.stride = stride;
				}
				else if (attribute.first == "NORMAL")
				{
					layoutPosition = 1;
				}
				else if (attribute.first == "TANGENT") 
				{	
					layoutPosition = 2; 
					tangentsFound = true; 
					//normalData.buffer = dBuffer;
					//normalData.amount = amount;
					//normalData.stride = stride;
				}
				else if (attribute.first == "TEXCOORD_0") 
				{
					layoutPosition = 3;
				}
				else NXT_LOG_DEBUG("Attribute found that is not supported: {0}", attribute.first);

				if (layoutPosition >= 0)
				{
					//NXT_LOG_TRACE("Setting Layout Position: {0}, amount {1}", layoutPosition, amount);
					NXT_LOG_TRACE(stride);
					arrayObject->SetLayoutPosition(layoutPosition, amount, static_cast<nxtDataType>(accessor.componentType), stride, static_cast<uint32_t>(accessor.byteOffset), accessor.normalized);
				}
			}

			// If no tangents are found:
			// If no texture is found, use normals
			// else, use texture coordinates, vertices, and indices to make them

			//if (!tangentsFound)
			//{
				//NXT_LOG_INFO("Tangents not found, generating.");
				//std::vector<glm::vec4> tangentVectors{ GenerateTangents(positionData, normalData) };
				//SDataBuffer tanBuffer{ buffers::DataBuffer::Create(sizeof(glm::vec4) * tangentVectors.size(), &tangentVectors) };
				//tanBuffer->Bind();
				//arrayObject->SetLayoutPosition(2, tangentVectors.size(), nxtDataType_Float, 0, 0, false);
				//arrayObject->Unbind();
			//}
			//arrayObject->Bind();

		}

		return primitives;
	}

	static Mesh RegisterNode(buffers::SArrayObject& arrayObject, tinygltf::Model& model, tinygltf::Node& node, std::vector<buffers::SDataBuffer>& buffers)
	{
		//NXT_LOG_TRACE("Register Node");
		Mesh mesh{};
		if (node.mesh >= 0)
		{
			mesh.primitives = RegisterMesh(arrayObject, model, model.meshes[node.mesh], buffers);
		}
		for (int32_t& i : node.children)
		{
			mesh.children.push_back(RegisterNode(arrayObject, model, model.nodes[i], buffers));
		}
		return mesh;
	}

	static std::vector<Mesh> RegisterModel(buffers::SArrayObject& arrayObject, std::vector<STexture>& textures, std::vector<SMaterial>& materials, gltf::Model& model)
	{
		using namespace buffers;
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
				NXT_LOG_TRACE("Model Texture URI: {0}", image.uri);

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

				NXT_LOG_TRACE("w{0} h{1}", image.width, image.height);
				STexture tex{ Texture::Create(image.width, image.height, format) };
				tex->SetData(format, dataType, &image.image.at(0));
				textures.push_back(tex);

			}
		}

		// Materials
		if (model.materials.size() > 0)
		{
			for (gltf::Material& material : model.materials)
			{
				std::vector<double>& colour{ material.pbrMetallicRoughness.baseColorFactor };
				glm::vec4 baseColor{ colour[0], colour[1], colour[2], colour[3] };

				PBRColorProperties colProp{ baseColor, material.pbrMetallicRoughness.baseColorTexture.index };
				PBRRoughnessProperties roProp{ static_cast<float>(material.pbrMetallicRoughness.roughnessFactor), material.pbrMetallicRoughness.metallicRoughnessTexture.index };
				PBRMetallicProperties metProp{ static_cast<float>(material.pbrMetallicRoughness.metallicFactor), material.pbrMetallicRoughness.metallicRoughnessTexture.index };

				PBRProperties props{ colProp, roProp, metProp };

				PBRTextures tex{ material.emissiveTexture.index, material.occlusionTexture.index, material.normalTexture.index };

				SMaterial mat{ NewShared<Material>(props, tex) };
				materials.push_back(mat);
			}
		}

		// Cycle nodes
		std::vector<Mesh> meshes;
		for (int32_t& i : model.scenes[model.defaultScene].nodes)
		{
			meshes.push_back(RegisterNode(arrayObject, model, model.nodes[i], buffers));
		}

		// MUST Unbind VAO before the buffers are deleted
		arrayObject->Unbind();
		return meshes;
	}

	Shared<Model> Model::Create(const std::filesystem::path& filepath)
	{
		return NewShared<Model>(filepath);
	}

	Model::Model(const std::filesystem::path& filepath)
	{
		tinygltf::TinyGLTF loader;
		std::string msgError;
		std::string msgWarn;
		tinygltf::Model model;
		if (filepath.extension().string() == ".gltf")
		{
			bool pass{ loader.LoadASCIIFromFile(&model, &msgError, &msgWarn, filepath.string()) };
		}
		else if(filepath.extension().string() == ".glb")
		{
			bool pass{ loader.LoadBinaryFromFile(&model, &msgError, &msgWarn, filepath.string()) };
		}
		else
		{
			NXT_LOG_CRIT("Invalid file extension: {0}", filepath.extension().string());
			return;
		}
		mMeshes = RegisterModel(mArrayObject, mTextures, mMaterials, model);
	}

	void Model::Bind() const
	{
		mArrayObject->Bind();
	}

	const std::vector<Mesh>& Model::GetMeshes() const
	{
		return mMeshes;
	}

	const std::vector<STexture>& Model::GetTextures() const
	{
		return mTextures;
	}

	const std::vector<SMaterial>& Model::GetMaterials() const
	{
		return mMaterials;
	}

	Model::~Model()
	{

	}

}
