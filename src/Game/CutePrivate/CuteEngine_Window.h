#pragma once
//=============================================================================
extern CuteEngineApp* thisCuteEngineApp;
//=============================================================================
struct handleCloser { void operator()(HANDLE h) noexcept { if (h) ::CloseHandle(h); } };
using ScopedHandle = std::unique_ptr<void, handleCloser>;
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
	bool      isMinimized{ false };
	bool      isMaximized{ false };
	bool      fullScreen{ false };

	void (*dropCallback)(char const*, uint32_t, void*) = nullptr;
	void*     callbackData = nullptr;
} // namespace windowData
//=============================================================================
namespace mouseData
{
	Input::MouseMode  mode = Input::MouseMode::Absolute;
	Input::MouseState state{};

	ScopedHandle      scrollWheelValue;
	ScopedHandle      relativeRead;
	ScopedHandle      absoluteMode;
	ScopedHandle      relativeMode;

	int               lastX{ 0 };
	int               lastY{ 0 };
	int               relativeX{ INT32_MAX };
	int               relativeY{ INT32_MAX };

	bool              inFocus{ true };
	bool              autoReset{ true };

} // namespace mouseData
//=============================================================================
namespace keyboardData
{
	Input::KeyboardState state{};
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
			memset(&mouseData::state, 0, sizeof(Input::MouseState));
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
void keyboardReset()
{
	memset(&keyboardData::state, 0, sizeof(Input::KeyboardState));
}
//=============================================================================
namespace
{
	inline void keyDown(int key, Input::KeyboardState& state) noexcept
	{
		if (key < 0 || key > 0xfe)
			return;

		auto ptr = reinterpret_cast<uint32_t*>(&state);

		const unsigned int bf = 1u << (key & 0x1f);
		ptr[(key >> 5)] |= bf;
	}

	inline void keyUp(int key, Input::KeyboardState& state) noexcept
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

		switch (message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_ACTIVATE:
		case WM_ACTIVATEAPP:
			if (wParam)
			{
				//game->OnActivated();
			}
			else
			{
				//game->OnDeactivated();
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
			windowData::isMinimized = IsIconic(hwnd);
			windowData::isMaximized = IsZoomed(hwnd);

			if (wParam != SIZE_MINIMIZED)
			{
				windowData::isResized = true;
				windowData::width = static_cast<uint32_t>(LOWORD(lParam));
				windowData::height = static_cast<uint32_t>(HIWORD(lParam));
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

		case WM_MOUSEACTIVATE:
			// Когда вы нажимаете на кнопку активации окна, мы хотим, чтобы мышь игнорировала его.
			return MA_ACTIVATEANDEAT;
		case WM_MENUCHAR:
			// A menu is active and the user presses a key that does not correspond to any mnemonic or accelerator key. Ignore so we don't produce an error beep.
			return MAKELRESULT(0, 1/*MNC_CLOSE*/);

		case WM_CHAR:
		case WM_SETCURSOR:
		case WM_DEVICECHANGE:
		case WM_KEYUP: case WM_SYSKEYUP:
		case WM_KEYDOWN: case WM_SYSKEYDOWN:
		case WM_LBUTTONUP: case WM_RBUTTONUP:
		case WM_MBUTTONUP: case WM_XBUTTONUP:
		case WM_MOUSEWHEEL: case WM_MOUSEHWHEEL:
		case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
		case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK:

			if (ImGui::GetCurrentContext() != nullptr && ImGui::GetIO().BackendPlatformUserData != nullptr)
			{
				if (ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam))
					return true;
			}

			break;

		//case WM_SYSKEYDOWN:
		//	if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
		//	{
		//		// Implements the classic ALT+ENTER fullscreen toggle
		//		/*if (thisWindowSystem->m_fullScreen)
		//		{
		//			SetWindowLongPtr(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		//			SetWindowLongPtr(hwnd, GWL_EXSTYLE, 0);

		//			int width = thisWindowSystem->m_widthInWindowMode;
		//			int height = thisWindowSystem->m_heightInWindowMode;

		//			ShowWindow(hwnd, SW_SHOWNORMAL);
		//			SetWindowPos(hwnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
		//		}
		//		else
		//		{
		//			thisWindowSystem->m_widthInWindowMode = thisWindowSystem->m_width;
		//			thisWindowSystem->m_heightInWindowMode = thisWindowSystem->m_height;
		//			SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP);
		//			SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_TOPMOST);

		//			SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

		//			ShowWindow(hwnd, SW_SHOWMAXIMIZED);
		//		}

		//		thisWindowSystem->m_fullScreen = !thisWindowSystem->m_fullScreen;*/
		//	}
		//	break;
		}
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
//=============================================================================
bool InitWindow(uint32_t width, uint32_t height, std::wstring_view title, CreateWindowFlags flags)
{
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
		keyboardData::state = {};

		mouseData::state = {};
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
Input::MouseState CuteEngineApp::GetMouseState() const
{
	Input::MouseState state;
	memcpy(&state, &mouseData::state, sizeof(Input::MouseState));
	state.positionMode = mouseData::mode;

	DWORD result = WaitForSingleObjectEx(mouseData::scrollWheelValue.get(), 0, FALSE);
	if (result == WAIT_FAILED)
	{
		Fatal("WaitForSingleObjectEx");
		return {};
	}

	if (result == WAIT_OBJECT_0)
	{
		state.scrollWheelValue = 0;
	}

	if (state.positionMode == Input::MouseMode::Relative)
	{
		result = WaitForSingleObjectEx(mouseData::relativeRead.get(), 0, FALSE);

		if (result == WAIT_FAILED)
		{
			Fatal("WaitForSingleObjectEx");
			return {};
		}

		if (result == WAIT_OBJECT_0)
		{
			state.x = state.y = 0;
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

	return state;
}
//=============================================================================
void CuteEngineApp::ResetScrollWheelValue() const
{
	SetEvent(mouseData::scrollWheelValue.get());
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
Input::KeyboardState CuteEngineApp::GetKeyboardState() const
{
	return keyboardData::state;
}
//=============================================================================