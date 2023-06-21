
#include "EditorApp.h"

#include <nxt/render/RenderAPI.h>

static nxt::Shared<nxt::buffers::VertexBuffer>gVB;
static nxt::Shared<nxt::buffers::ElementBuffer>gEB;

static nxt::Shared<nxt::texture::Texture> gTT;

static nxt::Shader gShader;

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

	Editor::Editor()
	{
		gVB = buffers::VertexBuffer::Create(sizeof(gVertices), buffers::BUFFER_USAGE_STATIC, gVertices);
		gEB = buffers::ElementBuffer::Create(sizeof(gIndices), gIndices, buffers::BUFFER_USAGE_STATIC);

		gShader = Shader{ "assets/shaders/shader.vert", "assets/shaders/shader.frag" };

		uint32_t stride{ 5 * sizeof(float) };
		gVB->SetLayoutPosition(0, 3, nxt::DATA_TYPE_FLOAT, stride);
		gVB->SetLayoutPosition(1, 2, nxt::DATA_TYPE_FLOAT, stride, 3 * sizeof(float));

		gEB->AddVertexBuffer(gVB);

		gTT = texture::Texture::Create("assets/textures/swirl.png");
	}

	void Editor::OnUpdate(double& dt)
	{
		render::command::Clear();
		gShader.Bind();
		gTT->Bind();

		gShader.SetValue("simpleTexture", 0);

		gEB->Draw(buffers::DRAW_MODE_TRIANGLES, 6);
		//NXT_LOG_TRACE("Framerate: {0}", static_cast<int32_t>((1.0 / dt)));
	}

	void Editor::OnEvent(nxt::events::Event& ev)
	{
		NXT_LOG_INFO("Event Received: {0}", ev.GetName());
	}

}

