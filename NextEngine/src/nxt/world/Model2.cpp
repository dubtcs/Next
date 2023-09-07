
#include "Model2.h"

#include <nxt/render/buffers/DataBuffer.h>

#include <nxt/core/utility/TypeToSize.h>

#include <tiny_gltf.h>

namespace gltf = tinygltf;

namespace nxt
{

	static std::vector<Shared<buffers::DataBuffer>> RegisterBuffers(gltf::Model& model)
	{
		using namespace buffers;
		std::vector<Shared<DataBuffer>> buffs{};
		for (tinygltf::BufferView& view : model.bufferViews)
		{
			// This creates a buffer for every view. Unused buffers will be deleted once the model creation is complete
			tinygltf::Buffer& buffer{ model.buffers[view.buffer] };
			Shared<DataBuffer> dBuffer{ DataBuffer::Create(view.byteLength, &buffer.data.at(0) + view.byteOffset, static_cast<nxtBufferTarget>(view.target)) };
			buffs.push_back(dBuffer);
		}
		return buffs;
	}

	static void RegisterTextures(std::vector<Shared<FrameTexture>>& textures, gltf::Model& model)
	{
		if (model.textures.size() > 0)
		{
			for (tinygltf::Texture& texture : model.textures)
			{
				tinygltf::Image& image{ model.images[texture.source] };

				nxtTextureFormat format{ 0 };
				nxtTextureFormatInternal internal{ 0 };
				switch (image.component)
				{
					case(1):
					{
						format = nxtTextureFormat_R;
						internal = nxtTextureFormatInternal_R8;
						break;
					}
					case(2):
					{
						format = nxtTextureFormat_RG;
						internal = nxtTextureFormatInternal_RG8;
						break;
					}
					case(3):
					{
						format = nxtTextureFormat_RGB;
						internal = nxtTextureFormatInternal_RGB8;
						break;
					}
					case(4):
					{
						format = nxtTextureFormat_RGBA;
						internal = nxtTextureFormatInternal_RGBA8;
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

				Shared<FrameTexture> tex{ NewShared<FrameTexture>(image.width, image.height, 1, format, internal) };
				tex->SetData(format, dataType, &image.image.at(0));
				textures.push_back(tex);

			}
		}
	}

	static void RegisterMaterials(std::vector<Shared<Material>>& materials, gltf::Model& model)
	{
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
	}

	static void RegisterAnimations(std::vector<Animation>& animations, gltf::Model& model)
	{
		if (!model.animations.empty())
		{
			// On^2 dhawouhodawhuioawd
			for (gltf::Animation& a : model.animations)
			{
				Animation animation{};
				for (gltf::AnimationChannel& channel : a.channels)
				{
					AnimationTrack track{ channel.target_node };

					animation.tracks.push_back(track);
				}
				animations.push_back(animation);
			}
		}
	}

	static std::vector<Primitive> RegisterMesh(gltf::Mesh& node, gltf::Model& model, std::vector<Shared<buffers::DataBuffer>>& buffers)
	{
		std::vector<Primitive> prims{};

		using namespace buffers;
		for (gltf::Primitive& prim : node.primitives)
		{
			Primitive add{};
			add.arrayObject->Bind();

			if (prim.indices >= 0)
			{
				gltf::Accessor& ac{ model.accessors[prim.indices] };
				add.count = static_cast<uint32_t>(ac.count);
				add.byteOffset = ac.byteOffset;
				add.componentType = static_cast<nxtDataType>(ac.componentType);
				add.hasIndices = true;
				add.buffer = buffers[ac.bufferView];
			}

			add.mode = static_cast<nxtDrawMode>(prim.mode);
			add.material = prim.material;

			for (auto& attribute : prim.attributes)
			{
				gltf::Accessor& ac{ model.accessors[attribute.second] };

				Shared<DataBuffer> buffer{ buffers[ac.bufferView] };
				buffer->Bind();

				int32_t amount{ ac.type == TINYGLTF_TYPE_SCALAR ? 1 : ac.type };
				int32_t stride{ ac.ByteStride(model.bufferViews[ac.bufferView]) };

				if (ac.sparse.isSparse)
				{
					NXT_LOG_WARN("Sparse accessor found.");
					BufferDataModifier mod{ buffer };
					gltf::Accessor::Sparse& sparse{ ac.sparse };

					gltf::BufferView& indexView{ model.bufferViews[sparse.indices.bufferView] };
					void* indexData{ &model.buffers[indexView.buffer].data.at(indexView.byteOffset + sparse.indices.byteOffset) };

					// Index Data
					mod.indices.resize(sparse.count);
					size_t byteSize{ sizeof(uint16_t) * sparse.count };
					memcpy_s(&mod.indices.at(0), byteSize, indexData, byteSize);

					// Value Data
					tinygltf::BufferView& view{ model.bufferViews[sparse.values.bufferView] };
					ModifierInfo info{ static_cast<size_t>(sparse.values.byteOffset) };
					mod.info.elementByteSize = SizeInBytes(ac.componentType) * amount; // amount is making it a vec3, vec2, etc...
					mod.info.targetByteOffset = ac.byteOffset;
					mod.info.targetByteStride = stride;
					mod.info.dataBuffer = buffers[sparse.values.bufferView];

					add.modifiers.push_back(mod);
				}

				int32_t layoutPosition{ -1 };
				if (attribute.first == "POSITION")
				{
					layoutPosition = 0;
					if (!add.hasIndices)
					{
						add.buffer = buffer;
						add.byteOffset = ac.byteOffset;
						add.componentType = static_cast<nxtDataType>(ac.componentType);
						add.count = static_cast<uint32_t>(ac.count);
					}
				}
				else if (attribute.first == "NORMAL")
				{
					layoutPosition = 1;
				}
				else if (attribute.first == "TANGENT")
				{
					layoutPosition = 2;
					add.hasTangents = true;
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
					add.arrayObject->SetLayoutPosition(layoutPosition, amount, static_cast<nxtDataType>(ac.componentType), stride, static_cast<uint32_t>(ac.byteOffset), ac.normalized);
				}

			}

			prims.push_back(add);
			add.arrayObject->Unbind();
		}

		return prims;
	}

	static std::vector<Mesh2> RegisterNodes(gltf::Model& model, std::vector<Shared<buffers::DataBuffer>>& buffers)
	{
		std::vector<Mesh2> meshes{};
		for (gltf::Node& n : model.nodes)
		{
			Mesh2 temp{};
			if (n.mesh >= 0)
			{
				temp.primitives = RegisterMesh(model.meshes[n.mesh], model, buffers);
			}
			temp.children = n.children;
			meshes.push_back(temp);
		}
		return meshes;
	}

	static std::vector<Mesh2> RegisterModel2(std::vector<Shared<FrameTexture>>& textures, std::vector<Shared<Material>>& materials, std::vector<Animation>& animations, gltf::Model& model)
	{
		std::vector<Shared<buffers::DataBuffer>> buffers{ RegisterBuffers(model) };
		RegisterTextures(textures, model);
		RegisterMaterials(materials, model);
		RegisterAnimations(animations, model);
		return RegisterNodes(model, buffers);
	}

	Model2::Model2(const std::filesystem::path& filepath)
	{
		tinygltf::TinyGLTF loader;
		std::string msgError;
		std::string msgWarn;
		tinygltf::Model model;
		if (filepath.extension().string() == ".gltf")
		{
			bool pass{ loader.LoadASCIIFromFile(&model, &msgError, &msgWarn, filepath.string()) };
		}
		else if (filepath.extension().string() == ".glb")
		{
			bool pass{ loader.LoadBinaryFromFile(&model, &msgError, &msgWarn, filepath.string()) };
		}
		else
		{
			NXT_LOG_CRIT("Invalid file extension: {0}", filepath.extension().string());
			return;
		}
		NXT_LOG_DEBUG("MODEL: {0}", filepath.string());

		// Load Scene Info
		for (gltf::Scene& s : model.scenes)
		{
			mScenes.push_back(s.nodes);
		}

		mMeshes = RegisterModel2(mTextures, mMaterials, mAnimations, model);
	}

	Model2::~Model2()
	{

	}

	const std::vector<Mesh2>& Model2::GetMeshes() const { return mMeshes; }
	const std::vector<Model2::Scene>& Model2::GetScenes() const { return mScenes; }
	const std::vector<Animation>& Model2::GetAnimations() const { return mAnimations; }
	const std::vector<Shared<Material>>& Model2::GetMaterials() const {	return mMaterials; }
	const std::vector<Shared<FrameTexture>>& Model2::GetTextures() const { return mTextures; }

}
