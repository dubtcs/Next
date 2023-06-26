
#include <NextEngine.h>

#include "EditorApp.h"

int main()
{

	nxt::app::Launch("Nxt Viewport", 1920, 1080);
	nxt::app::AddInterface(nxt::NewShared<nxt::Editor>());
	nxt::app::Run();

}
