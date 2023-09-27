
#include "Model2.h"

#include <nxt/render/buffers/DataBuffer.h>

#include <nxt/core/utility/TypeToSize.h>

#include <tiny_gltf.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

namespace gltf = tinygltf;

namespace nxt
{

	static nxtInterpolation GetInterpolationTypeFromString(const std::string& name)
	{
		if (name == "LINEAR")
			return nxtInterpolation_Linear;
		else if (name == "EXPONENTIAL")
			return nxtInterpolation_Exponential;

		// default
		return nxtInterpolation_Linear;
	}

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
				nxtTextureFormatInternal internalFormat{ 0 };
				switch (image.component)
				{
					case(1):
					{
						format = nxtTextureFormat_R;
						internalFormat = nxtTextureFormatInternal_R8;
						break;
					}
					case(2):
					{
						format = nxtTextureFormat_RG;
						internalFormat = nxtTextureFormatInternal_RG8;
						break;
					}
					case(3):
					{
						format = nxtTextureFormat_RGB;
						internalFormat = nxtTextureFormatInternal_RGB8;
						break;
					}
					case(4):
					{
						format = nxtTextureFormat_RGBA;
						internalFormat = nxtTextureFormatInternal_RGBA8;
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

				Shared<FrameTexture> tex{ NewShared<FrameTexture>(image.width, image.height, 1, format, internalFormat) };
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

	static void RegisterAnimations(std::vector<Animation>& animations, std::vector<Shared<buffers::DataBuffer>>& buffers, std::vector<Mesh2>& meshes, gltf::Model& model)
	{
		if (!model.animations.empty())
		{
			// On^2 dhawouhodawhuioawd
			int32_t animationIndex{ 0 };
			for (gltf::Animation& a : model.animations) {
				Animation animation{};

				int32_t trackIndex{ 0 };
				for (gltf::AnimationChannel& channel : a.channels)
				{
					AnimationTrack track{ channel.target_node };
					Mesh2& mesh{ meshes[channel.target_node] };
					mesh.animations.emplace(animationIndex, trackIndex);

					//meshes[channel.target_node].animations[animationIndex].push_back(trackIndex);
					//meshes[channel.target_node].animationInfo.inProgress = true;
					//meshes[channel.target_node].animationInfo.currentAnimation = 0;

					// From what I can tell from gltf repo, only one channel per sampler
					gltf::AnimationSampler& sampler{ a.samplers[channel.sampler] };
					track.interpolation = GetInterpolationTypeFromString(sampler.interpolation);

					// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					// This assumes all data is packed.
					// Account for data interleaving in the future
					// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

					// Input
					// Can maybe just store an index to a timing vector instead of storing duplicates in each track. Some tracks will have the same keyframe timing
					gltf::Accessor& inputAccessor{ model.accessors[sampler.input] };
					size_t count{ inputAccessor.count };

					gltf::BufferView& inputView{ model.bufferViews[inputAccessor.bufferView] };
					gltf::Buffer& inputBuffer{ model.buffers[inputView.buffer] };

					int32_t inStride{ inputAccessor.ByteStride(inputView) };
					if (inStride > 4) // 4 is size of float
					{
						NXT_LOG_WARN("Non packed timing buffer.");
					}

					track.timing.resize(count);
					size_t dataSize{ count * sizeof(float) };
					memcpy_s(&track.timing.at(0), dataSize, &inputBuffer.data.at(0) + inputView.byteOffset + inputAccessor.byteOffset, dataSize);

					// are all animation samplers the same runtime? If so, set this once and be done
					animation.totalRuntime = std::max(animation.totalRuntime, track.timing.back());
					//

					// Output
					gltf::Accessor& outAccessor{ model.accessors[sampler.output] };
					int32_t amountPerElement{ outAccessor.type == TINYGLTF_TYPE_SCALAR ? 1 : outAccessor.type };
					gltf::BufferView& outView{ model.bufferViews[outAccessor.bufferView] };
					gltf::Buffer& outBuffer{ model.buffers[outView.buffer] };

					NXT_LOG_INFO("Data");
					NXT_LOG_TRACE("Stride: {0}", outAccessor.ByteStride(outView));

					int32_t outStride{ outAccessor.ByteStride(outView) };
					if (outStride > (sizeof(float) * amountPerElement))
					{
						NXT_LOG_WARN("Non packed data buffer.");
					}

					track.indicesPerElement = amountPerElement;
					count *= amountPerElement;
					dataSize = count * sizeof(float);
					track.data.resize(count);
					memcpy_s(&track.data.at(0), dataSize, &outBuffer.data.at(0) + outView.byteOffset + outAccessor.byteOffset, dataSize);
					//

					if (channel.target_path == "translation")
						track.animationTarget = nxtAnimationTarget_Position;
					else if (channel.target_path == "rotation")
						track.animationTarget = nxtAnimationTarget_Rotation;
					else if (channel.target_path == "scale")
						track.animationTarget = nxtAnimationTarget_Scale;
					else if (channel.target_path == "weights")
						track.animationTarget = nxtAnimationTarget_Weights;

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

			using morphmap = std::map<std::string, int32_t>;
			for (int32_t i{ 0 }; i < prim.targets.size(); i++)
			{
				morphmap& target{ prim.targets[i] };
				int32_t mask{ 0 };
				if (target.contains("POSITION"))
				{
					mask |= nxtMorphTarget_Position;
				}
				if (target.contains("NORMAL"))
				{
					mask |= nxtMorphTarget_Normal;
				}
				if (target.contains("TANGENT"))
				{
					mask |= nxtMorphTarget_Tangent;
				}

				Morph morph{ static_cast<float>(node.weights[i]), mask };
			}

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
					add.features |= nxtPrimitiveFeature_Normals;
				}
				else if (attribute.first == "TANGENT")
				{
					layoutPosition = 2;
					add.hasTangents = true;
					add.features |= nxtPrimitiveFeature_Tangents;
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
		for (gltf::Node& node : model.nodes)
		{
			Mesh2 mesh{};
			if (node.mesh >= 0)
			{
				mesh.primitives = RegisterMesh(model.meshes[node.mesh], model, buffers);
			}

			if (!node.matrix.empty())
			{
				glm::mat4 matrix{ glm::make_mat4(&node.matrix.at(0)) };

				// extract translation
				mesh.matrix.position = matrix[3];
				matrix[3] = glm::vec4{ 0.f, 0.f, 0.f, 1.f }; // remove translation

				// negative scaling wont work. Fix it
				mesh.matrix.scale = glm::vec3{ glm::length(matrix[0]), glm::length(matrix[1]), glm::length(matrix[2]) };

				glm::mat4 rotation{
					matrix[0] / mesh.matrix.scale.x,
					matrix[1] / mesh.matrix.scale.y,
					matrix[2] / mesh.matrix.scale.z,
					glm::vec4{ 0.f, 0.f, 0.f, 1.f} 
				};

				glm::quat q{ glm::quat_cast(rotation) };
				mesh.matrix.rotation = { q.x, q.y, q.z, q.w };
			}
			else
			{
				// Translation rotation scale
				if (!node.scale.empty())
				{
					glm::vec3 sc{ glm::make_vec3(&node.scale.at(0)) };
					mesh.matrix.scale = sc;
				}
				if (!node.rotation.empty())
				{
					glm::quat rot{ static_cast<float>(node.rotation.at(3)), static_cast<float>(node.rotation.at(0)), static_cast<float>(node.rotation.at(1)), static_cast<float>(node.rotation.at(2)) };
					mesh.matrix.rotation = { rot.x, rot.y, rot.z, rot.w };
				}
				if (!node.translation.empty())
				{
					glm::vec3 pos{ glm::make_vec3(&node.translation.at(0)) };
					mesh.matrix.position = pos;
				}

			}

			mesh.children = node.children;
			meshes.push_back(mesh);
		}
		return meshes;
	}

	static std::vector<Mesh2> RegisterModel2(std::vector<Shared<FrameTexture>>& textures, std::vector<Shared<Material>>& materials, std::vector<Animation>& animations, gltf::Model& model)
	{
		std::vector<Shared<buffers::DataBuffer>> buffers{ RegisterBuffers(model) };
		std::vector<Mesh2> meshes{ RegisterNodes(model, buffers) };
		RegisterTextures(textures, model);
		RegisterMaterials(materials, model);
		RegisterAnimations(animations, buffers, meshes, model);
		return meshes;
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
		NXT_LOG_INFO("MODEL: {0}", filepath.string());

		// Load Scene Info
		for (gltf::Scene& s : model.scenes)
		{
			mScenes.push_back(s.nodes);
		}
		mRootScene = model.defaultScene;

		mMeshes = RegisterModel2(mTextures, mMaterials, mAnimations, model);
		NXT_LOG_INFO("INFO:\n\tMeshes: {0}\n\tTextures: {1}\n\tMaterials: {2}\n\tAnimations: {3}", mMeshes.size(), mTextures.size(), mMaterials.size(), mAnimations.size());
	}

	Model2::~Model2()
	{

	}

	bool Model2::PlayAnimation(int32_t animationIndex, bool loop)
	{
		if ((animationIndex >= 0) && (animationIndex < mAnimations.size()))
		{
			// construct new obj so I don't have to comeback and add reset lines every time a change is made to the struct
			mAnimation = { animationIndex, loop };
			return true;
		}
		return false;
	}

	bool Model2::PauseAnimation()
	{
		mAnimation.isPaused = !mAnimation.isPaused;
		return true;
	}

	bool Model2::StopAnimation()
	{
		mAnimation = AnimationInfo{};
		return true;
	}

	std::vector<Mesh2>& Model2::GetMeshes() { return mMeshes; }
	std::vector<Model2::Scene>& Model2::GetScenes() { return mScenes; }
	std::vector<Animation>& Model2::GetAnimations() { return mAnimations; }
	std::vector<Shared<Material>>& Model2::GetMaterials() {	return mMaterials; }
	std::vector<Shared<FrameTexture>>& Model2::GetTextures() { return mTextures; }

}
