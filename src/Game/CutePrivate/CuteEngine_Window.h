#pragma once
//=============================================================================
extern CuteEngineApp* thisCuteEngineApp;
//=============================================================================
struct handleCloser { void operator()(HANDLE h) noexcept { if (h) ::CloseHandle(h); } };
using ScopedHandle = std::unique_ptr<void, handleCloser>;
//=============================================================================
struct MouseState final
{
	bool             leftButton{ false };
	bool             middleButton{ false };
	bool             rightButton{ false };
	bool             xButton1{ false };
	bool             xButton2{ false };
	int              x{ 0 };
	int              y{ 0 };
	int              scrollWheelValue{ 0 };
	Input::MouseMode positionMode{ Input::MouseMode::Absolute };
};
//=============================================================================
enum ButtonState : uint32_t
{
	UP = 0,       // Button is up
	HELD = 1,     // Button is held down
	RELEASED = 2, // Button was just released
	PRESSED = 3,  // Buton was just pressed
};
//=============================================================================
struct KeyboardState final
{
	bool Reserved0 : 8;
	bool Back : 1;              // VK_BACK, 0x8
	bool Tab : 1;               // VK_TAB, 0x9
	bool Reserved1 : 3;
	bool Enter : 1;             // VK_RETURN, 0xD
	bool Reserved2 : 2;
	bool Reserved3 : 3;
	bool Pause : 1;             // VK_PAUSE, 0x13
	bool CapsLock : 1;          // VK_CAPITAL, 0x14
	bool Kana : 1;              // VK_KANA, 0x15
	bool ImeOn : 1;             // VK_IME_ON, 0x16
	bool Reserved4 : 1;
	bool Reserved5 : 1;
	bool Kanji : 1;             // VK_KANJI, 0x19
	bool ImeOff : 1;            // VK_IME_OFF, 0X1A
	bool Escape : 1;            // VK_ESCAPE, 0x1B
	bool ImeConvert : 1;        // VK_CONVERT, 0x1C
	bool ImeNoConvert : 1;      // VK_NONCONVERT, 0x1D
	bool Reserved7 : 2;
	bool Space : 1;             // VK_SPACE, 0x20
	bool PageUp : 1;            // VK_PRIOR, 0x21
	bool PageDown : 1;          // VK_NEXT, 0x22
	bool End : 1;               // VK_END, 0x23
	bool Home : 1;              // VK_HOME, 0x24
	bool Left : 1;              // VK_LEFT, 0x25
	bool Up : 1;                // VK_UP, 0x26
	bool Right : 1;             // VK_RIGHT, 0x27
	bool Down : 1;              // VK_DOWN, 0x28
	bool Select : 1;            // VK_SELECT, 0x29
	bool Print : 1;             // VK_PRINT, 0x2A
	bool Execute : 1;           // VK_EXECUTE, 0x2B
	bool PrintScreen : 1;       // VK_SNAPSHOT, 0x2C
	bool Insert : 1;            // VK_INSERT, 0x2D
	bool Delete : 1;            // VK_DELETE, 0x2E
	bool Help : 1;              // VK_HELP, 0x2F
	bool D0 : 1;                // 0x30
	bool D1 : 1;                // 0x31
	bool D2 : 1;                // 0x32
	bool D3 : 1;                // 0x33
	bool D4 : 1;                // 0x34
	bool D5 : 1;                // 0x35
	bool D6 : 1;                // 0x36
	bool D7 : 1;                // 0x37
	bool D8 : 1;                // 0x38
	bool D9 : 1;                // 0x39
	bool Reserved8 : 6;
	bool Reserved9 : 1;
	bool A : 1;                 // 0x41
	bool B : 1;                 // 0x42
	bool C : 1;                 // 0x43
	bool D : 1;                 // 0x44
	bool E : 1;                 // 0x45
	bool F : 1;                 // 0x46
	bool G : 1;                 // 0x47
	bool H : 1;                 // 0x48
	bool I : 1;                 // 0x49
	bool J : 1;                 // 0x4A
	bool K : 1;                 // 0x4B
	bool L : 1;                 // 0x4C
	bool M : 1;                 // 0x4D
	bool N : 1;                 // 0x4E
	bool O : 1;                 // 0x4F
	bool P : 1;                 // 0x50
	bool Q : 1;                 // 0x51
	bool R : 1;                 // 0x52
	bool S : 1;                 // 0x53
	bool T : 1;                 // 0x54
	bool U : 1;                 // 0x55
	bool V : 1;                 // 0x56
	bool W : 1;                 // 0x57
	bool X : 1;                 // 0x58
	bool Y : 1;                 // 0x59
	bool Z : 1;                 // 0x5A
	bool LeftWindows : 1;       // VK_LWIN, 0x5B
	bool RightWindows : 1;      // VK_RWIN, 0x5C
	bool Apps : 1;              // VK_APPS, 0x5D
	bool Reserved10 : 1;
	bool Sleep : 1;             // VK_SLEEP, 0x5F
	bool NumPad0 : 1;           // VK_NUMPAD0, 0x60
	bool NumPad1 : 1;           // VK_NUMPAD1, 0x61
	bool NumPad2 : 1;           // VK_NUMPAD2, 0x62
	bool NumPad3 : 1;           // VK_NUMPAD3, 0x63
	bool NumPad4 : 1;           // VK_NUMPAD4, 0x64
	bool NumPad5 : 1;           // VK_NUMPAD5, 0x65
	bool NumPad6 : 1;           // VK_NUMPAD6, 0x66
	bool NumPad7 : 1;           // VK_NUMPAD7, 0x67
	bool NumPad8 : 1;           // VK_NUMPAD8, 0x68
	bool NumPad9 : 1;           // VK_NUMPAD9, 0x69
	bool Multiply : 1;          // VK_MULTIPLY, 0x6A
	bool Add : 1;               // VK_ADD, 0x6B
	bool Separator : 1;         // VK_SEPARATOR, 0x6C
	bool Subtract : 1;          // VK_SUBTRACT, 0x6D
	bool Decimal : 1;           // VK_DECIMANL, 0x6E
	bool Divide : 1;            // VK_DIVIDE, 0x6F
	bool F1 : 1;                // VK_F1, 0x70
	bool F2 : 1;                // VK_F2, 0x71
	bool F3 : 1;                // VK_F3, 0x72
	bool F4 : 1;                // VK_F4, 0x73
	bool F5 : 1;                // VK_F5, 0x74
	bool F6 : 1;                // VK_F6, 0x75
	bool F7 : 1;                // VK_F7, 0x76
	bool F8 : 1;                // VK_F8, 0x77
	bool F9 : 1;                // VK_F9, 0x78
	bool F10 : 1;               // VK_F10, 0x79
	bool F11 : 1;               // VK_F11, 0x7A
	bool F12 : 1;               // VK_F12, 0x7B
	bool F13 : 1;               // VK_F13, 0x7C
	bool F14 : 1;               // VK_F14, 0x7D
	bool F15 : 1;               // VK_F15, 0x7E
	bool F16 : 1;               // VK_F16, 0x7F
	bool F17 : 1;               // VK_F17, 0x80
	bool F18 : 1;               // VK_F18, 0x81
	bool F19 : 1;               // VK_F19, 0x82
	bool F20 : 1;               // VK_F20, 0x83
	bool F21 : 1;               // VK_F21, 0x84
	bool F22 : 1;               // VK_F22, 0x85
	bool F23 : 1;               // VK_F23, 0x86
	bool F24 : 1;               // VK_F24, 0x87
	bool Reserved11 : 8;
	bool NumLock : 1;           // VK_NUMLOCK, 0x90
	bool Scroll : 1;            // VK_SCROLL, 0x91
	bool Reserved12 : 6;
	bool Reserved13 : 8;
	bool LeftShift : 1;         // VK_LSHIFT, 0xA0
	bool RightShift : 1;        // VK_RSHIFT, 0xA1
	bool LeftControl : 1;       // VK_LCONTROL, 0xA2
	bool RightControl : 1;      // VK_RCONTROL, 0xA3
	bool LeftAlt : 1;           // VK_LMENU, 0xA4
	bool RightAlt : 1;          // VK_RMENU, 0xA5
	bool BrowserBack : 1;       // VK_BROWSER_BACK, 0xA6
	bool BrowserForward : 1;    // VK_BROWSER_FORWARD, 0xA7
	bool BrowserRefresh : 1;    // VK_BROWSER_REFRESH, 0xA8
	bool BrowserStop : 1;       // VK_BROWSER_STOP, 0xA9
	bool BrowserSearch : 1;     // VK_BROWSER_SEARCH, 0xAA
	bool BrowserFavorites : 1;  // VK_BROWSER_FAVORITES, 0xAB
	bool BrowserHome : 1;       // VK_BROWSER_HOME, 0xAC
	bool VolumeMute : 1;        // VK_VOLUME_MUTE, 0xAD
	bool VolumeDown : 1;        // VK_VOLUME_DOWN, 0xAE
	bool VolumeUp : 1;          // VK_VOLUME_UP, 0xAF
	bool MediaNextTrack : 1;    // VK_MEDIA_NEXT_TRACK, 0xB0
	bool MediaPreviousTrack : 1;// VK_MEDIA_PREV_TRACK, 0xB1
	bool MediaStop : 1;         // VK_MEDIA_STOP, 0xB2
	bool MediaPlayPause : 1;    // VK_MEDIA_PLAY_PAUSE, 0xB3
	bool LaunchMail : 1;        // VK_LAUNCH_MAIL, 0xB4
	bool SelectMedia : 1;       // VK_LAUNCH_MEDIA_SELECT, 0xB5
	bool LaunchApplication1 : 1;// VK_LAUNCH_APP1, 0xB6
	bool LaunchApplication2 : 1;// VK_LAUNCH_APP2, 0xB7
	bool Reserved14 : 2;
	bool OemSemicolon : 1;      // VK_OEM_1, 0xBA
	bool OemPlus : 1;           // VK_OEM_PLUS, 0xBB
	bool OemComma : 1;          // VK_OEM_COMMA, 0xBC
	bool OemMinus : 1;          // VK_OEM_MINUS, 0xBD
	bool OemPeriod : 1;         // VK_OEM_PERIOD, 0xBE
	bool OemQuestion : 1;       // VK_OEM_2, 0xBF
	bool OemTilde : 1;          // VK_OEM_3, 0xC0
	bool Reserved15 : 7;
	bool Reserved16 : 8;
	bool Reserved17 : 8;
	bool Reserved18 : 3;
	bool OemOpenBrackets : 1;   // VK_OEM_4, 0xDB
	bool OemPipe : 1;           // VK_OEM_5, 0xDC
	bool OemCloseBrackets : 1;  // VK_OEM_6, 0xDD
	bool OemQuotes : 1;         // VK_OEM_7, 0xDE
	bool Oem8 : 1;              // VK_OEM_8, 0xDF
	bool Reserved19 : 2;
	bool OemBackslash : 1;      // VK_OEM_102, 0xE2
	bool Reserved20 : 2;
	bool ProcessKey : 1;        // VK_PROCESSKEY, 0xE5
	bool Reserved21 : 2;
	bool Reserved22 : 8;
	bool Reserved23 : 2;
	bool OemCopy : 1;           // 0XF2
	bool OemAuto : 1;           // 0xF3
	bool OemEnlW : 1;           // 0xF4
	bool Reserved24 : 1;
	bool Attn : 1;              // VK_ATTN, 0xF6
	bool Crsel : 1;             // VK_CRSEL, 0xF7
	bool Exsel : 1;             // VK_EXSEL, 0xF8
	bool EraseEof : 1;          // VK_EREOF, 0xF9
	bool Play : 1;              // VK_PLAY, 0xFA
	bool Zoom : 1;              // VK_ZOOM, 0xFB
	bool Reserved25 : 1;
	bool Pa1 : 1;               // VK_PA1, 0xFD
	bool OemClear : 1;          // VK_OEM_CLEAR, 0xFE
	bool Reserved26 : 1;

	bool __cdecl IsKeyDown(Input::Keys key) const noexcept
	{
		if (key <= 0xfe)
		{
			auto ptr = reinterpret_cast<const uint32_t*>(this);
			const unsigned int bf = 1u << (key & 0x1f);
			return (ptr[(key >> 5)] & bf) != 0;
		}
		return false;
	}

	bool __cdecl IsKeyUp(Input::Keys key) const noexcept
	{
		if (key <= 0xfe)
		{
			auto ptr = reinterpret_cast<const uint32_t*>(this);
			const unsigned int bf = 1u << (key & 0x1f);
			return (ptr[(key >> 5)] & bf) == 0;
		}
		return false;
	}
};
//=============================================================================
namespace windowData
{
	constexpr const auto windowClassName = L"Cute Window Class";

	uint32_t  width{ 0 };
	uint32_t  height{ 0 };
	uint32_t  widthInWindowMode{ 0 };
	uint32_t  heightInWindowMode{ 0 };
	HINSTANCE handleInstance{ nullptr };
	HWND      hwnd{ nullptr };
	MSG       msg{};
	bool      isCloseRequested{ true };
	bool      isResized{ false };
	bool      isSizemove{ false };
	bool      isSuspend{ false };
	bool      isMinimized{ false };
	bool      isMaximized{ false };
	bool      fullScreen{ false };

	void      (*dropCallback)(char const*, uint32_t, void*) = nullptr;
	void*     callbackData = nullptr;
} // namespace windowData
//=============================================================================
namespace mouseData
{
	Input::MouseMode mode = Input::MouseMode::Absolute;
	MouseState       state{};
	MouseState       currentState{};
	MouseState       lastState{};

	ButtonState      leftButton;
	ButtonState      middleButton;
	ButtonState      rightButton;
	ButtonState      xButton1;
	ButtonState      xButton2;

	ScopedHandle     scrollWheelValue;
	ScopedHandle     relativeRead;
	ScopedHandle     absoluteMode;
	ScopedHandle     relativeMode;

	int              lastX{ 0 };
	int              lastY{ 0 };
	int              relativeX{ INT32_MAX };
	int              relativeY{ INT32_MAX };

	bool             inFocus{ true };
	bool             autoReset{ true };

} // namespace mouseData
//=============================================================================
namespace keyboardData
{
	KeyboardState state{};

	KeyboardState released{};
	KeyboardState pressed{};
	KeyboardState lastState{};
} // namespace keyboardData
//=============================================================================
enum CreateWindowFlag
{
	MaximizeWindow = 1 << 0,
	NoResizeWindow = 1 << 1,
	FullscreenWindow = 1 << 2,
	AcceptDrop = 1 << 3
};
typedef uint32_t CreateWindowFlags;
//=============================================================================
IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//=============================================================================
void clipToWindow()
{
	RECT rect = {};
	std::ignore = ::GetClientRect(windowData::hwnd, &rect);

	POINT ul;
	ul.x = rect.left;
	ul.y = rect.top;

	POINT lr;
	lr.x = rect.right;
	lr.y = rect.bottom;

	std::ignore = ::MapWindowPoints(windowData::hwnd, nullptr, &ul, 1);
	std::ignore = ::MapWindowPoints(windowData::hwnd, nullptr, &lr, 1);

	rect.left = ul.x;
	rect.top = ul.y;

	rect.right = lr.x;
	rect.bottom = lr.y;

	::ClipCursor(&rect);
}
//=============================================================================
void mouseProcessMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	// First handle any pending scroll wheel reset event.
	switch (WaitForSingleObjectEx(mouseData::scrollWheelValue.get(), 0, FALSE))
	{
	default:
	case WAIT_TIMEOUT:
		break;

	case WAIT_OBJECT_0:
		mouseData::state.scrollWheelValue = 0;
		ResetEvent(mouseData::scrollWheelValue.get());
		break;

	case WAIT_FAILED:
		Fatal("WaitForMultipleObjectsEx");
		return;
	}

	// Next handle mode change events.
	HANDLE events[2] = { mouseData::absoluteMode.get(), mouseData::relativeMode.get() };
	switch (WaitForMultipleObjectsEx(static_cast<DWORD>(std::size(events)), events, FALSE, 0, FALSE))
	{
	default:
	case WAIT_TIMEOUT:
		break;

	case WAIT_OBJECT_0:
	{
		mouseData::mode = Input::MouseMode::Absolute;
		ClipCursor(nullptr);

		POINT point;
		point.x = mouseData::lastX;
		point.y = mouseData::lastY;

		// We show the cursor before moving it to support Remote Desktop
		ShowCursor(TRUE);

		if (MapWindowPoints(windowData::hwnd, nullptr, &point, 1))
		{
			SetCursorPos(point.x, point.y);
		}
		mouseData::state.x = mouseData::lastX;
		mouseData::state.y = mouseData::lastY;
	}
	break;

	case (WAIT_OBJECT_0 + 1):
	{
		ResetEvent(mouseData::relativeRead.get());

		mouseData::mode = Input::MouseMode::Relative;
		mouseData::state.x = mouseData::state.y = 0;
		mouseData::relativeX = INT32_MAX;
		mouseData::relativeY = INT32_MAX;

		ShowCursor(FALSE);

		clipToWindow();
	}
	break;

	case WAIT_FAILED:
		Fatal("WaitForMultipleObjectsEx");
		return;
	}

	switch (message)
	{
	case WM_ACTIVATE:
	case WM_ACTIVATEAPP:
		if (wParam)
		{
			mouseData::inFocus = true;

			if (mouseData::mode == Input::MouseMode::Relative)
			{
				mouseData::state.x = mouseData::state.y = 0;

				ShowCursor(FALSE);

				clipToWindow();
			}
		}
		else
		{
			const int scrollWheel = mouseData::state.scrollWheelValue;
			memset(&mouseData::state, 0, sizeof(MouseState));
			mouseData::state.scrollWheelValue = scrollWheel;

			if (mouseData::mode == Input::MouseMode::Relative)
			{
				ClipCursor(nullptr);
			}

			mouseData::inFocus = false;
		}
		return;

	case WM_INPUT:
		if (mouseData::inFocus && mouseData::mode == Input::MouseMode::Relative)
		{
			RAWINPUT raw;
			UINT rawSize = sizeof(raw);

			const UINT resultData = GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, &raw, &rawSize, sizeof(RAWINPUTHEADER));
			if (resultData == UINT(-1))
			{
				Fatal("GetRawInputData");
				return;
			}

			if (raw.header.dwType == RIM_TYPEMOUSE)
			{
				if (!(raw.data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE))
				{
					mouseData::state.x += raw.data.mouse.lLastX;
					mouseData::state.y += raw.data.mouse.lLastY;

					ResetEvent(mouseData::relativeRead.get());
				}
				else if (raw.data.mouse.usFlags & MOUSE_VIRTUAL_DESKTOP)
				{
					// This is used to make Remote Desktop sessons work
					const int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
					const int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);

					const auto x = static_cast<int>((float(raw.data.mouse.lLastX) / 65535.0f) * float(width));
					const auto y = static_cast<int>((float(raw.data.mouse.lLastY) / 65535.0f) * float(height));

					if (mouseData::relativeX == INT32_MAX)
					{
						mouseData::state.x = mouseData::state.y = 0;
					}
					else
					{
						mouseData::state.x = x - mouseData::relativeX;
						mouseData::state.y = y - mouseData::relativeY;
					}

					mouseData::relativeX = x;
					mouseData::relativeY = y;

					ResetEvent(mouseData::relativeRead.get());
				}
			}
		}
		return;

	case WM_MOUSEMOVE:
		break;

	case WM_LBUTTONDOWN:
		mouseData::state.leftButton = true;
		break;

	case WM_LBUTTONUP:
		mouseData::state.leftButton = false;
		break;

	case WM_RBUTTONDOWN:
		mouseData::state.rightButton = true;
		break;

	case WM_RBUTTONUP:
		mouseData::state.rightButton = false;
		break;

	case WM_MBUTTONDOWN:
		mouseData::state.middleButton = true;
		break;

	case WM_MBUTTONUP:
		mouseData::state.middleButton = false;
		break;

	case WM_MOUSEWHEEL:
		mouseData::state.scrollWheelValue += GET_WHEEL_DELTA_WPARAM(wParam);
		return;

	case WM_XBUTTONDOWN:
		switch (GET_XBUTTON_WPARAM(wParam))
		{
		case XBUTTON1:
			mouseData::state.xButton1 = true;
			break;

		case XBUTTON2:
			mouseData::state.xButton2 = true;
			break;

		default:
			break;
		}
		break;

	case WM_XBUTTONUP:
		switch (GET_XBUTTON_WPARAM(wParam))
		{
		case XBUTTON1:
			mouseData::state.xButton1 = false;
			break;

		case XBUTTON2:
			mouseData::state.xButton2 = false;
			break;

		default:
			break;
		}
		break;

	case WM_MOUSEHOVER:
		break;

	default:
		// Not a mouse message, so exit
		return;
	}

	if (mouseData::mode == Input::MouseMode::Absolute)
	{
		// All mouse messages provide a new pointer position
		const int xPos = static_cast<short>(LOWORD(lParam)); // GET_X_LPARAM(lParam);
		const int yPos = static_cast<short>(HIWORD(lParam)); // GET_Y_LPARAM(lParam);

		mouseData::state.x = mouseData::lastX = xPos;
		mouseData::state.y = mouseData::lastY = yPos;
	}
}
//=============================================================================
void mouseUpdate()
{
	memcpy(&mouseData::currentState, &mouseData::state, sizeof(MouseState));
	mouseData::currentState.positionMode = mouseData::mode;

	DWORD result = WaitForSingleObjectEx(mouseData::scrollWheelValue.get(), 0, FALSE);
	if (result == WAIT_FAILED)
	{
		Fatal("WaitForSingleObjectEx");
		return;
	}

	if (result == WAIT_OBJECT_0)
	{
		mouseData::currentState.scrollWheelValue = 0;
	}

	if (mouseData::currentState.positionMode == Input::MouseMode::Relative)
	{
		result = WaitForSingleObjectEx(mouseData::relativeRead.get(), 0, FALSE);

		if (result == WAIT_FAILED)
		{
			Fatal("WaitForSingleObjectEx");
			return;
		}

		if (result == WAIT_OBJECT_0)
		{
			mouseData::currentState.x = mouseData::currentState.y = 0;
		}
		else
		{
			SetEvent(mouseData::relativeRead.get());
		}

		if (mouseData::autoReset)
		{
			mouseData::state.x = mouseData::state.y = 0;
		}
	}

#define UPDATE_BUTTON_STATE(field) mouseData::field = static_cast<ButtonState>( ( !!mouseData::currentState.field ) | ( ( !!mouseData::currentState.field ^ !!mouseData::lastState.field ) << 1 ) )

	UPDATE_BUTTON_STATE(leftButton);

	assert((!mouseData::currentState.leftButton && !mouseData::lastState.leftButton) == (mouseData::leftButton == UP));
	assert((mouseData::currentState.leftButton && mouseData::lastState.leftButton) == (mouseData::leftButton == HELD));
	assert((!mouseData::currentState.leftButton && mouseData::lastState.leftButton) == (mouseData::leftButton == RELEASED));
	assert((mouseData::currentState.leftButton && !mouseData::lastState.leftButton) == (mouseData::leftButton == PRESSED));

	UPDATE_BUTTON_STATE(middleButton);
	UPDATE_BUTTON_STATE(rightButton);
	UPDATE_BUTTON_STATE(xButton1);
	UPDATE_BUTTON_STATE(xButton2);

	mouseData::lastState = mouseData::currentState;

#undef UPDATE_BUTTON_STATE
}
//=============================================================================
void keyboardUpdate()
{
	auto currPtr = reinterpret_cast<const uint32_t*>(&keyboardData::state);
	auto prevPtr = reinterpret_cast<const uint32_t*>(&keyboardData::lastState);
	auto releasedPtr = reinterpret_cast<uint32_t*>(&keyboardData::released);
	auto pressedPtr = reinterpret_cast<uint32_t*>(&keyboardData::pressed);
	for (size_t j = 0; j < (256 / 32); ++j)
	{
		*pressedPtr = *currPtr & ~(*prevPtr);
		*releasedPtr = ~(*currPtr) & *prevPtr;

		++currPtr;
		++prevPtr;
		++releasedPtr;
		++pressedPtr;
	}

	keyboardData::lastState = keyboardData::state;
}
//=============================================================================
void keyboardReset()
{
	memset(&keyboardData::state, 0, sizeof(KeyboardState));
	memset(&keyboardData::lastState, 0, sizeof(KeyboardState));
	memset(&keyboardData::pressed, 0, sizeof(KeyboardState));
	memset(&keyboardData::released , 0, sizeof(KeyboardState));
}
//=============================================================================
namespace
{
	inline void keyDown(int key, KeyboardState& state) noexcept
	{
		if (key < 0 || key > 0xfe)
			return;

		auto ptr = reinterpret_cast<uint32_t*>(&state);

		const unsigned int bf = 1u << (key & 0x1f);
		ptr[(key >> 5)] |= bf;
	}

	inline void keyUp(int key, KeyboardState& state) noexcept
	{
		if (key < 0 || key > 0xfe)
			return;

		auto ptr = reinterpret_cast<uint32_t*>(&state);

		const unsigned int bf = 1u << (key & 0x1f);
		ptr[(key >> 5)] &= ~bf;
	}
}
//=============================================================================
void keyboardProcessMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	bool down = false;

	switch (message)
	{
	case WM_ACTIVATE:
	case WM_ACTIVATEAPP:
		keyboardReset();
		return;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		down = true;
		break;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		break;

	default:
		return;
	}

	int vk = LOWORD(wParam);
	// We want to distinguish left and right shift/ctrl/alt keys
	switch (vk)
	{
	case VK_SHIFT:
	case VK_CONTROL:
	case VK_MENU:
	{
		if (vk == VK_SHIFT && !down)
		{
			// Workaround to ensure left vs. right shift get cleared when both were pressed at same time
			keyUp(VK_LSHIFT, keyboardData::state);
			keyUp(VK_RSHIFT, keyboardData::state);
		}

		bool isExtendedKey = (HIWORD(lParam) & KF_EXTENDED) == KF_EXTENDED;
		int scanCode = LOBYTE(HIWORD(lParam)) | (isExtendedKey ? 0xe000 : 0);
		vk = LOWORD(MapVirtualKeyW(static_cast<UINT>(scanCode), MAPVK_VSC_TO_VK_EX));
	}
	break;

	default:
		break;
	}

	if (down)
	{
		keyDown(vk, keyboardData::state);
	}
	else
	{
		keyUp(vk, keyboardData::state);
	}
}
//=============================================================================
void suspending()
{
	thisCuteEngineApp->OnSuspending();
}
//=============================================================================
void resuming()
{
	thisCuteEngineApp->OnResuming();
}
//=============================================================================
void windowSizeChanged(uint32_t width, uint32_t height)
{
	windowData::isResized = true;
	windowData::width = width;
	windowData::height = height;
	thisCuteEngineApp->OnWindowSizeChanged(width, height);
}
//=============================================================================
void activated()
{
	thisCuteEngineApp->OnActivated();
}
//=============================================================================
void deactivated()
{
	thisCuteEngineApp->OnDeactivated();
}
//=============================================================================
LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
	if (message == WM_DESTROY) [[unlikely]]
	{
		PostQuitMessage(0);
		return 0;
	}

	if (thisCuteEngineApp) [[likely]]
	{
		mouseProcessMessage(message, wParam, lParam);
		keyboardProcessMessage(message, wParam, lParam);

		if (ImGui::GetCurrentContext() != nullptr && ImGui::GetIO().BackendPlatformUserData != nullptr)
		{
			ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam);
		}

		switch (message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_ACTIVATE:
		case WM_ACTIVATEAPP:
			if (wParam)
			{
				activated();
			}
			else
			{
				deactivated();
			}
			break;

		case WM_KILLFOCUS:
			break;

		case WM_DROPFILES:
		{
			HDROP hdrop = (HDROP)wParam;
			if (windowData::dropCallback != nullptr)
			{
				char file_name[MAX_PATH];
				// Get the number of files dropped onto window
				uint32_t const file_count = DragQueryFileA(hdrop, 0xFFFFFFFFu, file_name, MAX_PATH);
				for (uint32_t i = 0; i < file_count; i++)
				{
					// Get i'th filename
					DragQueryFileA(hdrop, i, file_name, MAX_PATH);

					// Pass to callback function
					(*windowData::dropCallback)(file_name, i, windowData::callbackData);
				}
			}
			DragFinish(hdrop);
		}
		break;

		case WM_SIZE:
			windowData::isMaximized = IsZoomed(hwnd);

			if (wParam == SIZE_MINIMIZED)
			{
				if (!windowData::isMinimized)
				{
					windowData::isMinimized = true;
					if (!windowData::isSuspend)
						suspending();
					windowData::isSuspend = true;
				}
			}
			else if (windowData::isMinimized)
			{
				windowData::isMinimized = false;
				if (windowData::isSuspend)
					resuming();
				windowData::isSuspend = false;
			}
			else if (!windowData::isSizemove)
			{
				windowSizeChanged(static_cast<uint32_t>(LOWORD(lParam)), static_cast<uint32_t>(HIWORD(lParam)));
			}
			break;

		case WM_ENTERSIZEMOVE:
			windowData::isSizemove = true;
			break;

		case WM_EXITSIZEMOVE:
			windowData::isSizemove = false;
			{
				RECT rc;
				GetClientRect(hwnd, &rc);
				windowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
			}
			break;

		case WM_GETMINMAXINFO:
			if (lParam)
			{
				auto info = reinterpret_cast<MINMAXINFO*>(lParam);
				info->ptMinTrackSize.x = 320;
				info->ptMinTrackSize.y = 200;
			}
			break;

		case WM_POWERBROADCAST:
			switch (wParam)
			{
			case PBT_APMQUERYSUSPEND:
				if (!windowData::isSuspend)
					suspending();
				windowData::isSuspend = true;
				return TRUE;

			case PBT_APMRESUMESUSPEND:
				if (!windowData::isMinimized)
				{
					if (windowData::isSuspend)
						resuming();
					windowData::isSuspend = false;
				}
				return TRUE;

			default:
				break;
			}
			break;

		case WM_MOUSEACTIVATE:
			// Когда вы нажимаете на кнопку активации окна, мы хотим, чтобы мышь игнорировала его.
			return MA_ACTIVATEANDEAT;
		case WM_MENUCHAR:
			// A menu is active and the user presses a key that does not correspond to any mnemonic or accelerator key. Ignore so we don't produce an error beep.
			return MAKELRESULT(0, 1/*MNC_CLOSE*/);

		case WM_SYSKEYDOWN:
			if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
			{
				// Implements the classic ALT+ENTER fullscreen toggle
				if (windowData::fullScreen)
				{
					SetWindowLongPtr(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
					SetWindowLongPtr(hwnd, GWL_EXSTYLE, 0);

					int width = windowData::widthInWindowMode;
					int height = windowData::heightInWindowMode;

					ShowWindow(hwnd, SW_SHOWNORMAL);
					SetWindowPos(hwnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
				}
				else
				{
					windowData::widthInWindowMode = windowData::width;
					windowData::heightInWindowMode = windowData::height;
					SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP);
					SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_TOPMOST);

					SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

					ShowWindow(hwnd, SW_SHOWMAXIMIZED);
				}

				windowData::fullScreen = !windowData::fullScreen;
			}
			break;
		}
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
//=============================================================================
bool InitWindow(uint32_t width, uint32_t height, std::wstring_view title, CreateWindowFlags flags)
{
	if (width == 0)  width = 800;
	if (height == 0) height = 600;

	windowData::isCloseRequested = true;
	windowData::fullScreen = (flags & FullscreenWindow) != 0;
	windowData::handleInstance = GetModuleHandle(nullptr);

	WNDCLASSEX windowClassInfo{ .cbSize = sizeof(WNDCLASSEX) };
	windowClassInfo.style         = CS_HREDRAW | CS_VREDRAW;
	windowClassInfo.lpfnWndProc   = WindowProc;
	windowClassInfo.hInstance     = windowData::handleInstance;
	windowClassInfo.hIcon         = LoadIcon(windowData::handleInstance, L"IDI_ICON");
	windowClassInfo.hIconSm       = LoadIcon(windowData::handleInstance, L"IDI_ICON");
	windowClassInfo.hCursor       = LoadCursor(nullptr, IDC_ARROW);
	windowClassInfo.hbrBackground = reinterpret_cast<HBRUSH>(5/*COLOR_WINDOW*/ + 1);
	windowClassInfo.lpszClassName = windowData::windowClassName;
	if (!RegisterClassEx(&windowClassInfo))
	{
		Fatal("Failed to register window class.");
		return false;
	}

	const DWORD windowStyle = WS_OVERLAPPEDWINDOW & ~((flags & NoResizeWindow) != 0 ? WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX : 0);

	const int screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	const int windowLeft   = screenWidth / 2 - (int)width / 2;
	const int windowTop    = screenHeight / 2 - (int)height / 2;

	RECT windowRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
	AdjustWindowRect(&windowRect, windowStyle, FALSE);
	LONG windowWidth = windowRect.right - windowRect.left;
	LONG windowHeight = windowRect.bottom - windowRect.top;

	windowData::hwnd = CreateWindowEx(
		(flags & AcceptDrop) != 0 ? WS_EX_ACCEPTFILES : 0,
		windowData::windowClassName, title.data(),
		windowStyle,
		windowLeft, windowTop, windowWidth, windowHeight,
		nullptr, nullptr, windowData::handleInstance, nullptr);
	if (!windowData::hwnd)
	{
		Fatal("Failed to create window.");
		return false;
	}

	if (windowData::fullScreen)
	{
		WINDOWPLACEMENT g_wpPrev;
		memset(&g_wpPrev, 0, sizeof(g_wpPrev));
		g_wpPrev.length = sizeof(g_wpPrev);
		DWORD dwStyle = GetWindowLong(windowData::hwnd, GWL_STYLE);

		if ((dwStyle & WS_OVERLAPPEDWINDOW) != 0)
		{
			MONITORINFO mi;
			memset(&mi, 0, sizeof(mi));
			mi.cbSize = sizeof(mi);
			if (GetWindowPlacement(windowData::hwnd, &g_wpPrev) && GetMonitorInfo(MonitorFromWindow(windowData::hwnd, MONITOR_DEFAULTTOPRIMARY), &mi))
			{
				SetWindowLong(windowData::hwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(windowData::hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
					mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top,
					SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
			}
		}
		else
		{
			SetWindowLong(windowData::hwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
			SetWindowPlacement(windowData::hwnd, &g_wpPrev);
			SetWindowPos(windowData::hwnd, nullptr, 0, 0, 0, 0,
				SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
	}

	ShowWindow(windowData::hwnd, (flags & MaximizeWindow) != 0 ? SW_SHOWMAXIMIZED : SW_SHOWDEFAULT);

	GetClientRect(windowData::hwnd, &windowRect);
	windowData::width = static_cast<uint32_t>(windowRect.right - windowRect.left);
	windowData::height = static_cast<uint32_t>(windowRect.bottom - windowRect.top);
	windowData::widthInWindowMode = windowData::width;
	windowData::heightInWindowMode = windowData::height;

	// init input
	{
		keyboardReset();

		mouseData::state = {};
		mouseData::currentState = {};
		mouseData::lastState = {};
		mouseData::mode = Input::MouseMode::Absolute;
		mouseData::lastX = 0;
		mouseData::lastY = 0;
		mouseData::relativeX = INT32_MAX;
		mouseData::relativeY = INT32_MAX;
		mouseData::inFocus = true;
		mouseData::autoReset = true;

		mouseData::scrollWheelValue.reset(CreateEventEx(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, EVENT_MODIFY_STATE | SYNCHRONIZE));
		mouseData::relativeRead.reset(CreateEventEx(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, EVENT_MODIFY_STATE | SYNCHRONIZE));
		mouseData::absoluteMode.reset(CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE));
		mouseData::relativeMode.reset(CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE));
		if (!mouseData::scrollWheelValue
			|| !mouseData::relativeRead
			|| !mouseData::absoluteMode
			|| !mouseData::relativeMode)
		{
			Fatal("CreateEventEx");
			return false;
		}

		RAWINPUTDEVICE Rid;
		Rid.usUsagePage = 0x1 /* HID_USAGE_PAGE_GENERIC */;
		Rid.usUsage     = 0x2 /* HID_USAGE_GENERIC_MOUSE */;
		Rid.dwFlags     = RIDEV_INPUTSINK;
		Rid.hwndTarget  = windowData::hwnd;
		if (!RegisterRawInputDevices(&Rid, 1, sizeof(RAWINPUTDEVICE)))
		{
			Fatal("RegisterRawInputDevices");
			return false;
		}
	}

	windowData::isResized = false;
	windowData::isSizemove = false;
	windowData::isSuspend = false;
	windowData::isMinimized = false;
	windowData::isMaximized = (flags & MaximizeWindow) != 0;
	windowData::isCloseRequested = false;
	return true;
}
//=============================================================================
void EndWindow()
{
	if (windowData::hwnd) DestroyWindow(windowData::hwnd);
	windowData::hwnd = nullptr;
	if (windowData::handleInstance) UnregisterClass(windowData::windowClassName, windowData::handleInstance);
	windowData::handleInstance = nullptr;
	windowData::isCloseRequested = true;
}
//=============================================================================
void PollEvent()
{
	while (PeekMessage(&windowData::msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&windowData::msg);
		DispatchMessage(&windowData::msg);

		if (windowData::msg.message == WM_QUIT)
		{
			windowData::isCloseRequested = true;
			break;
		}
	}
	mouseUpdate();
	keyboardUpdate();
}
//=============================================================================
void EndOfInputFrame()
{
	mouseData::autoReset = false;

	if (mouseData::mode == Input::MouseMode::Relative)
	{
		mouseData::state.x = mouseData::state.y = 0;
	}
}
//=============================================================================
void CuteEngineApp::SetWindowTitle(std::wstring_view title)
{
	if (!SetWindowTextW(windowData::hwnd, title.data()))
	{
		Fatal("Failed to set window title.");
	}
}
//=============================================================================
void CuteEngineApp::RegisterDropCallback(void(*callback)(char const*, uint32_t, void*), void* data)
{
	windowData::dropCallback = callback;
	windowData::callbackData = data;
}
//=============================================================================
void CuteEngineApp::UnregisterDropCallback()
{
	windowData::dropCallback = nullptr;
	windowData::callbackData = nullptr;
}
//=============================================================================
uint32_t CuteEngineApp::GetWindowWidth() const
{
	return windowData::width;
}
//=============================================================================
uint32_t CuteEngineApp::GetWindowHeight() const
{
	return windowData::height;
}
//=============================================================================
float CuteEngineApp::GetWindowAspect() const
{
	return static_cast<float>(GetWindowWidth()) / static_cast<float>(GetWindowHeight());
}
//=============================================================================
void CuteEngineApp::ResetScrollWheelValue() const
{
	SetEvent(mouseData::scrollWheelValue.get());
}
//=============================================================================
int CuteEngineApp::GetScrollWheelValue() const
{
	return mouseData::currentState.scrollWheelValue;
}
//=============================================================================
void CuteEngineApp::SetMouseMode(Input::MouseMode mode) const
{
	if (mouseData::mode == mode)
		return;

	SetEvent((mode == Input::MouseMode::Absolute) ? mouseData::absoluteMode.get() : mouseData::relativeMode.get());

	// Send a WM_HOVER as a way to 'kick' the message processing even if the mouse is still.
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.dwFlags = TME_HOVER;
	tme.hwndTrack = windowData::hwnd;
	tme.dwHoverTime = 1;
	if (!TrackMouseEvent(&tme))
	{
		Fatal("TrackMouseEvent");
	}
}
//=============================================================================
Input::MouseMode CuteEngineApp::GetMouseMode() const
{
	return mouseData::currentState.positionMode;
}
//=============================================================================
bool CuteEngineApp::IsMouseVisible() const
{
	if (mouseData::mode == Input::MouseMode::Relative)
		return false;

	CURSORINFO info = { sizeof(CURSORINFO), 0, nullptr, {} };
	if (!GetCursorInfo(&info))
		return false;

	return (info.flags & CURSOR_SHOWING) != 0;
}
//=============================================================================
void CuteEngineApp::SetMouseVisible(bool visible) const
{
	if (mouseData::mode == Input::MouseMode::Relative)
		return;

	CURSORINFO info = { sizeof(CURSORINFO), 0, nullptr, {} };
	if (!GetCursorInfo(&info))
	{
		Fatal("GetCursorInfo");
		return;
	}

	const bool isvisible = (info.flags & CURSOR_SHOWING) != 0;
	if (isvisible != visible)
	{
		::ShowCursor(visible);
	}
}
//=============================================================================
ButtonState isMouseState(Input::MouseButton button)
{
	switch (button)
	{
	case Input::MouseButton::Left:     return mouseData::leftButton;
	case Input::MouseButton::Right:    return mouseData::rightButton;
	case Input::MouseButton::Middle:   return mouseData::middleButton;
	case Input::MouseButton::xButton1: return mouseData::xButton1;
	case Input::MouseButton::xButton2: return mouseData::xButton2;
	}
}
//=============================================================================
bool CuteEngineApp::IsMouseUp(Input::MouseButton button) const
{
	return isMouseState(button) == ButtonState::UP || isMouseState(button) == ButtonState::RELEASED;
}
//=============================================================================
bool CuteEngineApp::IsMouseDown(Input::MouseButton button) const
{
	return isMouseState(button) == ButtonState::HELD || isMouseState(button) == ButtonState::PRESSED;
}
//=============================================================================
bool CuteEngineApp::IsMousePressed(Input::MouseButton button) const
{
	return isMouseState(button) == ButtonState::PRESSED;
}
//=============================================================================
bool CuteEngineApp::IsMouseReleased(Input::MouseButton button) const
{
	return isMouseState(button) == ButtonState::RELEASED;
}
//=============================================================================
Input::MousePosition CuteEngineApp::GetMousePosition() const
{
	return { mouseData::currentState.x, mouseData::currentState.y };
}
//=============================================================================
bool CuteEngineApp::IsKeyUp(Input::Keys key) const
{
	return keyboardData::state.IsKeyUp(key);
}
//=============================================================================
bool CuteEngineApp::IsKeyDown(Input::Keys key) const
{
	return keyboardData::state.IsKeyDown(key);
}
//=============================================================================
bool CuteEngineApp::IsKeyPressed(Input::Keys key) const
{
	return keyboardData::pressed.IsKeyDown(key);
}
//=============================================================================
 bool CuteEngineApp::IsKeyReleased(Input::Keys key) const
{
	return keyboardData::released.IsKeyDown(key);
}
//=============================================================================