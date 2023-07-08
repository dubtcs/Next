
#include "RenderSystem.h"

#include <glm/gtc/matrix_transform.hpp>

static nxt::SModel cubeModel;

namespace nxt
{

	RenderSystem::RenderSystem() :
		mShader{ "assets/shaders/gltfShader.vert", "assets/shaders/gltfShader.frag" },
		mSkyboxShader{ "assets/shaders/skybox.vert", "assets/shaders/skybox.frag" },
		mSkyboxCubemap{ Cubemap::Create({
			"assets/textures/skybox/right.jpg",
			"assets/textures/skybox/left.jpg",
			"assets/textures/skybox/top.jpg",
			"assets/textures/skybox/bottom.jpg",
			"assets/textures/skybox/front.jpg",
			"assets/textures/skybox/back.jpg"
			})
		},
		mCamera{ {-2.f, 1.f, 2.f} }
	{
		cubeModel = nxt::Model::Create( "assets/models/BoxTextured.gltf" );
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
		render::command::Clear();
		mCamera.OnUpdate(dt);
		
		// translate vertices to world space
		glm::mat4 ones{ 1.f };

		//render::command::SetRenderFeature(nxtRenderFeature_DepthTest, true);
		render::command::SetFaceCullingMode(nxtCullingMode_Back);

		mShader.Bind();
		mShader.SetValue("projectionViewMatrix", mCamera.GetProjectionViewMatrix());
		mShader.SetValue("cameraPosition", mCamera.GetPosition());

		float ti{ clock::GetRunTime() };
		glm::vec3 lightPosition{ 0.f, std::sin(ti) * 500.f, 300.f };
		mShader.SetValue("lightPosition", lightPosition);

		necs::SceneView<cmp::WorldModel, cmp::Transform> view{ world.GetScene() };
		for (const necs::Entity& e : view)
		{
			cmp::Transform& t{ world.GetComponent<cmp::Transform>(e) };

			glm::mat4 modelMatrix{
				glm::translate(ones, t.Position)
			};
			mShader.SetValue("worldMatrix", modelMatrix);
			mShader.SetValue("normalMatrix", glm::transpose(glm::inverse(modelMatrix)));

			cmp::WorldModel& wm{ world.GetComponent<cmp::WorldModel>(e) };
			wm.ModelInstance->GetTextures().front()->Bind();
			DrawModel(wm.ModelInstance);
		}

		// Skybox
		//render::command::SetRenderFeature(nxtRenderFeature_DepthTest, false);
		render::command::SetFaceCullingMode(nxtCullingMode_Front);

		glm::mat4 skyboxMatrix{ mCamera.GetProjectionMatrix() * glm::mat4{glm::mat3{mCamera.GetViewMatrix()}} };
		mSkyboxShader.Bind();
		mSkyboxShader.SetValue("projectionView", skyboxMatrix);
		mSkyboxCubemap->Bind();
		DrawModel(cubeModel);

	}

	bool RenderSystem::OnEvent(events::Event& ev)
	{
		mCamera.OnEvent(ev);
		return false;
	}

}
