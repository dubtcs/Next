
#include "RenderSystem.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static nxt::SModel cubeModel;

static uint32_t gSamples{ 4 };
static bool drawNormals{ true };
static bool useBlinn{ true };

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

#define POINT_LIGHT 0
#define DIRECTIONAL_LIGHT 1
#define SPOT_LIGHT 2
#define AMBIENT_LIGHT 3

namespace nxt
{

	RenderSystem::RenderSystem() :
		mShader{ "assets/shaders/objects/obj4.1.vert", "assets/shaders/objects/obj4.1.frag" },
		mCameraMatrixBuffer{ buffers::DataBuffer::Create(76, nullptr, nxtBufferTarget_UniformBuffer) },
		mLightInfoBuffer{ buffers::DataBuffer::Create(sizeof(SceneLightData), nullptr, nxtBufferTarget_UniformBuffer)},
		mShadowmap{ 1024 },
		mShadowShader{ "assets/shaders/shadows/shadowCube.vert", "assets/shaders/shadows/shadowCube.geom", "assets/shaders/shadows/shadowCube.frag" },
		mCamera{ {-2.f, 1.f, 2.f} }
	{
		cubeModel = nxt::Model::Create( "assets/models/BoxTextured.gltf" );
		mShader.Bind();
		mShader.SetValue("useBlinn", useBlinn);

		mCameraMatrixBuffer->BindIndexed(0);
		mLightInfoBuffer->BindIndexed(1);

		uint32_t i{ 0 };

		LightInfo light{};
		light.Type = SPOT_LIGHT;
		light.Intensity = 0.f;
		light.Direction = glm::vec3{ 1.f, 0.f, 0.f };
		light.Position = glm::vec3{ -1.f, 0.f, 0.f };
		light.Color = glm::vec3{ 0.25f, 1.f, 0.8f };
		light.Radius = std::cos(glm::radians(25.f));
		mLightInfoBuffer->SetSubData(sizeof(LightInfo), sizeof(LightInfo) * i, &light);
		i++;

		LightInfo light2{};
		light2.Type = POINT_LIGHT;
		light2.Intensity = 1.f;
		light2.Position = glm::vec3{ -2.f, 4.f, -1.f };
		light2.Color = glm::vec3{ 0.8f, 1.f, 0.8f };
		light2.Direction = glm::vec3{ 1.f, -1.f, 0.f };
		mLightInfoBuffer->SetSubData(sizeof(LightInfo), sizeof(LightInfo) * i, &light2);
		i++;

		// Premade lighting matrices
		glm::mat4 shadowProjection{ glm::perspective(glm::radians(90.f), 1.f, 0.01f, 25.f) };
		//glm::mat4 shadowView{ glm::lookAt(light2.Position, glm::vec3{0.f}, glm::vec3{ 0.f, 1.f, 0.f }) };
		//glm::mat4 lightSpaceMatrix{ shadowProjection * shadowView };
		//mShader.SetValue("shadowMatrix", lightSpaceMatrix);
		//mShader.SetValue("shadowMap", 0);
		//mShadowShader.Bind();
		//mShadowShader.SetValue("projectionViewMatrix", lightSpaceMatrix);

		std::vector<glm::mat4> shadowViewMatrices{
			shadowProjection * glm::lookAt(light2.Position, light2.Position + glm::vec3{ 1.f,  0.f,  0.f}, glm::vec3{ 0.f,  -1.f,  0.f}),
			shadowProjection * glm::lookAt(light2.Position, light2.Position + glm::vec3{-1.f,  0.f,  0.f}, glm::vec3{ 0.f,  -1.f,  0.f}),
			shadowProjection * glm::lookAt(light2.Position, light2.Position + glm::vec3{ 0.f,  1.f,  0.f}, glm::vec3{ 0.f,  0.f,  1.f}),
			shadowProjection * glm::lookAt(light2.Position, light2.Position + glm::vec3{ 0.f, -1.f,  0.f}, glm::vec3{ 0.f,  0.f, -1.f}),
			shadowProjection * glm::lookAt(light2.Position, light2.Position + glm::vec3{ 0.f,  0.f,  1.f}, glm::vec3{ 0.f,  -1.f,  0.f}),
			shadowProjection * glm::lookAt(light2.Position, light2.Position + glm::vec3{ 0.f,  0.f, -1.f}, glm::vec3{ 0.f, - 1.f,  0.f}),
		};

		mShader.SetValue("shadowMap", 0);
		mShadowShader.Bind();
		mShadowShader.SetArrayValue("projectionViewMatrices", shadowViewMatrices);

		LightInfo light3{};
		light3.Type = AMBIENT_LIGHT;
		light3.Intensity = 0.05f;
		light3.Color = glm::vec3(1.f, 0.f, 0.f);
		mLightInfoBuffer->SetSubData(sizeof(LightInfo), sizeof(LightInfo) * i, &light3);
		i++;

		mLightInfoBuffer->SetSubData(4, 480, &i);

	}

	static void DrawMesh(const Mesh& mesh)
	{
		using namespace buffers;
		for (const Primitive& p : mesh.primitives)
		{
			SDataBuffer buffer{ p.buffer };
			buffer->Bind();
			render::command::DrawElements(p.mode, p.count, p.componentType, (void*)(p.byteOffset));
		}
		for (const Mesh& otherMesh : mesh.children)
		{
			DrawMesh(otherMesh);
		}
	}

	static void DrawModel(const SModel& model)
	{
		model->Bind();
		for (const Mesh& m : model->GetMeshes())
		{
			DrawMesh(m);
		}
	}

	void RenderSystem::OnUpdate(float& dt, World& world)
	{
		
		mCamera.OnUpdate(dt);
		mCameraMatrixBuffer->SetSubData(64, 0, glm::value_ptr(mCamera.GetProjectionViewMatrix()));
		mCameraMatrixBuffer->SetSubData(12, 64, (void*)glm::value_ptr(mCamera.GetPosition()));
		
		mLightInfoBuffer->SetSubData(sizeof(glm::vec3), 0, (void*)glm::value_ptr(mCamera.GetPosition()));
		mLightInfoBuffer->SetSubData(sizeof(glm::vec3), 16, (void*)glm::value_ptr(mCamera.GetLookVector()));

		glm::mat4 ones{ 1.f };

		// Lighting render pass
		mShadowShader.Bind();
		mShadowmap.BeginRenderPass();

		necs::SceneView<cmp::Transform, cmp::WorldModel> view{ world.GetScene() };
		for (const necs::Entity& e : view)
		{
			cmp::Transform& t{ world.GetComponent<cmp::Transform>(e) };
			glm::mat4 worldMatrix{ glm::translate(ones, t.Position)
				//* glm::rotate(ones, t.Scale) // quaternions required
				* glm::scale(ones, t.Scale)
			};
			mShadowShader.SetValue("worldMatrix", worldMatrix);

			cmp::WorldModel& m{ world.GetComponent<cmp::WorldModel>(e) };
			DrawModel(m.ModelInstance);
		}

		mShadowmap.EndRenderPass(mWidth, mHeight);

		// Visible render pass
		mFrameBuffer->Bind();

		render::command::Clear();
		mShader.Bind();
		mShadowmap.BindTextureMap(0);
		for (const necs::Entity& e : view)
		{
			cmp::Transform& t{ world.GetComponent<cmp::Transform>(e) };
			glm::mat4 worldMatrix{ glm::translate(ones, t.Position)
				//* glm::rotate(ones, t.Scale) // quaternions required
				* glm::scale(ones, t.Scale)
			};
			mShader.SetValue("worldMatrix", worldMatrix);
			mShader.SetValue("normalMatrix", glm::transpose(glm::inverse(worldMatrix)));

			cmp::WorldModel& m{ world.GetComponent<cmp::WorldModel>(e) };
			DrawModel(m.ModelInstance);
		}

		mFrameBuffer->PushToViewport();

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
		mFrameBuffer = buffers::FrameBuffer::Create(ev.Width, ev.Height, gSamples);
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
			NXT_LOG_TRACE("BlinnPhong model: {0}", useBlinn);
			mShader.Bind();
			mShader.SetValue("useBlinn", useBlinn);
		}

		if (ev.Keycode == nxtKeycode_G)
		{
			static bool jj{ true };
			jj = !jj;
			NXT_LOG_TRACE("Gamma correction: {0}", jj);
			render::command::SetRenderFeature(nxtRenderFeature_FrameBuffer_sRGB, jj);
		}

		// The enums are 2 off from eachother
		//if (ev.Keycode == nxtKeycode_Left || ev.Keycode == nxtKeycode_Right)
		//{
		//	uint32_t newVal{ ev.Keycode - 35 };
		//	if (newVal != gSamples)
		//	{
		//		// to stop making a framebuffer every time a key is pressed
		//		gSamples = ev.Keycode - 35;
		//		NXT_LOG_TRACE(gSamples);
		//		mFrameBuffer = buffers::FrameBuffer::Create(mWidth, mHeight, gSamples);
		//	}
		//}
		return false;
	}

}
