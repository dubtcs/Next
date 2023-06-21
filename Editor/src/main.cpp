
#include <NextEngine.h>

#include "EditorApp.h"

int main()
{

	nxt::App* app{ new nxt::App{"Nxt Editor"} };
	app->AddInterface(nxt::NewShared<nxt::Editor>());
	app->Run();
	delete app;

}
