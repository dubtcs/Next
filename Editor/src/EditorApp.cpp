
#include "EditorApp.h"

#include <nxt/render/RenderAPI.h>

static nxt::Shared<nxt::render::VertexBuffer>gVB;
static nxt::Shader gShader;

static float gVertices[9]{
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

namespace nxt
{

	Editor::Editor()
	{

		gVB = render::VertexBuffer::Create(sizeof(gVertices), render::BUFFERUSAGE_STATIC, gVertices);

		gShader = Shader{ "assets/shaders/shader.vert", "assets/shaders/shader.frag" };
		gVB->SetLayoutPosition(0, 3, render::DATATYPE_FLOAT);
	}

	void Editor::OnUpdate(double& dt)
	{
		render::api::Clear();
		gShader.Bind();
		gVB->Draw(render::DRAWMODE_TRIANGLES);
		NXT_LOG_TRACE("Framerate: {0}", static_cast<int32_t>((1.0 / dt)));
	}

	void Editor::OnEvent(nxt::events::Event& ev)
	{
		NXT_LOG_INFO("Event Received: {0}", ev.GetName());
	}

}

