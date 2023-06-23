
#include <NextEngine.h>

#include "EditorApp.h"

int main()
{

	//nxt::App* app{ new nxt::App{"Nxt Editor"} };
	nxt::app::Launch("Nxt Viewport", 1920, 1080);
	nxt::app::AddInterface(nxt::NewShared<nxt::Editor>());
	nxt::app::Run();

}
