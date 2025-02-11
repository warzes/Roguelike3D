#pragma once

//=============================================================================
#pragma region [ DOC ]

/*
DOC:
...
*/

/*
TODO:
...
*/

/*
HISTORY:

Version 0.0.1
- Основные файлы
- Главный класс приложения CuteEngineApp
*/

#pragma endregion
//=============================================================================
#pragma region [ Config Engine ]

#pragma endregion
//=============================================================================
#pragma region [ Constant Engine ]

constexpr const auto CuteEngineVersion = "0.0.1";

#pragma endregion




#include <cstdint>

class ICuteEngineApp
{
public:
	virtual ~ICuteEngineApp() = default;

	virtual bool OnInit() { return true; }
	virtual bool OnClose() { return true; }

	virtual void OnUpdate([[maybe_unused]] float deltaTime) {}
	virtual void OnFixedUpdate() {}
	virtual void OnFrame() {}

	virtual void OnWindowResize([[maybe_unused]] uint32_t width, [[maybe_unused]] uint32_t height) {}

	virtual bool OnMouseMove(int x, const int y, int deltaX, int deltaY) { return true; }
	virtual bool OnMouseButton(const int x, const int y, const MouseButton button, const bool pressed);
	virtual bool OnMouseWheel(const int x, const int y, const int scroll);
	virtual bool OnKey(const uint key, const bool pressed);
	virtual bool OnJoystickAxis(const int axis, const float value);
	virtual bool OnJoystickButton(const int button, const bool pressed);
	virtual void OnSize(const int w, const int h);
	virtual void OnClose();

};

class CuteEngineApp : protected ICuteEngineApp
{
public:
	~CuteEngineApp() override;

	void Run();
};