#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4820)
#endif
#include "CuteEngine.h"
#include "CutePrivate/CuteEngine_Header.h"
#include "CutePrivate/CuteEngine_Window.h"
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
	double accumulator = 0.0;
	double invFrequency = 0.0;
	int frameCount = 0;
	double fpsTimer = 0.0;
	int fps = 0;
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

			// Фиксированные обновления
			while (engineData::accumulator >= FIXED_TIME_STEP)
			{
				fixedUpdate();
				engineData::accumulator -= FIXED_TIME_STEP;
			}

			update();

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
	engineData::deltaTime = 0.0;
	engineData::accumulator = 0.0;
	engineData::invFrequency = 0.0;
	engineData::frameCount = 0;
	engineData::fpsTimer = 0.0;
	engineData::fps = 0;

	auto createInfo = GetCreateInfo();
	auto& windowCI = createInfo.window;

	if (!InitWindow(windowCI.width, windowCI.height, windowCI.title,
		windowCI.resizable, windowCI.fullScreen, windowCI.maximize))
		return false;

	if (!OnInit())
	{
		Fatal("OnInit() return false");
		return false;
	}

	// Получение текущего времени
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	engineData::invFrequency = 1.0 / frequency.QuadPart;
	engineData::lastTime = engineData::currentTime = getCurrentTime();

	thisCuteEngineApp = this;
	engineData::shouldClose = false;
	return true;
}
//=============================================================================
void CuteEngineApp::close()
{
	OnClose();

	EndWindow();
	thisCuteEngineApp = nullptr;
	engineData::shouldClose = true;
}
//=============================================================================
bool CuteEngineApp::isShouldClose() const
{
	return engineData::shouldClose || windowData::requestClose;
}
//=============================================================================
double CuteEngineApp::getCurrentTime()
{
	LARGE_INTEGER currentTimeLi;
	QueryPerformanceCounter(&currentTimeLi);
	double currentTime = currentTimeLi.QuadPart * engineData::invFrequency;
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
	engineData::accumulator += engineData::deltaTime;
	engineData::fpsTimer += engineData::deltaTime;
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
}
//=============================================================================