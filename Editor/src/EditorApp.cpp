
#include "EditorApp.h"

#include <necs/include.h>

#include <nxt/core/events/ScriptEvents.h>
#include <nxt/render/texture/Cubemap.h>

#include <nxt/core/events/ScriptEvents.h>
#include <glm/glm.hpp>

#include <nxt/core/utility/lerp.h>

static nxt::SModelInstance modelInstance{ nullptr };
static necs::Entity viewModel{};

namespace nxt
{

	Editor::Editor() :
		mWorld{}

	{

		// Lights
		necs::Entity l1{ mWorld.CreateEntity() };
		cmp::Light li1{ .Position{ 0.f, 3.f, 5.f }, .Intensity{3.f}, .LightType{cmp::nxtLightType_Point}, .Color{0.8, 0.7, 0.4f} };
		mWorld.Attach<cmp::Light>(l1, li1);

		necs::Entity l2{ mWorld.CreateEntity() };
		cmp::Light li2{ .Intensity{ 0.3f }, .LightType{ cmp::nxtLightType_Ambient }, .Color{ 1.f, 1.f, 1.f } };
		mWorld.Attach<cmp::Light>(l2, li2);

		necs::Entity l3{ mWorld.CreateEntity() };
		cmp::Light li3{ .Intensity{1.f}, .LightType{cmp::nxtLightType_Spot}, .Radius{std::cos(glm::radians(25.f))}};
		mWorld.Attach<cmp::Light>(l3, li3);
		
		necs::Entity l4{ mWorld.CreateEntity() };
		cmp::Light li4{ .Intensity{1.f}, .Direction{5.f, 2.f, 0.f}, .LightType{cmp::nxtLightType_Directional} };
		mWorld.Attach<cmp::Light>(l4, li4);

		// World Models
		modelInstance = NewShared<ModelInstance>(Model::Create("assets/models/Avocado.gltf"));
		viewModel = mWorld.CreateEntity();
		mWorld.Attach<cmp::Transform>(viewModel, { glm::vec3{0.f}, glm::vec3{0.f}, glm::vec3{1.f} });
		mWorld.Attach<cmp::WorldModel>(viewModel, modelInstance);
	}

	void Editor::OnUpdate(float& dt, bool isFocused)
	{
		World copy{ mWorld };
		mRender.OnUpdate(dt, copy, isFocused);
	}

	bool Editor::OnDragReceived(events::DragFileReceived& ev)
	{
		if (ev.Path.extension().string() == ".gltf" || ev.Path.extension().string() == ".glb")
		{
			modelInstance->Model = Model::Create(ev.Path);
		}
		return false;
	}

	bool Editor::OnEvent(nxt::events::Event& ev)
	{
		events::Handler handler{ ev };
		handler.Fire<events::DragFileReceived>(NXT_CALLBACK(Editor::OnDragReceived));
		mRender.OnEvent(ev);
		return false;
	}

}

