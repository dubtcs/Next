
#include "RenderSystem.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <nxt/render/ScreenQuad.h>
#include <nxt/core/utility/lerp.h>

static uint32_t gSamples{ 4 };
static bool drawNormals{ true };
static bool useBlinn{ true };

static nxt::STexture gDepthTexture;
static nxt::STexture gHDR;

static float gSizeScalar{ 1.f };
static constexpr float gSizeScalarMultiplier{ 2.f }; // size per sec

static constexpr uint32_t gMaxLights{ 10 };
struct LightInfo // 16 Byte Aligned
{
	glm::vec3 Position{ 0.f };	// 0 Offset,  16 byte alignment
	float Intensity{ 1.f };		// 12 Offset,  4 byte alignment
	glm::vec3 Direction{ 0.f };	// 16 Offset, 16 byte alignment
	uint32_t Type{ 0 };			// 28 Offset,  4 byte alignment
	glm::vec3 Color{ 1.f };		// 32 Offset, 16 byte alignment
	float Radius{ 0.f };		// 44 Offset,  4 byte alignment
};								// Total 48 bytes, 16 byte alignment
struct SceneLightData
{
	std::array<LightInfo, gMaxLights> LightData; // 0 Offset
	uint32_t lightsUsed{ 0 }; // 480 Offset
};
struct FrameInfoBuffer
{
	glm::mat4 projView;		// 0
	glm::mat4 proj;			// 64
	glm::mat4 projInv;		// 128
	glm::mat4 view;			// 192
	glm::mat4 viewInv;		// 256
	glm::mat4 normView;		// 320
	glm::vec3 position;		// 384
	int32_t xResolution;		// 396
	int32_t yResolution;		// 400
};
struct ObjectBufferInfo
{
	glm::mat4 normalMatrix;
	glm::mat4 worldMatrix;
	nxt::shader_mask lightingMask;
};
struct PrimitiveBufferInfo
{
	glm::mat4 primitiveMatrix; // should probably put this in promitive info??
	glm::vec4 baseColor;
	int32_t colorTextureIndex;
	float roughness;
	float metallic;
	int32_t normalTexture;
	int32_t emissionTexture;
	int32_t occlusionTexture;
	int32_t hasTan;
	int32_t hasMat;
};

namespace nxt
{

	RenderSystem::RenderSystem() :
		//mShader{ "assets/shaders/objects/obj5.vert", "assets/shaders/objects/obj5.frag" },
		mShader{ "assets/shaders/objects/deferred.vert", "assets/shaders/objects/deferred.frag" },
		mFrameInfoBuffer{ buffers::DataBuffer::Create(sizeof(FrameInfoBuffer), nullptr, nxtBufferTarget_UniformBuffer)},
		mLightInfoBuffer{ buffers::DataBuffer::Create(sizeof(SceneLightData), nullptr, nxtBufferTarget_UniformBuffer)},
		mObjectInfoBuffer{ buffers::DataBuffer::Create(sizeof(ObjectBufferInfo), nullptr, nxtBufferTarget_UniformBuffer) },
		mMaterialInfoBuffer{ buffers::DataBuffer::Create(sizeof(PrimitiveBufferInfo), nullptr, nxtBufferTarget_UniformBuffer) },
		mShadowmap{ 1024 },
		//mShadowShader{ "assets/shaders/shadows/shadowCube.vert", "assets/shaders/shadows/shadowCube.geom", "assets/shaders/shadows/shadowCube.frag" },
		mCamera{ {-2.f, 1.f, 2.f} },

		mBlurShader{ "assets/shaders/hdr/blur.vert", "assets/shaders/hdr/blur.frag" },
		mAOShader{ "assets/shaders/screen/quad.vert", "assets/shaders/screen/ssao.frag" },
		mAOBlurShader{ "assets/shaders/screen/quad.vert", "assets/shaders/screen/aoblur.frag" },
		mScreenQuad{  }
	{
		mShader.Bind();
		mShader.SetValue("useNormals", useBlinn);
		mShader.SetValue("depthMap", 0b1111);

		gDepthTexture = Texture::Create("assets/textures/Avocado_depthMap.png");
		gDepthTexture->Bind(15);

		// forgot to set the texture units for normals, so all textures were diverting to
		// the color texture causing normals look horrible.
		mShader.SetArrayValue("textures", { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });

		mFrameInfoBuffer->BindIndexed(0);
		mLightInfoBuffer->BindIndexed(1);
		mObjectInfoBuffer->BindIndexed(2);
		mMaterialInfoBuffer->BindIndexed(3);

		gHDR = Texture::Create(mWidth, mHeight, nxtTextureFormat_RGBAF, nxtTextureTarget_2D);

		mBlurShader.Bind();
		mBlurShader.SetValue("blurredTexture", 0);

		// SSAO
		mAOShader.Bind();
		// Kernel
		constexpr uint32_t sampleSize{ 64 };
		std::vector<glm::vec3> kernel{};
		kernel.reserve(sampleSize);
		for (uint32_t i{ 0 }; i < sampleSize; i++)
		{
			glm::vec3 sample{ random::GetNumber<float>(-1.f, 1.f), random::GetNumber<float>(-1.f, 1.f), random::GetNumber<float>(0.f, 1.f) };
			sample = glm::normalize(sample);
			float scale{ static_cast<float>(i) / static_cast<float>(sampleSize) };
			scale = Lerp(0.f, 1.f, scale, nxtInterpolation_Exponential);
			sample *= scale;
			kernel.push_back(sample);
		}
		mAOShader.SetArrayValue("kernel", kernel);
		std::vector<int32_t> teqwe{ 0, 1, 3 };
		mAOShader.SetArrayValue("gTextures", teqwe);

		mAOBlurShader.Bind();
		mAOBlurShader.SetValue("aoTexture", 3); // 3 is the texture unit set in update()

		mScreenQuad.toggleao(useBlinn);

		render::command::SetRenderFeature(nxtRenderFeature_Multisample, true);
		render::command::SetClearColor(0.f, 0.f, 0.f, 1.f);
	}

	static void DrawMesh(const SModel& model, const Mesh& mesh, buffers::SDataBuffer& objectInfo, glm::mat4& parentTransform)
	{
		using namespace buffers;

		glm::mat4 localTransform{ parentTransform * mesh.matrix };

		for (const Primitive& p : mesh.primitives)
		{
			p.arrayObject->Bind();
			SDataBuffer buffer{ p.buffer };
			bool hasMats{ (p.material >= 0) };

			objectInfo->SetSubData(sizeof(glm::mat4), 0, (void*)&localTransform);

			objectInfo->SetSubData(sizeof(bool), 104, (void*)&p.hasTangents);
			objectInfo->SetSubData(sizeof(bool), 108, &hasMats);

			if (hasMats)
			{
				const SMaterial& mat{ model->GetMaterials()[p.material] };
				objectInfo->SetSubData(sizeof(glm::vec4), 64, glm::value_ptr(mat->Properties.Color.BaseColor));
				objectInfo->SetSubData(sizeof(int32_t), 80, &mat->Properties.Color.Texture);

				objectInfo->SetSubData(sizeof(int32_t), 92, &mat->Textures.Normal);
				objectInfo->SetSubData(sizeof(int32_t), 96, &mat->Textures.Emissive);
				objectInfo->SetSubData(sizeof(int32_t), 100, &mat->Textures.Occlusion);
			}

			// sparse accessors
			for (const BufferDataModifier& mod : p.modifiers)
			{
				for (int32_t i{ 0 }; i < mod.indices.size(); i++)
				{
					uint16_t currentIndex{ mod.indices[i] };		// element size
					
					size_t readOffset{ mod.info.byteOffset + (mod.info.elementByteSize * i) };
					size_t writeOffset{ mod.info.targetByteOffset + (mod.info.targetByteStride * currentIndex) };

					mod.target->CopyBufferData(mod.info.dataBuffer, readOffset, writeOffset, mod.info.elementByteSize);
				}
			}

			buffer->Bind();
			if (p.hasIndices)
			{
				render::command::DrawElements(p.mode, p.count, p.componentType, (void*)(p.byteOffset));
			}
			else
			{
				render::command::DrawArrays(p.mode, p.count, 0);
			}
		}
		for (const Mesh& otherMesh : mesh.children)
		{
			DrawMesh(model, otherMesh, objectInfo, localTransform);
		}
	}

	static void DrawModel(const SModel& model, buffers::SDataBuffer& objectInfo)
	{
		int32_t i{ 0 };
		glm::mat4 ones{ 1.f };
		for (const STexture& tex : model->GetTextures())
		{
			tex->Bind(i++);
		}
		for (const Mesh& m : model->GetMeshes())
		{
			DrawMesh(model, m, objectInfo, ones);
		}
	}

	static void DrawMesh2(const Shared<Model2>& model, int32_t meshIndex, glm::mat4& parentTransform, const Shared<buffers::DataBuffer>& primitiveInfo, float dt)
	{
		Mesh2& mesh{ model->GetMeshes().at(meshIndex) };

		glm::mat4 localTransform{ mesh.matrix };

		if (mesh.animationInfo.inProgress && mesh.animationInfo.currentAnimation >= 0)
		{
			const Animation& animation{ model->GetAnimations().at(mesh.animationInfo.currentAnimation) };
			const AnimationTrack& track{ animation.tracks.at(mesh.animations.at(mesh.animationInfo.currentAnimation).at(0)) };

			// enforcing looping animations for now
			if (mesh.animationInfo.currentKeyframe == (track.timing.size() - 1))
			{
				mesh.animationInfo.runtime = 0.f;
				mesh.animationInfo.currentKeyframe = 0;
				mesh.animationInfo.timeStart = clock::GetTime();
			}

			int32_t nextKeyframe{ mesh.animationInfo.currentKeyframe + 1 };
			if (mesh.animationInfo.runtime >= track.timing.at(nextKeyframe))
			{
				mesh.animationInfo.currentKeyframe++;
				NXT_LOG_TRACE("Current keyframe: {0}", mesh.animationInfo.currentKeyframe);
			}

			if (track.animationTarget == nxtAnimationTarget_Rotation)
			{

			}

			mesh.animationInfo.runtime += dt;

		}

		localTransform = parentTransform * localTransform;

		for (const Primitive& primitive : mesh.primitives)
		{
			primitive.arrayObject->Bind();
			bool hasMaterials{ (primitive.material >= 0) };

			primitiveInfo->SetSubData(sizeof(glm::mat4), 0, &localTransform);
			primitiveInfo->SetSubData(sizeof(bool), 104, (void*)&primitive.hasTangents);
			primitiveInfo->SetSubData(sizeof(bool), 108, &hasMaterials);

			// Push Material Information Into Buffer
			if (hasMaterials)
			{
				const SMaterial& mat{ model->GetMaterials()[primitive.material] };
				primitiveInfo->SetSubData(sizeof(glm::vec4), 64, glm::value_ptr(mat->Properties.Color.BaseColor));
				primitiveInfo->SetSubData(sizeof(int32_t), 80, &mat->Properties.Color.Texture);

				primitiveInfo->SetSubData(sizeof(int32_t), 92, &mat->Textures.Normal);
				primitiveInfo->SetSubData(sizeof(int32_t), 96, &mat->Textures.Emissive);
				primitiveInfo->SetSubData(sizeof(int32_t), 100, &mat->Textures.Occlusion);
			}

			// Adjust buffer data for sparse accessors
			for (const BufferDataModifier& mod : primitive.modifiers)
			{
				for (int32_t i{ 0 }; i < mod.indices.size(); i++)
				{
					uint16_t currentIndex{ mod.indices[i] }; // element size

					size_t readOffset{ mod.info.byteOffset + (mod.info.elementByteSize * i) };
					size_t writeOffset{ mod.info.targetByteOffset + (mod.info.targetByteStride * currentIndex) };

					mod.target->CopyBufferData(mod.info.dataBuffer, readOffset, writeOffset, mod.info.elementByteSize);
				}
			}

			primitive.buffer->Bind();
			if (primitive.hasIndices)
			{
				render::command::DrawElements(primitive.mode, primitive.count, primitive.componentType, (void*)(primitive.byteOffset));
			}
			else
			{
				render::command::DrawArrays(primitive.mode, primitive.count, 0);
			}

		}

		for (int32_t childIndex : mesh.children)
		{
			DrawMesh2(model, childIndex, localTransform, primitiveInfo, dt);
		}
	}

	static void DrawModel2(const Shared<Model2>& model, Shared<buffers::DataBuffer>& primitiveInfo, float dt)
	{
		glm::mat4 ones{ 1.f };
		const Model2::Scene& scene{ model->GetScenes().at(model->mRootScene) };
		int32_t i{ 0 };
		for (const Shared<FrameTexture>& tex : model->GetTextures())
		{
			tex->BindToUnit(i++);
		}

		// Instead of looping here
		// Can save the animation track indices in the mesh they belong to and apply the animation there
		// Removing this loop at the cost of higher memory usage

		// Test loop for printing data
		//for (const Animation& animation : model->GetAnimations())
		//{
		//	for (const AnimationTrack& track : animation.tracks)
		//	{
		//		NXT_LOG_DEBUG("NEW ANIMATION");
		//		for (int32_t i{ 0 }; i < track.timing.size(); i++)
		//		{
		//			if (track.indicesPerElement == 4)
		//			{
		//				NXT_LOG_TRACE("\nKeyframe: {0}\n\tTiming: {1}\n\tValue: {2}", i, track.timing.at(i), glm::to_string(glm::make_vec4(&track.data.at(i * track.indicesPerElement))));
		//			}
		//		}
		//	}
		//}
		for (int32_t meshIndex : scene)
		{
			DrawMesh2(model, meshIndex, ones, primitiveInfo, dt);
		}
	}

	static void StripMatrixTranslation(glm::mat4* m)
	{
		(*m)[0].w = 0.f;
		(*m)[1].w = 0.f;
		(*m)[2].w = 0.f;
	}

	void RenderSystem::OnUpdate(float& dt, World& world, bool isFocused)
	{
		if(isFocused)
			mCamera.OnUpdate(dt);

		if (input::IsKeyDown(nxtKeycode_Plus))
		{
			gSizeScalar += dt * gSizeScalarMultiplier * gSizeScalar;
		}
		else if (input::IsKeyDown(nxtKeycode_Minus))
		{
			float temp{ gSizeScalar - (dt * gSizeScalarMultiplier * gSizeScalar) };
			gSizeScalar = std::max(0.01f, temp);
		}

		//glm::mat4 normalViewMatrix{ glm::transpose(glm::inverse(mCamera.GetViewMatrix())) }; // view space normal matrix
		//StripMatrixTranslation(&normalViewMatrix);

		glm::mat4 normalViewMatrix{ mCamera.GetViewMatrix() };
		normalViewMatrix = glm::transpose(glm::inverse(normalViewMatrix));

		glm::mat4 projInv{ glm::inverse(mCamera.GetProjectionMatrix()) };
		glm::mat4 viewInv{ glm::inverse(mCamera.GetViewMatrix()) };

		mFrameInfoBuffer->SetSubData(64, 0, glm::value_ptr(mCamera.GetProjectionViewMatrix()));
		mFrameInfoBuffer->SetSubData(64, 64, glm::value_ptr(mCamera.GetProjectionMatrix()));
		mFrameInfoBuffer->SetSubData(64, 128, glm::value_ptr(projInv));
		mFrameInfoBuffer->SetSubData(64, 192, glm::value_ptr(mCamera.GetViewMatrix()));
		mFrameInfoBuffer->SetSubData(64, 256, glm::value_ptr(viewInv));
		mFrameInfoBuffer->SetSubData(64, 320, glm::value_ptr(normalViewMatrix));
		mFrameInfoBuffer->SetSubData(12, 384, (void*)glm::value_ptr(mCamera.GetPosition()));

		//NXT_LOG_TRACE(glm::to_string(mCamera.GetViewMatrix()));

		glm::mat4 ones{ 1.f };

		// Push light data to buffer
		necs::SceneView<cmp::Light> lightView{ world.GetScene() };

		int32_t i{ 0 };
		cmp::Light& lc{ world.GetComponent<cmp::Light>(*lightView.begin()) };
		float circleMagnitude{ 10.f };
		lc.Position = glm::vec3{ -std::sin(clock::GetRunTime()) * circleMagnitude, 0.f, std::cos(clock::GetRunTime()) * circleMagnitude };
		for (const necs::Entity& e : lightView)
		{
			cmp::Light& l{ world.GetComponent<cmp::Light>(e) };
			if (l.LightType == cmp::nxtLightType_Spot)
			{
				l.Direction = mCamera.GetLookVector();
				l.Position = mCamera.GetPosition();
			}
			mLightInfoBuffer->SetSubData(sizeof(cmp::Light), sizeof(cmp::Light) * i++, &l);
		}
		mLightInfoBuffer->SetSubData(4, 480, &i);

		// Visible render pass
		mDeferredBuffer->Bind();

		render::command::Clear();
		mShader.Bind();

		necs::SceneView<cmp::Transform, cmp::WorldModel> view{ world.GetScene() };
		for (const necs::Entity& e : view)
		{
			cmp::Transform& t{ world.GetComponent<cmp::Transform>(e) };
			glm::mat4 worldMatrix{ glm::translate(ones, t.Position)
				//* glm::rotate(ones, t.Scale) // quaternions required
				* glm::scale(ones, t.Scale * gSizeScalar)
			};
			glm::mat4 normalMatrix{ glm::transpose(glm::inverse(worldMatrix)) };
			//glm::mat4 normalViewMatrix{ glm::transpose(glm::inverse(mCamera.GetViewMatrix())) }; // view space normal matrix

			cmp::WorldModel& m{ world.GetComponent<cmp::WorldModel>(e) };
			shader_mask mask{ 0 };
			mask |= SETBIT((m.Instance->Model->GetTextures().size() > 0), 0);
			mask |= SETBIT((!world.HasComponent<cmp::Light>(e)), 1);
			mObjectInfoBuffer->SetSubData(64, 0, glm::value_ptr(normalMatrix));
			mObjectInfoBuffer->SetSubData(64, 64, glm::value_ptr(worldMatrix));
			mObjectInfoBuffer->SetSubData(4, 128, &mask);

			DrawModel2(m.Instance->Model, mMaterialInfoBuffer, dt);
		}

		mDeferredBuffer->Unbind();

		for (int32_t i{ 0 }; i < mDeferredBuffer->GetAttachmentCount(); i++)
		{
			mDeferredBuffer->GetTexture(i)->BindToUnit(i);
		}

		// SSAO
		mDeferredBuffer->GetDepthTexture()->BindToUnit(0);
		mNoise->BindToUnit(3);
		mSSAO->Bind();
		mAOShader.Bind();
		mScreenQuad.DrawNoShader();
		mSSAO->Unbind();
		mSSAO->GetTexture(0)->BindToUnit(3); // ding ding, ref mAOBlurShader.SetValue("aoTexture", 3)

		// blurring AO
		mAOBlur->Bind();
		mAOBlurShader.Bind();
		mScreenQuad.DrawNoShader();
		mAOBlur->GetTexture(0)->BindToUnit(3);
		mAOBlur->Unbind();

		mScreenQuad.Draw();

	}
	
	bool RenderSystem::OnEvent(events::Event& ev)
	{
		events::Handler handler{ ev };
		handler.Fire<events::WindowResized>(NXT_CALLBACK(RenderSystem::OnWindowResize));
		handler.Fire<events::KeyboardPressed>(NXT_CALLBACK(RenderSystem::OnKeyPressed));
		mCamera.OnEvent(ev);
		return false;
	}

	bool RenderSystem::OnWindowResize(events::WindowResized& ev)
	{
		mWidth = ev.Width;
		mHeight = ev.Height;

		SFrameTexture t1{ NewShared<FrameTexture>(mWidth, mHeight, gSamples, nxtTextureFormat_RGBA, nxtTextureFormatInternal_RGBA16F) };
		SFrameTexture t2{ NewShared<FrameTexture>(mWidth, mHeight, gSamples, nxtTextureFormat_RGBA, nxtTextureFormatInternal_RGBA16F) };
		mBuffer2 = NewShared<FrameBuffer>(t1);
		mBuffer2->AttachTexture(t2, nxtTextureAttachment_Color0 + 1);

		SFrameTexture b1{ NewShared<FrameTexture>(mWidth, mHeight, 1, nxtTextureFormat_RGBA, nxtTextureFormatInternal_RGB16F) };
		mBlurs[0] = NewShared<FrameBuffer>(b1);

		SFrameTexture b2{ NewShared<FrameTexture>(mWidth, mHeight, 1, nxtTextureFormat_RGBA, nxtTextureFormatInternal_RGB16F) };
		mBlurs[1] = NewShared<FrameBuffer>(b2);

		mFrameInfoBuffer->SetSubData(4, 396, &mWidth);
		mFrameInfoBuffer->SetSubData(4, 400, &mHeight);

		BuildDeferredBuffer();

		render::command::SetViewport(ev.Width, ev.Height);
		return false;
	}

	bool RenderSystem::OnKeyPressed(events::KeyboardPressed& ev)
	{
		bool rebuildBuffer{ false };
		if (ev.Keycode == nxtKeycode_Left)
		{
			float newVal = std::max(1.f, gSamples * 0.5f);
			if (newVal != gSamples)
			{
				gSamples = static_cast<uint32_t>(newVal);
				rebuildBuffer = true;
			}
		}
		else if (ev.Keycode == nxtKeycode_Right)
		{
			float newVal = std::min(4.f, gSamples * 2.f);
			if (newVal != gSamples)
			{
				gSamples = static_cast<uint32_t>(newVal);
				rebuildBuffer = true;
			}
		}
		else if (ev.Keycode == nxtKeycode_I)
		{
			NXT_LOG_WARN("Deleting deffered normal buffer");
			mDeferredBuffer->DetachTexture(nxtTextureAttachment_Color0 + 1);
		}

		if (rebuildBuffer)
		{
			SFrameTexture t1{ NewShared<FrameTexture>(mWidth, mHeight, gSamples, nxtTextureFormat_RGBA, nxtTextureFormatInternal_RGBA16F) };
			SFrameTexture t2{ NewShared<FrameTexture>(mWidth, mHeight, gSamples, nxtTextureFormat_RGBA, nxtTextureFormatInternal_RGBA16F) };
			mBuffer2 = NewShared<FrameBuffer>(t1);
			mBuffer2->AttachTexture(t2, nxtTextureAttachment_Color0 + 1);

			SFrameTexture b1{ NewShared<FrameTexture>(mWidth, mHeight, 1, nxtTextureFormat_RGBA, nxtTextureFormatInternal_RGB16F) };
			mBlurs[0] = NewShared<FrameBuffer>(b1);

			SFrameTexture b2{ NewShared<FrameTexture>(mWidth, mHeight, 1, nxtTextureFormat_RGBA, nxtTextureFormatInternal_RGB16F) };
			mBlurs[1] = NewShared<FrameBuffer>(b2);

			BuildDeferredBuffer();

			NXT_LOG_TRACE("MSAA {0}", gSamples);
		}

		if (ev.Keycode == nxtKeycode_N)
		{
			drawNormals = !drawNormals;
			NXT_LOG_TRACE("{0} using framebuffer texture.", drawNormals ? "Now" : "No longer");
		}

		if (ev.Keycode == nxtKeycode_B)
		{
			useBlinn = !useBlinn;
			NXT_LOG_TRACE("AO: {0}", useBlinn);
			mScreenQuad.toggleao(useBlinn);
		}

		if (ev.Keycode == nxtKeycode_G)
		{
			static bool jj{ true };
			jj = !jj;
			NXT_LOG_TRACE("Gamma correction: {0}", jj);
			render::command::SetRenderFeature(nxtRenderFeature_FrameBuffer_sRGB, jj);
		}

		return false;
	}

	void RenderSystem::BuildDeferredBuffer()
	{
		int32_t SAMPLES{ 1 }; // enforcing 1 sample for now
		SFrameTexture positions{ NewShared<FrameTexture>(mWidth, mHeight, SAMPLES, nxtTextureFormat_RGBAF, nxtTextureFormatInternal_RGBA16F) };
		SFrameTexture normals{ NewShared<FrameTexture>(mWidth, mHeight, SAMPLES, nxtTextureFormat_RGBAF, nxtTextureFormatInternal_RGBA16F) };
		SFrameTexture colors{ NewShared<FrameTexture>(mWidth, mHeight, SAMPLES, nxtTextureFormat_RGBAF, nxtTextureFormatInternal_RGBA16F) };
		SFrameTexture depth{ NewShared<FrameTexture>(mWidth, mHeight, SAMPLES, nxtTextureFormat_DepthStencil, nxtTextureFormatInternal_Depth24Stencil8) };

		mDeferredBuffer = NewShared<FrameBuffer>(positions);
		mDeferredBuffer->AttachTexture(normals, nxtTextureAttachment_Color0 + 1);
		mDeferredBuffer->AttachTexture(colors, nxtTextureAttachment_Color0 + 2);
		mDeferredBuffer->AttachTexture(depth, nxtTextureAttachment_Depth);

		// SSAO

		// Noise
		constexpr uint32_t nSamples{ 16 };
		std::vector<glm::vec3> noiseData{};
		noiseData.reserve(nSamples);
		for (uint32_t i{ 0 }; i < nSamples; i++)
		{
			// Z is 0 for hemisphere
			glm::vec3 n{ random::GetNumber<float>(-1.f, 1.f), random::GetNumber<float>(-1.f, 1.f), 0.f };
			n = glm::normalize(n);
			noiseData.push_back(n);
		}

		// 4x4 texture, 16 length array
		SFrameTexture noise{ NewShared<FrameTexture>(4, 4, 1, nxtTextureFormat_RGBA, nxtTextureFormatInternal_RGBA16F) };
		noise->SetParameter(nxtTextureParamName_WrapS, nxtTextureParam_Repeat);
		noise->SetParameter(nxtTextureParamName_WrapT, nxtTextureParam_Repeat);
		noise->SetData(nxtTextureFormat_RGBA, nxtDataType_Float, &noiseData[0]);
		mNoise = noise;

		SFrameTexture aoColor{ NewShared<FrameTexture>(mWidth, mHeight, SAMPLES, nxtTextureFormat_R, nxtTextureFormatInternal_R16) };
		mSSAO = NewShared<FrameBuffer>(aoColor);

		SFrameTexture aoBlur{ NewShared<FrameTexture>(mWidth, mHeight, SAMPLES, nxtTextureFormat_R, nxtTextureFormatInternal_R16) };
		mAOBlur = NewShared<FrameBuffer>(aoBlur);

		//mSSAO->AttachTexture(noise, nxtTextureAttachment_Color0 + 1);
		// Adding it to the buffer to hold the pointer.
		// Just don't write to location = 1
		// can't do that lmao
		// render command clears it

	}

}
