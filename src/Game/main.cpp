﻿#include "stdafx.h"
#include "GameApp.h"
// Demo
#include "001_DemoCube.h"
#include "002_TexturedCube.h"
#include "003_Mesh.h"
#include "004_Model.h"
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
	
	в невервинтере мир был разделен на тайлы (тайлсеты). возможно тоже такое использовать. посмотреть редактор

	// Demo
	//DemoCube app;
	//TexturedCube app;
	//MeshDemo app;
	ModelDemo app;
	app.Run();
}
//=============================================================================