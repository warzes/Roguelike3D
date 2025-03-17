#include "stdafx.h"
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

//план:
//https://vk.com/game_darkandlight
//https://github.com/SDraw/roc_engine/tree/master/roc_app
//https://www.youtube.com/@farhananjumrahman3463
//https://github.com/meemknight/gl3d
//3Dev
//
//	- загрузка меша и возможно анимации
//		материалы - (pomdog)
//		источники света из растертек?
//		если эффекты - то книга realtime rendering
//		https ://www.geometrictools.com/
//		
//	
//	в невервинтере мир был разделен на тайлы (тайлсеты). возможно тоже такое использовать. посмотреть редактор
//	
//	Dread Delusion для идей
//
//на скринах из папки	
//
//в pomdog есть материал

	// Demo
	//DemoCube app;
	//TexturedCube app;
	//MeshDemo app;
	ModelDemo app;
	app.Run();
}
//=============================================================================