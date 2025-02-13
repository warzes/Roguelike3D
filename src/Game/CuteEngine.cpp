#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4820)
#endif
#include "CuteEngine.h"
#include "CutePrivate/CuteEngine_Header.h"
#include "CutePrivate/CuteEngine_Window.h"
#include "CutePrivate/CuteEngine_RHICore.h"
#include "CutePrivate/CuteEngine_RHIResurces.h"
#include "CutePrivate/CuteEngine_RHI.h"
#if defined(_MSC_VER)
#	pragma warning(pop)
#endif
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
void Fatal(const std::string& error)
{
	// TODO: пользовательская функция
	puts(("FATAL: " + error).c_str());
	engineData::shouldClose = true;
}
//=============================================================================
void CuteEngineApp::Run()
{
	if (init())
	{
		while (!isShouldClose())
		{
			computeTimer();

			PollEvent();

			if (windowData::isResized)
			{
				if (!ResizeRHI(windowData::width, windowData::height)) 
					break;

				OnWindowResize(windowData::width, windowData::height);
				windowData::isResized = false;
			}

			update();

			// Фиксированные обновления
			while (engineData::fixedAccumulator >= FIXED_TIME_STEP)
			{
				fixedUpdate();
				engineData::fixedAccumulator -= FIXED_TIME_STEP;
			}

			frame();
		}
	}
	close();
}
//=============================================================================
void CuteEngineApp::Exit()
{
	engineData::shouldClose = true;
}
//=============================================================================
bool CuteEngineApp::init()
{
	auto createInfo = GetCreateInfo();
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

	CreateRHIFlags rhiFlags = 0;
	if (rhiCI.vsync) rhiFlags |= rhi_vsync;

	if (!InitRHI(rhiFlags))
		return false;

	if (!OnInit())
	{
		Fatal("OnInit() return false");
		return false;
	}

	thisCuteEngineApp = this;
	engineData::shouldClose = false;
	return true;
}
//=============================================================================
void CuteEngineApp::close()
{
	OnClose();

	CloseRHI();
	EndWindow();
	thisCuteEngineApp = nullptr;
	engineData::shouldClose = true;
}
//=============================================================================
bool CuteEngineApp::isShouldClose() const
{
	return engineData::shouldClose || windowData::isCloseRequested;
}
//=============================================================================
double CuteEngineApp::getCurrentTime()
{
	LARGE_INTEGER currentTimeLi;
	QueryPerformanceCounter(&currentTimeLi);
	const double currentTime = static_cast<double>(currentTimeLi.QuadPart) * engineData::invFrequency;
	return currentTime;
}
//=============================================================================
void CuteEngineApp::computeTimer()
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
void CuteEngineApp::fixedUpdate()
{
	OnFixedUpdate();
}
//=============================================================================
void CuteEngineApp::update()
{
	OnUpdate(engineData::deltaTime);
}
//=============================================================================
void CuteEngineApp::frame()
{
	OnFrame();
	PresentRHI();
}
//=============================================================================