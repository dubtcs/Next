
#include "EditorApp.h"

#include <necs/include.h>

#include <nxt/core/events/ScriptEvents.h>
#include <nxt/render/texture/Cubemap.h>

#include <nxt/core/events/ScriptEvents.h>

static nxt::SModel modelInstance{ nullptr };
static nxt::cmp::WorldModel worldModel{};
static necs::Entity viewModel{};

namespace nxt
{

	Editor::Editor() :
		mWorld{}
	{

		// Lights
		necs::Entity l1{ mWorld.CreateEntity() };
		cmp::Light li1{ .Position{ 0.f, 5.f, 5.f }, .Intensity{3.f}, .LightType{cmp::nxtLightType_Point}, .Color{0.5, 0.7, 1.f} };
		mWorld.Attach<cmp::Light>(l1, li1);

		necs::Entity l2{ mWorld.CreateEntity() };
		cmp::Light li2{ .Intensity{ 0.1f }, .LightType{ cmp::nxtLightType_Ambient }, .Color{ 1.f, 1.f, 1.f } };
		mWorld.Attach<cmp::Light>(l2, li2);

		// World Models
		modelInstance = Model::Create("assets/models/stoneCube/Cube.gltf");
		//SModel avo{ Model::Create("assets/models/Avocado.gltf") };
		
		worldModel.ModelInstance = modelInstance;

		viewModel =  mWorld.CreateEntity();
		mWorld.Attach<cmp::Transform>(viewModel, { glm::vec3{0.f}, glm::vec3{0.f}, glm::vec3{1.f} });
		mWorld.Attach<cmp::WorldModel>(viewModel, worldModel);

	}

	void Editor::OnUpdate(float& dt, bool isFocused)
	{
		mRender.OnUpdate(dt, mWorld, isFocused);
	}

	bool Editor::OnDragReceived(events::DragFileReceived& ev)
	{
		if (ev.Path.extension().string() == ".gltf" || ev.Path.extension().string() == ".glb")
		{
			cmp::WorldModel& wm{ mWorld.GetComponent<cmp::WorldModel>(viewModel) };
			modelInstance = Model::Create(ev.Path);
			wm.ModelInstance = modelInstance;
		}
		return false;
	}

	bool Editor::OnEvent(nxt::events::Event& ev)
	{
		events::Handler handler{ ev };
		//handler.Fire<events::KeyboardPressed>(NXT_CALLBACK(Editor::OnKeyPressed));
		//handler.Fire<events::MouseButtonPressed>(NXT_CALLBACK(Editor::OnMouseButtonPressed));
		//handler.Fire<events::MouseButtonReleased>(NXT_CALLBACK(Editor::OnMouseButtonReleased));
		//handler.Fire<events::WindowResized>(NXT_CALLBACK(Editor::OnWindowResize));
		handler.Fire<events::DragFileReceived>(NXT_CALLBACK(Editor::OnDragReceived));
		mRender.OnEvent(ev);
		return false;
	}

}

