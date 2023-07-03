
#include "RenderSystem.h"

#include <gtc/matrix_transform.hpp>

static float gVertices[]{
	 0.5f,  0.5f, 0.0f,   1.0f, 1.0f,
	 0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
	-0.5f,  0.5f, 0.0f,   0.0f, 1.0f
};

static uint32_t gIndices[6]{
	0, 1, 3,
	1, 2, 3
};

namespace nxt
{

	RenderSystem::RenderSystem() :
		mTexture{ Texture::Create("assets/textures/swirl.png") },
		mShader{ "assets/shaders/gltfShader.vert", "assets/shaders/gltfShader.frag" },
		mCamera{  },
		mModel2{ "assets/models/BoxTexturedOut.gltf" }
	{

	}

	void RenderSystem::OnUpdate(float& dt)
	{
		render::command::Clear();
		mCamera.OnUpdate(dt);
		mShader.Bind();
		//mTexture->Bind();
		//mShader.SetValue("simpleTexture", 0);

		// translate vertices to world space
		glm::mat4 ones{ 1.f };

		glm::mat4 model2{
			glm::translate(ones, glm::vec3{0.f})
		};
		mShader.SetValue("worldMatrix", mCamera.GetProjectionViewMatrix() * model2);
		mModel2.Draw();

	}

	bool RenderSystem::OnEvent(events::Event& ev)
	{
		mCamera.OnEvent(ev);
		return false;
	}

}
