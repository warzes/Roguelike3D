#pragma once

//=============================================================================
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#if defined(_DEBUG)
#	pragma comment(lib, "dxguid.lib")
#endif
#pragma comment(lib, "d3dcompiler.lib")
//=============================================================================
extern "C"
{
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
//=============================================================================