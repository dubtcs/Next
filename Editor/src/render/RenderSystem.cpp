
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
		mVertexBuffer{ buffers::VertexBuffer::Create(sizeof(gVertices), buffers::BUFFER_USAGE_STATIC, gVertices) },
		mElementBuffer{ buffers::ElementBuffer::Create(sizeof(gIndices), gIndices, buffers::BUFFER_USAGE_STATIC) },
		mTexture{ texture::Texture::Create("assets/textures/swirl.png") },
		mShader{ "assets/shaders/shader3.vert", "assets/shaders/shader3.frag" },
		mCamera{  }
	{
		uint32_t stride{ 5 * sizeof(float) };
		mVertexBuffer->SetLayoutPosition(0, 3, DATA_TYPE_FLOAT, stride, 0, false);
		mVertexBuffer->SetLayoutPosition(1, 2, DATA_TYPE_FLOAT, stride, 3 * sizeof(float), false);
		mElementBuffer->AddVertexBuffer(mVertexBuffer);
	}

	void RenderSystem::OnUpdate(float& dt)
	{
		render::command::Clear();
		mCamera.OnUpdate(dt);
		mShader.Bind();
		mTexture->Bind();
		mShader.SetValue("simpleTexture", 0);

		// translate vertices to world space
		glm::mat4 ones{ 1.f };

		glm::mat4 model{ 
			glm::translate(ones, glm::vec3{0.f, 0.5f, 0.f})
			//glm::rotate(ones, glm::radians(45.f), glm::vec3{1.f,0.f,0.f})
		};

		mShader.SetValue("worldMatrix", mCamera.GetProjectionViewMatrix() * model);

		mElementBuffer->Draw(buffers::DRAW_MODE_TRIANGLES, 6);
	}

	bool RenderSystem::OnEvent(events::Event& ev)
	{
		mCamera.OnEvent(ev);
		return false;
	}

}
