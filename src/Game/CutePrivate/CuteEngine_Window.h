#pragma once
//=============================================================================
extern CuteEngineApp* thisCuteEngineApp;
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
	bool      requestClose{ true };
	bool      isSizeMove{ false };
	bool      isMinimized{ false };
	bool      fullScreen{ false };
}
//=============================================================================
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//=============================================================================
LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
	if (message == WM_DESTROY) [[unlikely]]
	{
		PostQuitMessage(0);
		return 0;
	}

	//if (ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam))
	//	return true;

	if (thisCuteEngineApp) [[likely]]
	{
		switch (message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
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

		case WM_ENTERSIZEMOVE:
			//thisWindowSystem->m_isSizeMove = true;
			break;
		case WM_EXITSIZEMOVE:
			//thisWindowSystem->m_isSizeMove = false;
			{
				RECT rc;
				GetClientRect(hwnd, &rc);
				//thisWindowSystem->windowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
			}
			break;

		case WM_SIZE:
			if (wParam == SIZE_MINIMIZED)
			{
				//if (!thisWindowSystem->m_isMinimized)
				{
				//	thisWindowSystem->m_isMinimized = true;
				}
			}
			//else if (thisWindowSystem->m_isMinimized)
			{
			//	thisWindowSystem->m_isMinimized = false;
			}
			//else if (!thisWindowSystem->m_isSizeMove)
			{
			//	thisWindowSystem->windowSizeChanged(static_cast<uint32_t>(LOWORD(lParam)), static_cast<uint32_t>(HIWORD(lParam)));
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

		case WM_SYSKEYDOWN:
			if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
			{
				// Implements the classic ALT+ENTER fullscreen toggle
				/*if (thisWindowSystem->m_fullScreen)
				{
					SetWindowLongPtr(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
					SetWindowLongPtr(hwnd, GWL_EXSTYLE, 0);

					int width = thisWindowSystem->m_widthInWindowMode;
					int height = thisWindowSystem->m_heightInWindowMode;

					ShowWindow(hwnd, SW_SHOWNORMAL);
					SetWindowPos(hwnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
				}
				else
				{
					thisWindowSystem->m_widthInWindowMode = thisWindowSystem->m_width;
					thisWindowSystem->m_heightInWindowMode = thisWindowSystem->m_height;
					SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP);
					SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_TOPMOST);

					SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

					ShowWindow(hwnd, SW_SHOWMAXIMIZED);
				}

				thisWindowSystem->m_fullScreen = !thisWindowSystem->m_fullScreen;*/
			}
			break;
		}
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
//=============================================================================
bool InitWindow(uint32_t width, uint32_t height, std::wstring_view title, bool resizable, bool maximize, bool fullScreen)
{
	windowData::requestClose = true;
	windowData::fullScreen = fullScreen;
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

	const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	const int windowLeft = screenWidth / 2 - (int)width / 2;
	const int windowTop = screenHeight / 2 - (int)height / 2;

	RECT windowRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
	LONG windowWidth = windowRect.right - windowRect.left;
	LONG windowHeight = windowRect.bottom - windowRect.top;

	windowData::hwnd = CreateWindowEx(0,
		windowData::windowClassName, title.data(),
		WS_OVERLAPPEDWINDOW,
		windowLeft, windowTop, windowWidth, windowHeight,
		nullptr, nullptr, windowData::handleInstance, nullptr);
	if (!windowData::hwnd)
	{
		Fatal("Failed to create window.");
		return false;
	}

	ShowWindow(windowData::hwnd, maximize ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL);

	GetClientRect(windowData::hwnd, &windowRect);
	windowData::width = static_cast<uint32_t>(windowRect.right - windowRect.left);
	windowData::height = static_cast<uint32_t>(windowRect.bottom - windowRect.top);
	windowData::widthInWindowMode = windowData::width;
	windowData::heightInWindowMode = windowData::height;

	windowData::isSizeMove = false;
	windowData::isMinimized = false;
	windowData::requestClose = false;
	return true;
}
//=============================================================================
void EndWindow()
{
	if (windowData::hwnd) DestroyWindow(windowData::hwnd);
	windowData::hwnd = nullptr;
	if (windowData::handleInstance) UnregisterClass(windowData::windowClassName, windowData::handleInstance);
	windowData::handleInstance = nullptr;
	windowData::requestClose = true;
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
			windowData::requestClose = true;
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