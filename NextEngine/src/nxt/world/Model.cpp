
#include "Model.h"

#include <nxt/render/buffers/DataBuffer.h>
#include <nxt/core/log/Log.h>

#include <nxt/core/utility/TypeToSize.h>

#include <tiny_gltf.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

static constexpr double gOneThird{ 0.3333 };

namespace nxt
{

	namespace gltf = tinygltf;

	// Generate normals for tris only for now
	static buffers::SDataBuffer GenerateNormals(Primitive& primitive)
	{
		using namespace buffers;
		std::vector<glm::vec3> normals{};

		if (!primitive.hasIndices)
		{
			for (uint32_t i{ 0 }; i < primitive.count;)
			{
				// Need access to GLTF model buffers for the raw data
				// Convert to Vertex 1, 2, 3
				// Cross product of vectors

				// OpenGL default front face is CCW
				// vector(TO - FROM) will give a position direction vector for CCW normals
				// V1 Normal:
				//		cross( normalize(v3 - v1), normalize(v2 - v1) )
				// V2
				//		cross( normalize(v1 - v2), normalize(v3 - v2) )
				// V3
				//		cross( normalize(v1 - v3), normalize(v2 - v3) )

				i += 3;
			}
		}

		SDataBuffer buffer{};
		return buffer;
	}
	// Should I even do this? I feel like at this point in 3D pipeline it's the previous programs problem

	static std::vector<Primitive> RegisterMesh2(buffers::SArrayObject& arrayObject, tinygltf::Model& model, tinygltf::Mesh& mesh, std::vector<buffers::SDataBuffer>& buffers)
	{
		using namespace buffers;
		std::vector<Primitive> primitives{};
		for (tinygltf::Primitive& primitive : mesh.primitives)
		{
			Primitive addition{};
			// has indices
			if (primitive.indices >= 0)
			{
				tinygltf::Accessor& accessor{ model.accessors[primitive.indices] };
				addition.count = static_cast<uint32_t>(accessor.count);
				addition.byteOffset = static_cast<uint32_t>(accessor.byteOffset);
				addition.componentType = static_cast<nxtDataType>(accessor.componentType);
				addition.hasIndices = true;
				addition.buffer = buffers[primitive.indices];
			}
			addition.mode = static_cast<nxtDrawMode>(primitive.mode);
			addition.material = primitive.material;

			bool hasNormals{ false };
			bool hasTangents{ false };
			SDataBuffer positionBuffer{ nullptr };

			for (auto& attribute : primitive.attributes)
			{
				tinygltf::Accessor& accessor{ model.accessors[attribute.second] };

				SDataBuffer& buffer{ buffers[accessor.bufferView] };
				buffer->Bind();

				int32_t amount{ accessor.type == TINYGLTF_TYPE_SCALAR ? 1 : accessor.type };
				int32_t stride{ accessor.ByteStride(model.bufferViews[accessor.bufferView]) };

				int32_t layoutPosition{ -1 };
				if (attribute.first == "POSITION")
				{
					layoutPosition = 0;
					// Non indexed geometry. Use draw arrays
					if (!addition.hasIndices)
					{
						addition.buffer = buffer;
						addition.byteOffset = accessor.byteOffset;
						addition.componentType = static_cast<nxtDataType>(accessor.componentType);
						addition.count = static_cast<uint32_t>(accessor.count);
					}
					positionBuffer = buffer;
				}
				else if (attribute.first == "NORMALS")
				{
					hasNormals = true;
					layoutPosition = 1;
				}
				else if (attribute.first == "TANGENT")
				{
					layoutPosition = 2;
					addition.hasTangents = true;
				}
				else if (attribute.first == "TEXCOORD_0")
				{
					layoutPosition = 3;
				}
				else NXT_LOG_DEBUG("Attribute found that is not supported: {0}", attribute.first);

				if (layoutPosition >= 0)
				{
					arrayObject->SetLayoutPosition(layoutPosition, amount, static_cast<nxtDataType>(accessor.componentType), stride, static_cast<uint32_t>(accessor.byteOffset), accessor.normalized);
				}
			}

			if (!hasNormals)
			{
				GenerateNormals(addition);
			}

			primitives.push_back(addition);
		}
		return primitives;
	}

	static std::vector<Primitive> RegisterMesh(tinygltf::Model& model, tinygltf::Mesh& mesh, std::vector<buffers::SDataBuffer>& buffers)
	{
		using namespace buffers;
		std::vector<Primitive> primitives{};

		for (tinygltf::Primitive& primitive : mesh.primitives)
		{
			Primitive addPrimitive{};
			addPrimitive.arrayObject->Bind();
			SArrayObject arrayObject{ addPrimitive.arrayObject };

			// Check for indices;
			if (primitive.indices >= 0)
			{
				tinygltf::Accessor& indexAccessor{ model.accessors[primitive.indices] };
				addPrimitive.count = static_cast<uint32_t>(indexAccessor.count);
				addPrimitive.byteOffset = static_cast<uint32_t>(indexAccessor.byteOffset);
				addPrimitive.componentType = static_cast<nxtDataType>(indexAccessor.componentType);
				addPrimitive.hasIndices = true;
				gltf::Accessor& ac{ model.accessors[primitive.indices] };
				SDataBuffer& buf{ buffers[ac.bufferView] };
				addPrimitive.buffer = buf;
			}

			addPrimitive.mode = static_cast<nxtDrawMode>(primitive.mode);
			addPrimitive.material = primitive.material;
			
			bool tangentsFound{ false };
			for (auto& attribute : primitive.attributes)
			{
				// Binding buffers for attribute locationd data
				tinygltf::Accessor& accessor{ model.accessors[attribute.second] };
				
				Shared<DataBuffer>& dBuffer{ buffers[accessor.bufferView] };
				dBuffer->Bind();

				int32_t amount{ accessor.type == TINYGLTF_TYPE_SCALAR ? 1 : accessor.type };
				int32_t stride{ accessor.ByteStride(model.bufferViews[accessor.bufferView]) };

				if (accessor.sparse.isSparse)
				{
					NXT_LOG_WARN("Model has sparse accessor.");

					BufferDataModifier modifier{ dBuffer };
					tinygltf::Accessor::Sparse& sparse{ accessor.sparse };

					tinygltf::BufferView& indexView{ model.bufferViews[sparse.indices.bufferView] };
					void* indexData{ &model.buffers[indexView.buffer].data.at(indexView.byteOffset + sparse.indices.byteOffset) };

					// Index Data
					modifier.indices.resize(sparse.count);
					size_t byteSize{ sizeof(uint16_t) * sparse.count };
					memcpy_s(&modifier.indices.at(0), byteSize, indexData, byteSize);

					// Value Data
					tinygltf::BufferView& view{ model.bufferViews[sparse.values.bufferView] };
					ModifierInfo info{ static_cast<size_t>(sparse.values.byteOffset) };
					modifier.info.elementByteSize = SizeInBytes(accessor.componentType) * amount; // amount is making it a vec3, vec2, etc...
					modifier.info.targetByteOffset = accessor.byteOffset;
					modifier.info.targetByteStride = stride;
					modifier.info.dataBuffer = buffers[sparse.values.bufferView];

					addPrimitive.modifiers.push_back(modifier);
				}

				int32_t layoutPosition{ -1 };

				if (attribute.first == "POSITION")
				{
					layoutPosition = 0;
					if (!addPrimitive.hasIndices)
					{
						addPrimitive.buffer = dBuffer;
						addPrimitive.byteOffset = accessor.byteOffset;
						addPrimitive.componentType = static_cast<nxtDataType>(accessor.componentType);
						addPrimitive.count = static_cast<uint32_t>(accessor.count);
					}
				}
				else if (attribute.first == "NORMAL")
				{
					layoutPosition = 1;
				}
				else if (attribute.first == "TANGENT") 
				{
					layoutPosition = 2;
					tangentsFound = true;
					addPrimitive.hasTangents = true;
				}
				else if (attribute.first == "COLOR_0")
				{
					layoutPosition = 3;
				}
				else if (attribute.first == "TEXCOORD_0") 
				{
					layoutPosition = 4;
				}
				else NXT_LOG_DEBUG("Attribute found that is not supported: {0}", attribute.first);

				if (layoutPosition >= 0)
				{
					arrayObject->SetLayoutPosition(layoutPosition, amount, static_cast<nxtDataType>(accessor.componentType), stride, static_cast<uint32_t>(accessor.byteOffset), accessor.normalized);
				}
			}

			primitives.push_back(addPrimitive);
			addPrimitive.arrayObject->Unbind();
		}

		return primitives;
	}

	static Mesh RegisterNode(tinygltf::Model& model, tinygltf::Node& node, std::vector<buffers::SDataBuffer>& buffers)
	{
		//NXT_LOG_TRACE("Register Node");
		Mesh mesh{};
		if (node.mesh >= 0)
		{
			mesh.primitives = RegisterMesh(model, model.meshes[node.mesh], buffers);
		}

		if (!node.matrix.empty())
		{
			//NXT_LOG_TRACE("Matrix found");
			mesh.matrix = glm::make_mat4(&node.matrix.at(0));
		}
		else
		{
			// Translation rotation scale
			glm::mat4 ones{ 1.f };

			if (!node.scale.empty())
			{
				glm::vec3 sc{ glm::make_vec3(&node.scale.at(0)) };
				mesh.matrix = glm::scale(ones, sc) * mesh.matrix;
			}
			if (!node.rotation.empty())
			{
				glm::quat rot{ static_cast<float>(node.rotation.at(3)), static_cast<float>(node.rotation.at(0)), static_cast<float>(node.rotation.at(1)), static_cast<float>(node.rotation.at(2)) };
				mesh.matrix = glm::mat4_cast(rot) * mesh.matrix;
			}
			if (!node.translation.empty())
			{
				glm::vec3 pos{ glm::make_vec3(&node.translation.at(0)) };
				mesh.matrix = glm::translate(ones, pos) * mesh.matrix;
			}

		}

		for (int32_t& i : node.children)
		{
			mesh.children.push_back(RegisterNode(model, model.nodes[i], buffers));
		}
		return mesh;
	}

	static std::vector<Animation> RegisterAnimation(tinygltf::Model& model)
	{
		for (tinygltf::Animation& animation : model.animations)
		{
			Animation an{};
			for (tinygltf::AnimationSampler& sampler : animation.samplers)
			{

			}
		}
		return {};
	}

	// Rewrite to create all meshes first and create a contiguous vector of them
	// Then use a root mesh to start the render loop

	static std::vector<Mesh> RegisterModel(std::vector<STexture>& textures, std::vector<SMaterial>& materials, gltf::Model& model)
	{
		using namespace buffers;
		// Buffers
		std::vector<Shared<DataBuffer>> buffers{};
		for (tinygltf::BufferView& view : model.bufferViews)
		{
			// This creates a buffer for every view. Unused buffers will be deleted once the model creation is complete
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

		// Animations
		if (model.animations.size() > 0)
		{
			std::vector<Animation> animations{ RegisterAnimation(model) };
		}

		// Cycle nodes
		std::vector<Mesh> meshes;
		for (int32_t& i : model.scenes[model.defaultScene].nodes)
		{
			meshes.push_back(RegisterNode(model, model.nodes[i], buffers));
		}

		// MUST Unbind VAO before the buffers are deleted
		NXT_LOG_DEBUG("Buffers: {0}, Textures: {1}", buffers.size(), textures.size());
		//arrayObject->Unbind();
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
		NXT_LOG_DEBUG("MODEL: {0}", filepath.string());
		mMeshes = RegisterModel(mTextures, mMaterials, model);
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
