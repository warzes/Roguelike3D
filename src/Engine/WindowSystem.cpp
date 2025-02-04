#include "stdafx.h"
#include "WindowSystem.h"
#include "Log.h"
//=============================================================================
namespace
{
	constexpr const auto windowClassName = L"Sapphire Window Class";
	WindowSystem* thisWindowSystem{ nullptr };
}
//=============================================================================
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//=============================================================================
LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
	if (message == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}
	
	if (ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam))
		return true;

	if (thisWindowSystem)
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
			thisWindowSystem->m_isSizeMove = true;
			break;
		case WM_EXITSIZEMOVE:
			thisWindowSystem->m_isSizeMove = false;
			{
				RECT rc;
				GetClientRect(hwnd, &rc);
				thisWindowSystem->windowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
			}
			break;

		case WM_SIZE:
			if (wParam == SIZE_MINIMIZED)
			{
				if (!thisWindowSystem->m_isMinimized)
				{
					thisWindowSystem->m_isMinimized = true;
				}
			}
			else if (thisWindowSystem->m_isMinimized)
			{
				thisWindowSystem->m_isMinimized = false;
			}
			else if (!thisWindowSystem->m_isSizeMove)
			{
				thisWindowSystem->windowSizeChanged(static_cast<uint32_t>(LOWORD(lParam)), static_cast<uint32_t>(HIWORD(lParam)));
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
				if (thisWindowSystem->m_fullScreen)
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

				thisWindowSystem->m_fullScreen = !thisWindowSystem->m_fullScreen;
			}
			break;
		};
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
//=============================================================================
WindowSystem::~WindowSystem()
{
	assert(!thisWindowSystem);
	assert(!m_hwnd);
}
//=============================================================================
bool WindowSystem::Create(const WindowSystemCreateInfo& createInfo)
{
	m_requestClose = true;
	m_fullScreen = createInfo.fullScreen;
	m_handleInstance = GetModuleHandle(nullptr);

	WNDCLASSEX windowClassInfo{ .cbSize = sizeof(WNDCLASSEX) };
	windowClassInfo.style = CS_HREDRAW | CS_VREDRAW;
	windowClassInfo.lpfnWndProc = WindowProc;
	windowClassInfo.hInstance = m_handleInstance;
	windowClassInfo.hIcon = LoadIconW(m_handleInstance, L"IDI_ICON");
	windowClassInfo.hIconSm = LoadIconW(m_handleInstance, L"IDI_ICON");
	windowClassInfo.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClassInfo.hbrBackground = reinterpret_cast<HBRUSH>(5/*COLOR_WINDOW*/ + 1);
	windowClassInfo.lpszClassName = windowClassName;
	if (!RegisterClassEx(&windowClassInfo))
	{
		Fatal("Window class registration failed!");
		return false;
	}

	const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	const int windowLeft = screenWidth / 2 - (int)createInfo.width / 2;
	const int windowTop = screenHeight / 2 - (int)createInfo.height / 2;

	RECT windowRect = { 0, 0, static_cast<LONG>(createInfo.width), static_cast<LONG>(createInfo.height) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
	LONG width = windowRect.right - windowRect.left;
	LONG height = windowRect.bottom - windowRect.top;

	m_hwnd = CreateWindowEx(0,
		windowClassName, createInfo.title.data(),
		WS_OVERLAPPEDWINDOW,
		windowLeft, windowTop, width, height,
		nullptr, nullptr, m_handleInstance, nullptr);
	if (!m_hwnd)
	{
		Fatal("Window creation failed!");
		return false;
	}

	ShowWindow(m_hwnd, createInfo.maximize ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL);

	GetClientRect(m_hwnd, &windowRect);
	m_width = static_cast<uint32_t>(windowRect.right - windowRect.left);
	m_height = static_cast<uint32_t>(windowRect.bottom - windowRect.top);
	m_widthInWindowMode = m_width;
	m_heightInWindowMode = m_height;

	thisWindowSystem = this;
	m_requestClose = false;
	m_isSizeMove = false;
	m_isMinimized = false;
	return true;
}
//=============================================================================
void WindowSystem::Destroy()
{
	if (m_hwnd) DestroyWindow(m_hwnd);
	m_hwnd = nullptr;
	if (m_handleInstance) UnregisterClass(windowClassName, m_handleInstance);
	m_handleInstance = nullptr;
	thisWindowSystem = nullptr;
}
//=============================================================================
bool WindowSystem::IsShouldClose() const
{
	return m_requestClose;
}
//=============================================================================
void WindowSystem::PollEvent()
{
	while (PeekMessage(&m_msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&m_msg);
		DispatchMessage(&m_msg);

		if (m_msg.message == WM_QUIT)
		{
			m_requestClose = true;
			break;
		}
	}
}
//=============================================================================
uint32_t WindowSystem::GetPositionX() const
{
	RECT rect{};
	GetClientRect(m_hwnd, &rect);
	ClientToScreen(m_hwnd, (LPPOINT)&rect.left);
	ClientToScreen(m_hwnd, (LPPOINT)&rect.right);
	return rect.left;
}
//=============================================================================
uint32_t WindowSystem::GetPositionY() const
{
	RECT rect{};
	GetClientRect(m_hwnd, &rect);
	ClientToScreen(m_hwnd, (LPPOINT)&rect.left);
	ClientToScreen(m_hwnd, (LPPOINT)&rect.right);
	return rect.top;
}
//=============================================================================
void WindowSystem::windowSizeChanged(uint32_t width, uint32_t height)
{
	m_width = width;
	m_height = height;
	// TODO: dpi?
}
//=============================================================================