
# Nxt Engine

In development rendering platform / model viewer for Windows.

Sample models supplied by [KhronosGroup GLTF sample repo.](https://github.com/KhronosGroup/glTF-Sample-Models/tree/master/2.0)

![avocado c:](media/072723.png "Avocado :)")

## Download + Build

### Requirements 

 - Windows OS
 - C++20
 - (Suggested) Visual Studio 2022

### Downloading 

1. Download the repository with `git clone --recursive https://github.com/dubtcs/Next.git`

2. Run `_GenerateSolutions.bat` to create VS2022 solution and project files.
    - You can use this any time you make changes to the premake5.lua file or just want new solutions.

## Goals

NxtEngine is built with Win32 and OpenGL to get a better understanding of the computer graphics pipeline, graphics programming, and game engine development.

### Current Features 
   - [Custom ECS](https://github.com/dubtcs/nECS)
   - Deferred rendering
   - Drag and Drop 3D GLTF Model loading
   - MSAA
   - Normal and Parallax mapping
   - Shadow support
   - Skyboxes

## How To Use

NxtEngine comes with a built in viewer, but if you want to use the raw render api or your own app, you can.

### Viewer Controls

**MSAA Sampling:**
    Use the right/left arrow keys to increase/decrease MSAA sampling.

**Normals:**
    Press *N* to toggle normal mapping.

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
