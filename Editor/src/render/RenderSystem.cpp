
#include "RenderSystem.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static nxt::SModel cubeModel;

static uint32_t gSamples{ 4 };
static bool drawNormals{ true };
static bool useBlinn{ true };

namespace nxt
{

	RenderSystem::RenderSystem() :
		mShader{ "assets/shaders/gltfUBlock.vert", "assets/shaders/gltfShader.frag" },
		mSkyboxShader{ "assets/shaders/skybox.vert", "assets/shaders/skybox.frag" },
		mNormalShader{"assets/shaders/normals.vert", "assets/shaders/normals.geom", "assets/shaders/normals.frag" },
		mSkyboxCubemap{ Cubemap::Create({
			"assets/textures/skybox/right.jpg",
			"assets/textures/skybox/left.jpg",
			"assets/textures/skybox/top.jpg",
			"assets/textures/skybox/bottom.jpg",
			"assets/textures/skybox/front.jpg",
			"assets/textures/skybox/back.jpg"
			})
		},
		mCamera{ {-2.f, 1.f, 2.f} },
		mMatrixBuffer{ buffers::DataBuffer::Create(64, nullptr, nxtBufferTarget_UniformBuffer) }
	{
		cubeModel = nxt::Model::Create( "assets/models/BoxTextured.gltf" );
		mMatrixBuffer->BindIndexed(0);
		mShader.Bind();
		mShader.SetValue("useBlinn", useBlinn);
		//render::command::SetRenderFeature(nxtRenderFeature_PointSize, true);
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
		mFrameBuffer->Bind();
		render::command::Clear();
		mCamera.OnUpdate(dt);
		mMatrixBuffer->SetSubData(64, 0, glm::value_ptr(mCamera.GetProjectionViewMatrix()));

		mSkyboxCubemap->Bind();

		render::command::SetFaceCullingMode(nxtCullingMode_Back);

		mShader.Bind();
		mShader.SetValue("cameraPosition", mCamera.GetPosition());

		float ti{ clock::GetRunTime() };
		glm::vec3 lightPosition{ 0.f, (std::sin(ti)) * 25.f, 0.f };
		mShader.SetValue("lightPosition", lightPosition);

		// translate vertices to world space
		glm::mat4 ones{ 1.f };

		necs::SceneView<cmp::WorldModel, cmp::Transform> view{ world.GetScene() };
		for (const necs::Entity& e : view)
		{
			cmp::Transform& t{ world.GetComponent<cmp::Transform>(e) };

			glm::mat4 modelMatrix{
				glm::translate(ones, t.Position)
				//* glm::scale(ones, glm::vec3{ 5.f, 0.1f, 5.f })
			};
			mShader.Bind();
			mShader.SetValue("worldMatrix", modelMatrix);
			mShader.SetValue("normalMatrix", glm::transpose(glm::inverse(modelMatrix)));

			cmp::WorldModel& wm{ world.GetComponent<cmp::WorldModel>(e) };
			wm.ModelInstance->GetTextures().front()->Bind(1);
			mShader.SetValue("simpleTexture", 1);
			DrawModel(wm.ModelInstance);

			if (drawNormals)
			{
				mNormalShader.Bind();
				mNormalShader.SetValue("viewMatrix", mCamera.GetViewMatrix());
				mNormalShader.SetValue("projectionMatrix", mCamera.GetProjectionMatrix());
				mNormalShader.SetValue("worldMatrix", modelMatrix);
				mNormalShader.SetValue("normalMatrix", glm::transpose(glm::inverse(mCamera.GetViewMatrix() * modelMatrix)));
				DrawModel(wm.ModelInstance);
			}

		}

		// Skybox
		render::command::SetFaceCullingMode(nxtCullingMode_Front);

		glm::mat4 skyboxMatrix{ mCamera.GetProjectionMatrix() * glm::mat4{glm::mat3{mCamera.GetViewMatrix()}} };
		mSkyboxShader.Bind();
		mSkyboxShader.SetValue("projectionView", skyboxMatrix);
		DrawModel(cubeModel);

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
			NXT_LOG_TRACE(gSamples);
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
