﻿#pragma once

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
- Создание и управление окном
- Система Direct3D11
*/

#pragma endregion
//=============================================================================
#pragma region [ Config Engine ]

#pragma endregion
//=============================================================================
#pragma region [ Constant Engine ]

constexpr const auto CuteEngineVersion = "0.0.1";

#pragma endregion
//=============================================================================
#pragma region [ Header ]

#define _USE_MATH_DEFINES

#include <cstdint>
#include <optional>
#include <expected>
#include <memory>
#include <string_view>
#include <string>
#include <vector>

#include "CuteRHI.h"

#pragma endregion
//=============================================================================
#pragma region [ Base ]

void Print(const std::string& message);
void Print(const std::wstring& message);
void Fatal(const std::string& error);

#pragma endregion
//=============================================================================
#pragma region [ Input ]

namespace Input
{
	struct MousePosition final
	{
		int x{ 0 };
		int y{ 0 };
	};

	enum class MouseMode : uint8_t
	{
		Absolute,
		Relative
	};

	enum class MouseButton : uint8_t
	{
		Left,
		Right,
		Middle,
		xButton1,
		xButton2
	};

	enum Key
	{
		UNKNOWN = -1,

		KEY_SPACE = 32,
		KEY_APOSTROPHE = 39,
		KEY_COMMA = 44,
		KEY_MINUS = 45,
		KEY_PERIOD = 46,
		KEY_SLASH = 47,
		KEY_ZERO = 48,
		KEY_1 = 49,
		KEY_2 = 50,
		KEY_3 = 51,
		KEY_4 = 52,
		KEY_5 = 53,
		KEY_6 = 54,
		KEY_7 = 55,
		KEY_8 = 56,
		KEY_9 = 57,
		KEY_SEMICOLON = 59,
		KEY_EQUAL = 61,
		KEY_A = 65,
		KEY_B = 66,
		KEY_C = 67,
		KEY_D = 68,
		KEY_E = 69,
		KEY_F = 70,
		KEY_G = 71,
		KEY_H = 72,
		KEY_I = 73,
		KEY_J = 74,
		KEY_K = 75,
		KEY_L = 76,
		KEY_M = 77,
		KEY_N = 78,
		KEY_O = 79,
		KEY_P = 80,
		KEY_Q = 81,
		KEY_R = 82,
		KEY_S = 83,
		KEY_T = 84,
		KEY_U = 85,
		KEY_V = 86,
		KEY_W = 87,
		KEY_X = 88,
		KEY_Y = 89,
		KEY_Z = 90,
		KEY_LEFT_BRACKET = 91,
		KEY_BACKSLASH = 92,
		KEY_RIGHT_BRACKET = 93,
		KEY_GRAVE_ACCENT = 96,
		KEY_WORLD_1 = 161,
		KEY_WORLD_2 = 162,

		KEY_ESCAPE = 256,
		KEY_ENTER = 257,
		KEY_TAB = 258,
		KEY_BACKSPACE = 259,
		KEY_INSERT = 260,
		KEY_DELETE = 261,
		KEY_RIGHT = 262,
		KEY_LEFT = 263,
		KEY_DOWN = 264,
		KEY_UP = 265,
		KEY_PAGE_UP = 266,
		KEY_PAGE_DOW = 267,
		KEY_HOME = 268,
		KEY_END = 269,
		KEY_CAPS_LOCK = 280,
		KEY_SCROLL_LOCK = 281,
		KEY_NUM_LOCK = 282,
		KEY_PRINT_SCREEN = 283,
		KEY_PAUSE = 284,

		KEY_F1 = 290,
		KEY_F2 = 291,
		KEY_F3 = 292,
		KEY_F4 = 293,
		KEY_F5 = 294,
		KEY_F6 = 295,
		KEY_F7 = 296,
		KEY_F8 = 297,
		KEY_F9 = 298,
		KEY_F10 = 299,
		KEY_F11 = 300,
		KEY_F12 = 301,
		KEY_F13 = 302,
		KEY_F14 = 303,
		KEY_F15 = 304,
		KEY_F16 = 305,
		KEY_F17 = 306,
		KEY_F18 = 307,
		KEY_F19 = 308,
		KEY_F20 = 309,
		KEY_F21 = 310,
		KEY_F22 = 311,
		KEY_F23 = 312,
		KEY_F24 = 313,
		KEY_F25 = 314,

		KEY_KP_0 = 320,
		KEY_KP_1 = 321,
		KEY_KP_2 = 322,
		KEY_KP_3 = 323,
		KEY_KP_4 = 324,
		KEY_KP_5 = 325,
		KEY_KP_6 = 326,
		KEY_KP_7 = 327,
		KEY_KP_8 = 328,
		KEY_KP_9 = 329,
		KEY_KP_DECIMAL = 330,
		KEY_KP_DIVIDE = 331,
		KEY_KP_MULTIPLY = 332,
		KEY_KP_SUBTRACT = 333,
		KEY_KP_ADD = 334,
		KEY_KP_ENTER = 335,
		KEY_KP_EQUAL = 336,

		KEY_LEFT_SHIFT = 340,
		KEY_LEFT_CONTROL = 341,
		KEY_LEFT_ALT = 342,
		KEY_LEFT_SUPER = 343,
		KEY_RIGHT_SHIFT = 344,
		KEY_RIGHT_CONTROL = 345,
		KEY_RIGHT_ALT = 346,
		KEY_RIGHT_SUPER = 347,
		KEY_MENU = 348,

		KEYS_MAX = 349
	};

} // namespace Input

#pragma endregion
//=============================================================================
#pragma region [ Engine App ]

struct CuteEngineCreateInfo final
{
	struct Window
	{
		uint32_t          width{ 1600 };
		uint32_t          height{ 900 };
		std::wstring_view title{ L"Game" };
		bool              resizable{ true };
		bool              maximize{ false };
		bool              fullScreen{ false };
	} window;

	struct RHI
	{
		bool              vsync{ false };
		bool              enableImGui{ true };
	} rhi;
};

class ICuteEngineApp
{
public:
	virtual ~ICuteEngineApp() = default;

	virtual CuteEngineCreateInfo GetCreateInfo() { return {}; }
	virtual bool OnInit() { return true; }
	virtual void OnClose() {}

	virtual void OnUpdate([[maybe_unused]] double deltaTime) {}
	virtual void OnFixedUpdate() {}
	virtual void OnFrame() {}

	// Window Events
	virtual void OnWindowResize(
		[[maybe_unused]] uint32_t width, 
		[[maybe_unused]] uint32_t height) {}

	// Mouse Events
	virtual bool OnMouseMove(
		[[maybe_unused]] int x, 
		[[maybe_unused]] int y, 
		[[maybe_unused]] int deltaX, 
		[[maybe_unused]] int deltaY) { return true; }

	virtual bool OnMouseButton(
		[[maybe_unused]] int x, 
		[[maybe_unused]] int y, 
		[[maybe_unused]] Input::MouseButton button,
		[[maybe_unused]] bool pressed) { return true; }
		
	virtual bool OnMouseWheel([[maybe_unused]] int scroll) { return true; }

	// Keyboard Events
	virtual bool OnKey(
		[[maybe_unused]] uint32_t key, 
		[[maybe_unused]] bool pressed) { return true; }
};

class CuteEngineApp : protected ICuteEngineApp
{
public:
	void Run();

	void Exit();

	// Window
	void SetWindowTitle(std::wstring_view title);
	void RegisterDropCallback(void (*callback)(char const*, uint32_t, void*), void* data);
	void UnregisterDropCallback();

	[[nodiscard]] uint32_t GetWindowWidth() const;
	[[nodiscard]] uint32_t GetWindowHeight() const;
	[[nodiscard]] float    GetWindowAspect() const;

	// Input
	bool IsMouseDown(Input::MouseButton button) const;
	bool IsMousePressed(Input::MouseButton button) const;
	bool IsMouseReleased(Input::MouseButton button) const;
	Input::MousePosition GetMousePosition() const;
	void SetMouseMode(Input::MouseMode mode) const;
	int GetScrollWheelValue() const;
	void ResetScrollWheelValue() const;
	bool IsMouseVisible() const;
	void SetMouseVisible(bool visible) const;

	bool IsKeyDown(uint32_t keyCode) const;
	bool IsKeyPressed(uint32_t keyCode) const;
	bool IsKeyReleased(uint32_t keyCode) const;
};

#pragma endregion
//=============================================================================
#ifdef CUTE_IMPLEMENTATION_DEFINE
#	include "CuteEngine.cpp"
#endif
//=============================================================================