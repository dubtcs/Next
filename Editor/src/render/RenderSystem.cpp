
#include "RenderSystem.h"

#include <gtc/matrix_transform.hpp>

namespace nxt
{

	RenderSystem::RenderSystem() :
		mTexture{ Texture::Create("assets/textures/swirl.png") },
		mShader{ "assets/shaders/gltfShader.vert", "assets/shaders/gltfShader.frag" },
		mCamera{ {-2.f, 1.f, 2.f} },
		mModel{ "assets/models/BoxTextured.gltf" }
	{

	}

	static void DrawMesh(const Mesh& mesh)
	{
		using namespace buffers;
		for (const Primitive& p : mesh.primitives)
		{
			SDataBuffer buffer{ p.buffer };
			buffer->Bind();
			render::command::DrawElements(nxtDrawMode_Triangles, p.count, static_cast<nxtDataType>(p.componentType), (void*)(p.byteOffset));
		}
		for (const Mesh& otherMesh : mesh.children)
		{
			DrawMesh(otherMesh);
		}
	}

	void RenderSystem::OnUpdate(float& dt)
	{
		render::command::Clear();
		mCamera.OnUpdate(dt);
		mShader.Bind();
		
		// translate vertices to world space
		glm::mat4 ones{ 1.f };

		glm::mat4 model2{
			glm::translate(ones, glm::vec3{0.f})
		};
		mShader.SetValue("worldMatrix", model2);
		mShader.SetValue("projectionViewMatrix", mCamera.GetProjectionViewMatrix());
		mShader.SetValue("normalMatrix", glm::transpose(glm::inverse(model2)));
		mShader.SetValue("cameraPosition", mCamera.GetPosition());

		glm::vec3 lightPosition{  1.f + std::sin(clock::GetRunTime()), std::sin(clock::GetRunTime()), 1.f};
		mShader.SetValue("lightPosition", lightPosition);

		mModel.Bind();
		mModel.GetTextures().front()->Bind();
		for (const Mesh& m : mModel.GetMeshes())
		{
			DrawMesh(m);
		}

	}

	bool RenderSystem::OnEvent(events::Event& ev)
	{
		mCamera.OnEvent(ev);
		return false;
	}

}
