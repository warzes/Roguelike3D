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

	bool      isKeyDown[VK_OEM_CLEAR] = {};
	bool      isPreviousKeyDown[VK_OEM_CLEAR] = {};
} // namespace windowData
//=============================================================================
namespace mouseData
{
	void Reset()
	{
		x = y = lastX = lastY = 0;
		relativeX = relativeY = INT32_MAX;
		leftButton = middleButton = rightButton = false;
		xButton1 = xButton2 = false;
		scrollWheelValue = 0;
		inFocus = true;
		autoReset = true;
	}

	Input::MouseMode mode = Input::MouseMode::Absolute;

	int              x{ 0 };
	int              y{ 0 };
	int              lastX{ 0 };
	int              lastY{ 0 };
	int              relativeX{ INT32_MAX };
	int              relativeY{ INT32_MAX };

	bool             leftButton{ false };
	bool             middleButton{ false };
	bool             rightButton{ false };
	bool             xButton1{ false };
	bool             xButton2{ false };
	int              scrollWheelValue{ 0 };

	bool             inFocus{ true };
	bool             autoReset{ true };
} // namespace mouseData
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
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//=============================================================================
void resetKeyBindings()
{
	for (uint32_t i = 0; i < ARRAYSIZE(windowData::isKeyDown); ++i)
		windowData::isKeyDown[i] = false;
}
//=============================================================================
void updateKeyBinding(uint32_t message, uint32_t keyCode)
{
	bool is_down;
	switch (message)
	{
	case WM_KEYUP: case WM_SYSKEYUP:
		is_down = false;
		break;
	case WM_KEYDOWN: case WM_SYSKEYDOWN:
		is_down = true;
		break;
	default:
		return;
	}
	windowData::isKeyDown[keyCode] = is_down;
}
//=============================================================================
void clipToWindow()
{
#error
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

		switch (message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_ACTIVATE:
		case WM_ACTIVATEAPP:
			if (wParam)
			{
				mouseData::inFocus = true;
				if (mouseData::mode == Input::MouseMode::Relative)
				{
					mouseData::x = mouseData::y = 0;
					::ShowCursor(FALSE);
					clipToWindow();
				}
				//game->OnActivated();
			}
			else
			{
				const int scrollWheel = mouseData::scrollWheelValue;
				mouseData::Reset();
				mouseData::scrollWheelValue = scrollWheel;
				if (mouseData::mode == Input::MouseMode::Relative)
				{
					::ClipCursor(nullptr);
				}
				mouseData::inFocus = false;
				//game->OnDeactivated();
			}
			break;

		case WM_KILLFOCUS:
			resetKeyBindings();
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

		case WM_INPUT:
			if (mouseData::inFocus && mouseData::mode == Input::MouseMode::Relative)
			{
				RAWINPUT raw;
				UINT rawSize = sizeof(raw);
				const UINT resultData = GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, &raw, &rawSize, sizeof(RAWINPUTHEADER));
				if (resultData == UINT(-1))
				{
					Fatal("GetRawInputData() failed");
					return 0;
				}

				if (raw.header.dwType == RIM_TYPEMOUSE)
				{
					if (!(raw.data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE))
					{
						mouseData::x += raw.data.mouse.lLastX;
						mouseData::y += raw.data.mouse.lLastY;
					}
					else if (raw.data.mouse.usFlags & MOUSE_VIRTUAL_DESKTOP)
					{
						// This is used to make Remote Desktop sessons work
					}
				}
			}

			break;
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
			if (wParam < ARRAYSIZE(windowData::isKeyDown))
				updateKeyBinding(message, (uint32_t)wParam);

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

	// init mouse
	{
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

	resetKeyBindings();
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
	memcpy(windowData::isPreviousKeyDown, windowData::isKeyDown, sizeof(windowData::isKeyDown)); // TODO: оптимизировать
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
bool CuteEngineApp::IsKeyDown(uint32_t keyCode) const
{
	return (keyCode < ARRAYSIZE(windowData::isKeyDown) ? windowData::isKeyDown[keyCode] : false);
}
//=============================================================================
bool CuteEngineApp::IsKeyPressed(uint32_t keyCode) const
{
	return (keyCode < ARRAYSIZE(windowData::isKeyDown) ? windowData::isKeyDown[keyCode] && !windowData::isPreviousKeyDown[keyCode] : false);
}
//=============================================================================
bool CuteEngineApp::IsKeyReleased(uint32_t keyCode) const
{
	return (keyCode < ARRAYSIZE(windowData::isKeyDown) ? !windowData::isKeyDown[keyCode] && windowData::isPreviousKeyDown[keyCode] : false);
}
//=============================================================================