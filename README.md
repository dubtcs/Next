
# Nxt Engine

In development rendering platform / game engine for Windows.

## Download + Build

### Requirements 

 - C++20
 - (Suggested) Visual Studio 2022

### Downloading 

1. Download the repository with `git clone --recursive https://github.com/dubtcs/Next.git`

2. Run `_GenerateSolutions.bat` to create VS2022 solution and project files.
    - You can use this any time you make changes to the premake5.lua file or just want new solutions.

## Goals

NxtEngine is built from the ground up with Win32 and OpenGL to get a better understanding of the computer graphics pipeline.

### Current Features 
   - [Custom ECS](https://github.com/dubtcs/nECS)

## How To Use

NxtEngine comes with a built in editor, but if you want to use the raw render api or your own app, you can.
An interface is provided to hook into the main loop of the engine.

Be sure to add NxtEngine/src as an include directory.

```

#include <NextEngine.h>

// Include the app interface
#include <nxt/core/app/Interface.h>

class Example : public nxt::AppInterface
{
public:
    virtual OnUpdate(double dt) override;
    virtual OnEvent(nxt::events::Event& ev) override;
};

int main()
{
    nxt::App* app{ new nxt::App{} };

    // AddInterface takes an std::shared_pointer to an AppInterface
    app->AddInterface(nxt::NewShared<Example>());
    
    app->Run();
    delete app;
}

```
