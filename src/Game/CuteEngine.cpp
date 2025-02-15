#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4820)
#endif
//=============================================================================
#include "CuteEngine.h"
#include "CutePrivate/CuteEngine_Header.h"
#if defined(_DEBUG)
#	ifndef DBG_NEW
#		define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#		define new DBG_NEW
#	endif
#endif  // _DEBUG
#include "CutePrivate/CuteEngine_Window.h"
#include "CutePrivate/CuteEngine_RHICore.h"
#include "CutePrivate/CuteEngine_RHI.h"
#include "CutePrivate/CuteEngine_RHIResources.h"
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
bool init(const CuteEngineCreateInfo& createInfo);
bool initImGui();
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
					if (!ResizeRHI(windowData::width, windowData::height))
						break;

					OnWindowResize(windowData::width, windowData::height);
					windowData::isResized = false;
				}

				// Update
				{
					// Start the Dear ImGui frame
					ImGui_ImplDX11_NewFrame();
					ImGui_ImplWin32_NewFrame();
					ImGui::NewFrame();

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
					bool show_demo_window = true;
					if (show_demo_window)
						ImGui::ShowDemoWindow(&show_demo_window);

					ImGui::Begin("Hello, world!");
					ImGui::Text("This is some useful text.");
					ImGui::SameLine();
					ImGui::Text("2This is some useful text.");
					ImGui::End();

					ImGui::Render();

					rhiData::d3dContext->ClearRenderTargetView(rhiData::renderTargetView.Get(), DirectX::Colors::CornflowerBlue);
					rhiData::d3dContext->ClearDepthStencilView(rhiData::depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

					rhiData::d3dContext->OMSetRenderTargets(1, rhiData::renderTargetView.GetAddressOf(), rhiData::depthStencilView.Get());
					rhiData::d3dContext->RSSetViewports(1, &rhiData::viewport);

					OnFrame();

					ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
					PresentRHI();
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

	if (!DirectX::XMVerifyCPUSupport())
		return false;

	if (FAILED(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED)))
		return false;

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

	if (!initImGui())
	{
		Fatal("initImGui() return false");
		return false;
	}

	engineData::shouldClose = false;
	return true;
}
//=============================================================================
bool initImGui()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	if (!ImGui_ImplWin32_Init(windowData::hwnd))
		return false;
	if (!ImGui_ImplDX11_Init(rhiData::d3dDevice.Get(), rhiData::d3dContext.Get()))
		return false;

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != nullptr);

	return true;
}
//=============================================================================
void close()
{
	if (ImGui::GetCurrentContext() != nullptr && ImGui::GetIO().BackendPlatformUserData != nullptr)
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	CloseRHI();
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