#pragma once

#include <iostream>

#ifdef NXT_PLATFORM_WINDOWS

#include <nxt/engine/core/app/App.h>

int main(int argc, char** argv)
{

	nxt::App* app{ new nxt::App{} };

	app->Run();

	delete app;

}

#else

#error "Only windows supported"

#endif