
# Nxt Engine

In development rendering platform / game engine for Windows.

Sample models taken from the [KhronosGroup GLTF sample repo.](https://github.com/KhronosGroup/glTF-Sample-Models/tree/master/2.0)

Celestial models taken from the [NASA Solar System Exploration website.](https://solarsystem.nasa.gov/resources/all/?order=pub_date+desc&per_page=50&page=0&search=&condition_1=1%3Ais_in_resource_list&fs=&fc=324&ft=&dp=&category=324)

## Download + Build

### Requirements 

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

## How To Use

NxtEngine comes with a built in editor, but if you want to use the raw render api or your own app, you can.
An interface is provided to hook into the main loop of the engine.

Add ```NextEngine/src``` as an include directory.

```

#include <NextEngine.h>

class ExampleInterface : public nxt::AppInterface
{
public:
    virtual OnUpdate(double dt) override;
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
