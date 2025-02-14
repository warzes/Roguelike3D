#include "stdafx.h"
#include "GameApp.h"
//=============================================================================
#if defined(_MSC_VER)
#	pragma comment( lib, "3rdparty.lib" )
#	pragma comment( lib, "Engine.lib" )
#endif
//=============================================================================

#include <iostream>

struct Tesss
{
	Tesss() { std::cout << "Tessss"; }
	Tesss(const Tesss&) { std::cout << "Tessss(&)"; }
	Tesss(Tesss&&) { std::cout << "Tessss(&&)"; }
	~Tesss() { std::cout << "~Tessss"; }
};


std::expected<Tesss, std::string> Foo()
{
	//return {};

	return std::unexpected("dfd");
}

int main(
	[[maybe_unused]] int   argc,
	[[maybe_unused]] char* argv[])
{
	//auto foo = Foo();
	//if (!foo.has_value())
	//{
	//	std::cout << foo.error();
	//}
	GameApp app;
	app.Run();
}
//=============================================================================