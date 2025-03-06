#include "stdafx.h"
#include "GameApp.h"
// Demo
#include "001_DemoCube.h"
//=============================================================================
#if defined(_MSC_VER)
#	pragma comment( lib, "3rdparty.lib" )
#	pragma comment( lib, "Engine.lib" )
#endif
//=============================================================================
int main(
	[[maybe_unused]] int   argc,
	[[maybe_unused]] char* argv[])
{
	//GameApp app;

	// Demo
	DemoCube app;

	app.Run();
}
//=============================================================================