﻿#include "stdafx.h"
//=============================================================================
#if defined(_MSC_VER)
#	pragma comment( lib, "3rdparty.lib" )
#	pragma comment( lib, "Engine.lib" )
#endif

//extern "C"
//{
//	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
//	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
//}
//=============================================================================
void RequestExit() {}
//=============================================================================
int main()
{
	if (!DirectX::XMVerifyCPUSupport())
		return 1;

	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
		return 1;

	LogSystemCreateInfo logCI;
	LogSystem log;
	log.Create(logCI);

	WindowSystemCreateInfo windowCI;
	WindowSystem window;
	window.Create(windowCI);

	while (!window.IsShouldClose())
	{
		window.PollEvent();
	}

	window.Destroy();
	log.Destroy();
}
//=============================================================================S