
#include "../DeviceC.h"

namespace nxt
{

	DeviceC::DeviceC(bool createWindow)
	{
		
	}

	bool DeviceC::IsKeyDown(nxtKeycode keycode) const
	{
		return (::GetKeyState(keycode) & 0x8000);
	}

}
