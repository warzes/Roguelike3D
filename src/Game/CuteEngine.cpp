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

CuteEngineApp* thisCuteEngineApp{ nullptr };
//=============================================================================
namespace engineData
{
	bool shouldClose{ true };
}
//=============================================================================
CuteEngineApp::~CuteEngineApp()
{
	Close();
}
//=============================================================================
CuteEngineApp::Result CuteEngineApp::Init()
{
	auto createInfo = GetCreateInfo();
	auto& windowCI = createInfo.window;
		
	CuteEngineApp::Result result;
	
	result = InitWindow(windowCI.width, windowCI.height, windowCI.title,
		windowCI.resizable, windowCI.fullScreen, windowCI.maximize);
	if (!result) return result;

	thisCuteEngineApp = this;
	engineData::shouldClose = false;
	return { true };
}
//=============================================================================
void CuteEngineApp::Close()
{
	CloseWindow();
	thisCuteEngineApp = nullptr;
	engineData::shouldClose = true;
}
//=============================================================================
bool CuteEngineApp::IsShouldClose() const
{
	return engineData::shouldClose || windowData::requestClose;
}
//=============================================================================
void CuteEngineApp::Frame()
{
	PollEvent();
}
//=============================================================================
void CuteEngineApp::Exit()
{
	engineData::shouldClose = true;
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