
#include "RenderSystem.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <nxt/render/ScreenQuad.h>

static uint32_t gSamples{ 4 };
static bool drawNormals{ true };
static bool useBlinn{ true };

static nxt::STexture gDepthTexture;
static nxt::STexture gHDR;

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
struct ObjectBufferInfo
{
	glm::mat4 normalMatrix;
	glm::mat4 worldMatrix;
};
struct PrimitiveBufferInfo
{
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

#define POINT_LIGHT 0
#define DIRECTIONAL_LIGHT 1
#define SPOT_LIGHT 2
#define AMBIENT_LIGHT 3

namespace nxt
{

	RenderSystem::RenderSystem() :
		//mShader{ "assets/shaders/objects/obj5.vert", "assets/shaders/objects/obj5.frag" },
		mShader{ "assets/shaders/objects/compat.vert", "assets/shaders/objects/compat.frag" },
		mFrameInfoBuffer{ buffers::DataBuffer::Create(76, nullptr, nxtBufferTarget_UniformBuffer) },
		mLightInfoBuffer{ buffers::DataBuffer::Create(sizeof(SceneLightData), nullptr, nxtBufferTarget_UniformBuffer)},
		mObjectInfoBuffer{ buffers::DataBuffer::Create(sizeof(ObjectBufferInfo), nullptr, nxtBufferTarget_UniformBuffer) },
		mMaterialInfoBuffer{ buffers::DataBuffer::Create(sizeof(PrimitiveBufferInfo), nullptr, nxtBufferTarget_UniformBuffer) },
		mShadowmap{ 1024 },
		//mShadowShader{ "assets/shaders/shadows/shadowCube.vert", "assets/shaders/shadows/shadowCube.geom", "assets/shaders/shadows/shadowCube.frag" },
		mCamera{ {-2.f, 1.f, 2.f} },

		mScreenQuad{}
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
	}

	static void DrawMesh(const SModel& model, const Mesh& mesh, buffers::SDataBuffer& objectInfo)
	{
		using namespace buffers;
		for (const Primitive& p : mesh.primitives)
		{
			SDataBuffer buffer{ p.buffer };

			bool hasMats{ (p.material >= 0) };
			objectInfo->SetSubData(sizeof(bool), 40, (void*)&p.hasTangents);
			objectInfo->SetSubData(sizeof(bool), 44, &hasMats);
			if (hasMats)
			{
				const SMaterial& mat{ model->GetMaterials()[p.material] };
				objectInfo->SetSubData(sizeof(glm::vec4), 0, glm::value_ptr(mat->Properties.Color.BaseColor));
				objectInfo->SetSubData(sizeof(int32_t), 16, &mat->Properties.Color.Texture);

				objectInfo->SetSubData(sizeof(int32_t), 28, &mat->Textures.Normal);
				objectInfo->SetSubData(sizeof(int32_t), 32, &mat->Textures.Emissive);
				objectInfo->SetSubData(sizeof(int32_t), 36, &mat->Textures.Occlusion);
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
			DrawMesh(model, otherMesh, objectInfo);
		}
	}

	static void DrawModel(const SModel& model, buffers::SDataBuffer& objectInfo)
	{
		model->Bind();
		int32_t i{ 0 };
		for (const STexture& tex : model->GetTextures())
		{
			tex->Bind(i++);
		}
		for (const Mesh& m : model->GetMeshes())
		{
			DrawMesh(model, m, objectInfo);
		}
	}

	void RenderSystem::OnUpdate(float& dt, World& world, bool isFocused)
	{
		if(isFocused)
			mCamera.OnUpdate(dt);

		mFrameInfoBuffer->SetSubData(64, 0, glm::value_ptr(mCamera.GetProjectionViewMatrix()));
		mFrameInfoBuffer->SetSubData(12, 64, (void*)glm::value_ptr(mCamera.GetPosition()));
		
		mLightInfoBuffer->SetSubData(sizeof(glm::vec3), 0, (void*)glm::value_ptr(mCamera.GetPosition()));
		mLightInfoBuffer->SetSubData(sizeof(glm::vec3), 16, (void*)glm::value_ptr(mCamera.GetLookVector()));

		glm::mat4 ones{ 1.f };

		// Push light data to buffer
		necs::SceneView<cmp::Light> lightView{ world.GetScene() };
		int32_t i{ 0 };
		cmp::Light& lc{ world.GetComponent<cmp::Light>(*lightView.begin()) };
		float circleMagnitude{ 10.f };
		lc.Position = glm::vec3{ -std::sin(clock::GetRunTime()) * circleMagnitude, 0.f, std::cos(clock::GetRunTime()) * circleMagnitude };
		for (const necs::Entity& e : lightView)
		{
			mLightInfoBuffer->SetSubData(sizeof(cmp::Light), sizeof(cmp::Light) * i++, &world.GetComponent<cmp::Light>(e));
		}
		mLightInfoBuffer->SetSubData(4, 480, &i);

		// Visible render pass
		mFrameBuffer->Bind();

		render::command::Clear();
		mShader.Bind();
		mShadowmap.BindTextureMap(0);

		necs::SceneView<cmp::Transform, cmp::WorldModel> view{ world.GetScene() };
		for (const necs::Entity& e : view)
		{
			cmp::Transform& t{ world.GetComponent<cmp::Transform>(e) };
			glm::mat4 worldMatrix{ glm::translate(ones, t.Position)
				//* glm::rotate(ones, t.Scale) // quaternions required
				* glm::scale(ones, t.Scale)
			};
			glm::mat4 normalMatrix{ glm::transpose(glm::inverse(worldMatrix)) };

			mObjectInfoBuffer->SetSubData(64, 0, glm::value_ptr(normalMatrix));
			mObjectInfoBuffer->SetSubData(64, 64, glm::value_ptr(worldMatrix));

			cmp::WorldModel& m{ world.GetComponent<cmp::WorldModel>(e) };
			DrawModel(m.Instance->Model, mMaterialInfoBuffer);
		}

		//mFrameBuffer->PushToViewport();
		mFrameBuffer->BindTexture(0);
		mFrameBuffer->Unbind();
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
		SFrameTexture ca{ FrameTexture::Create(ev.Width, ev.Height, 1, nxtTextureFormat_RGBF, nxtTextureTarget_2D) };
		SFrameTexture da{ FrameTexture::Create(ev.Width, ev.Height, 1, nxtTextureFormat_DepthStencil, nxtTextureTarget_2D) };
		//mFrameBuffer = buffers::FrameBuffer::Create(ev.Width, ev.Height, gSamples);
		mFrameBuffer = buffers::FrameBuffer::Create(ca, da);
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

		if (rebuildBuffer)
		{
			mFrameBuffer = buffers::FrameBuffer::Create(mWidth, mHeight, gSamples);
			NXT_LOG_TRACE("MSAA {0}", gSamples);
		}

		if (ev.Keycode == nxtKeycode_N)
		{
			drawNormals = !drawNormals;
		}

		if (ev.Keycode == nxtKeycode_B)
		{
			useBlinn = !useBlinn;
			NXT_LOG_TRACE("Normals: {0}", useBlinn);
			mShader.Bind();
			mShader.SetValue("useNormals", useBlinn);
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

}
