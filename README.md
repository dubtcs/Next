
# Nxt Engine

In development rendering platform / model viewer for Windows. Thanks for taking the time to check out my project.

Sample models supplied by [KhronosGroup GLTF sample repo.](https://github.com/KhronosGroup/glTF-Sample-Models/tree/master/2.0)

## DEMO

[Here is a quick demo of the project in action.](https://youtu.be/H261Z77qU8I)

## Goals

NxtEngine is built with Win32 and OpenGL to get a better understanding of the computer graphics pipeline, graphics programming, and game engine development.

### Current Features
   - [Custom ECS](https://github.com/dubtcs/nECS)
   - SSAO
   - Deferred rendering
   - Drag and Drop 3D GLTF Model loading
   - MSAA
   - Normal and Parallax mapping
   - Shadow support
   - Skyboxes
   - GLTF Feature Support
      - [x] Animations
      - [x] Heirarchy
      - [x] Materials
      - [x] Meshes
      - [ ] Morph targets
      - [x] Multiple scenes
      - [ ] Skins
      - [x] Sparse Accessors
      - [x] Textures and Images
      - [x] Vertex Colors

## Media

Screenshots can be found in the [media](media) folder.

![lantern c:](media/092823.png "Lantern :)")

## Download + Build

### Requirements 

 - Windows OS
 - C++20
 - (Suggested) Visual Studio 2022

### Downloading 

1. Download the repository with `git clone --recursive https://github.com/dubtcs/Next.git`

2. Run `_GenerateSolutions.bat` to create VS2022 solution and project files.
    - You can use this any time you make changes to the premake5.lua file or just want new solutions.

## How To Use

NxtEngine comes with a built in viewer, but if you want to use the raw render api or your own app, you can.

### Viewer Controls

**Model Scale:**

[+] Scale model up

[-] Scale model down

**SSAO:**

[B] Toggle SSAO

**Animation:**

[0 - 9] Play animations.

[P] Pause animation

[G] Stop animation

### App Interface

An interface is provided to hook into the main loop of the engine.

Add ```NextEngine/src``` as an include directory.

```

#include <NextEngine.h>

class ExampleInterface : public nxt::AppInterface
{
public:
    virtual OnUpdate(float dt, bool isFocused) override;
    virtual OnEvent(nxt::events::Event& ev) override;
};

int main()
{

    nxt::app::Launch("Your App Name", 1920, 1080);

    // AddInterface takes an std::shared_pointer to an AppInterface
    nxt::app::AddInterface(nxt::NewShared<ExampleInterface>());
    
    nxt::app::Run();
}

```
