#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4820)
#endif
//=============================================================================
#pragma region [ Header ]
#include "CuteEngine.h"

#if defined(_MSC_VER)
#	pragma warning(push, 3)
#	pragma warning(disable : 4061)
#	pragma warning(disable : 4365)
#	pragma warning(disable : 4668)
#	pragma warning(disable : 5039)
#endif

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define NOBITMAP
#define NOGDICAPMASKS
//#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCTLMGR
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
#define NOGDI
#define NOWINSTYLE
#define NOKEYSTATES
#define NOCOLOR
#define NOMB
//#define NOWINOFFSETS
#define NOCRYPT
#define NOIME

#include <winsdkver.h>
#define _WIN32_WINNT 0x0A00 // Windows 10
#include <sdkddkver.h>
#include <Windows.h>
#include <shellapi.h>

#include <imgui/imgui.h>

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif
#if defined(_DEBUG)
#	ifndef DBG_NEW
#		define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#		define new DBG_NEW
#	endif
#endif  // _DEBUG
#if defined(_MSC_VER)
#	pragma warning(pop)
#endif
#pragma endregion
//=============================================================================
#pragma region [ Window ]
#include "CutePrivate/CuteEngine_Window.h"
#pragma endregion
//=============================================================================
CuteEngineApp* thisCuteEngineApp{ nullptr };
constexpr int TARGET_FPS = 60;
constexpr double FIXED_TIME_STEP = 1.0 / TARGET_FPS;
//=============================================================================
namespace engineData
{
	bool shouldClose{ true };

	double lastTime = 0.0;
	double currentTime = 0.0;
	double deltaTime = 0.0;
	double fixedAccumulator = 0.0;
	double invFrequency = 0.0;
}
//=============================================================================
bool init(const CuteEngineCreateInfo& createInfo);
void close();
bool isShouldClose();
double getCurrentTime();
void computeTimer();
//=============================================================================
void Print(const std::string& message)
{
	// TODO: пользовательская функция
	puts(message.c_str());
}
//=============================================================================
void Print(const std::wstring& message)
{
	// TODO: пользовательская функция
	_putws(message.c_str());
}
//=============================================================================
void Fatal(const std::string& error)
{
	// TODO: пользовательская функция
	puts(("FATAL: " + error).c_str());
	engineData::shouldClose = true;
}
//=============================================================================
void CuteEngineApp::Run()
{
	if (init(GetCreateInfo()))
	{
		if (OnInit())
		{
			thisCuteEngineApp = this;

			while (!isShouldClose())
			{
				computeTimer();
				PollEvent();

				// Resize
				if (windowData::isResized)
				{
					if (!rhi::Resize(windowData::width, windowData::height))
						break;

					OnWindowResize(windowData::width, windowData::height);
					windowData::isResized = false;
				}

				// Update
				{
					OnUpdate(engineData::deltaTime);
				}

				// Fixed Update
				while (engineData::fixedAccumulator >= FIXED_TIME_STEP)
				{
					OnFixedUpdate();
					engineData::fixedAccumulator -= FIXED_TIME_STEP;
				}

				// Frame
				{
					rhi::BeginFrame();	
					OnFrame();
					rhi::EndFrame();
				}
			}
		}
		else
		{
			Fatal("OnInit() return false");
		}
	}
	OnClose();
	close();
}
//=============================================================================
void CuteEngineApp::Exit()
{
	engineData::shouldClose = true;
}
//=============================================================================
bool init(const CuteEngineCreateInfo& createInfo)
{
	auto& windowCI = createInfo.window;
	auto& rhiCI = createInfo.rhi;

	LARGE_INTEGER frequency;
	if (!QueryPerformanceFrequency(&frequency))
	{
		Fatal("High-resolution performance counter not supported!");
		return false;
	}
	if (frequency.QuadPart == 0)
	{
		Fatal("Performance frequency is zero!");
		return false;
	}
	engineData::invFrequency     = 1.0 / static_cast<double>(frequency.QuadPart);
	engineData::lastTime         = engineData::currentTime = getCurrentTime();
	engineData::deltaTime        = 0.0;
	engineData::fixedAccumulator = 0.0;

	CreateWindowFlags windowFlags = 0;
	if (!windowCI.resizable) windowFlags |= NoResizeWindow;
	if (windowCI.fullScreen) windowFlags |= FullscreenWindow;
	if (windowCI.maximize)   windowFlags |= MaximizeWindow;

	if (!InitWindow(windowCI.width, windowCI.height, windowCI.title, windowFlags))
		return false;

	if (!rhi::Setup(windowData::hwnd, windowData::width, windowData::height, rhiCI.vsync, rhiCI.enableImGui))
		return false;

	engineData::shouldClose = false;
	return true;
}
//=============================================================================
void close()
{
	rhi::Shutdown();
	EndWindow();
	thisCuteEngineApp = nullptr;
	engineData::shouldClose = true;
}
//=============================================================================
bool isShouldClose()
{
	return engineData::shouldClose || windowData::isCloseRequested;
}
//=============================================================================
double getCurrentTime()
{
	LARGE_INTEGER currentTimeLi;
	QueryPerformanceCounter(&currentTimeLi);
	const double currentTime = static_cast<double>(currentTimeLi.QuadPart) * engineData::invFrequency;
	return currentTime;
}
//=============================================================================
void computeTimer()
{
	// Получение текущего времени
	engineData::currentTime = getCurrentTime();

	// Расчёт deltaTime
	engineData::deltaTime = engineData::currentTime - engineData::lastTime;
	engineData::lastTime = engineData::currentTime;
	// Аккумулирование времени
	engineData::fixedAccumulator += engineData::deltaTime;
}
//=============================================================================