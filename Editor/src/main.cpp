
#include <NextEngine.h>

#include "EditorApp.h"

int main()
{

	nxt::App* app{ new nxt::App{} };
	app->AddInterface(nxt::NewShared<nxt::Editor>());
	app->Run();
	delete app;

}
