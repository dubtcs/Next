#pragma once

#include <nxt/core/GenericEnums.h>

namespace nxt
{

	enum nxtKeycode : nxt_enum
	{

		// Mouse Buttons

		nxtKeycode_Mouse1 = 0x01,
		nxtKeycode_Mouse2 = 0x02,
		nxtKeycode_Mouse3 = 0x04,
		nxtKeycode_Mouse4 = 0x05,
		nxtKeycode_Mouse5 = 0x06,

		nxtKeycode_MouseLeft = nxtKeycode_Mouse1,
		nxtKeycode_MouseMiddle = nxtKeycode_Mouse3,
		nxtKeycode_MouseRight = nxtKeycode_Mouse2,

		nxtKeycode_MouseForward = nxtKeycode_Mouse5,
		nxtKeycode_MouseBackward = nxtKeycode_Mouse4,

		// Keyboard Misc Input

		nxtKeycode_Backspace = 0x08,
		nxtKeycode_Tab = 0x09,
		nxtKeycode_Clear = 0x0C,
		nxtKeycode_Enter = 0x0D,
		nxtKeycode_Shift = 0x10,
		nxtKeycode_Ctrl = 0x11,
		nxtKeycode_Menu = 0x12, // Alt Key
		nxtKeycode_Pause = 0x13,
		nxtKeycode_Capital = 0x14, // Caps Lock
		nxtKeycode_Escape = 0x1B,

		nxtKeycode_Space = 0x20,
		nxtKeycode_Delete = 0x2E,

		nxtKeycode_Left = 0x25,
		nxtKeycode_Up = 0x26,
		nxtKeycode_Right = 0x27,
		nxtKeycode_Down = 0x28,

		nxtKeycode_CapsLock = nxtKeycode_Capital,
		nxtKeycode_Alt = nxtKeycode_Menu,

		// Keyboard Main / Letter Input

		nxtKeycode_0 = 0x30,
		nxtKeycode_1 = 0x31,
		nxtKeycode_2 = 0x32,
		nxtKeycode_3 = 0x33,
		nxtKeycode_4 = 0x34,
		nxtKeycode_5 = 0x35,
		nxtKeycode_6 = 0x36,
		nxtKeycode_7 = 0x37,
		nxtKeycode_8 = 0x38,
		nxtKeycode_9 = 0x39,

		nxtKeycode_A = 0x41,
		nxtKeycode_B = 0x42,
		nxtKeycode_C = 0x43,
		nxtKeycode_D = 0x44,
		nxtKeycode_E = 0x45,
		nxtKeycode_F = 0x46,
		nxtKeycode_G = 0x47,
		nxtKeycode_H = 0x48,
		nxtKeycode_I = 0x49,
		nxtKeycode_J = 0x4A,
		nxtKeycode_K = 0x4B,
		nxtKeycode_L = 0x4C,
		nxtKeycode_M = 0x4D,
		nxtKeycode_N = 0x4E,
		nxtKeycode_O = 0x4F,
		nxtKeycode_P = 0x50,
		nxtKeycode_Q = 0x51,
		nxtKeycode_R = 0x52,
		nxtKeycode_S = 0x53,
		nxtKeycode_T = 0x54,
		nxtKeycode_U = 0x55,
		nxtKeycode_V = 0x56,
		nxtKeycode_W = 0x57,
		nxtKeycode_X = 0x58,
		nxtKeycode_Y = 0x59,
		nxtKeycode_Z = 0x5A,

		nxtKeycode_0NumPad = 0x60,
		nxtKeycode_1NumPad = 0x61,
		nxtKeycode_2NumPad = 0x62,
		nxtKeycode_3NumPad = 0x63,
		nxtKeycode_4NumPad = 0x64,
		nxtKeycode_5NumPad = 0x65,
		nxtKeycode_6NumPad = 0x66,
		nxtKeycode_7NumPad = 0x67,
		nxtKeycode_8NumPad = 0x68,
		nxtKeycode_9NumPad = 0x69,
		
		nxtKeycode_Plus = 0xBB,
		nxtKeycode_Minus = 0xBD
	};

}
