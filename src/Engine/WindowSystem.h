#pragma once

struct WindowSystemCreateInfo final
{
	std::wstring_view title{ L"Game" };
	uint32_t          width{ 1600 };
	uint32_t          height{ 900 };
	bool              maximize{ false };
	bool              resize{ true };
	bool              fullScreen{ false };
};

class WindowSystem final
{
public:
	~WindowSystem();

	[[nodiscard]] 
	bool Create(const WindowSystemCreateInfo& createInfo);
	void Destroy();

	[[nodiscard]] 
	bool IsShouldClose() const;

	void PollEvent();

	uint32_t GetPositionX() const;
	uint32_t GetPositionY() const;

	auto GetWidth() const { assert(m_width); return m_width; }
	auto GetHeight() const { assert(m_height); return m_height; }

	auto GetHWND() const { assert(m_hwnd); return m_hwnd; }
	auto GetWindowInstance() const { assert(m_handleInstance); return m_handleInstance; }

private:
	void windowSizeChanged(uint32_t width, uint32_t height);

	friend LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM) noexcept;

	HINSTANCE    m_handleInstance{ nullptr };
	HWND         m_hwnd{ nullptr };
	MSG          m_msg{};
	uint32_t     m_width{ 1600 };
	uint32_t     m_height{ 900 };
	uint32_t     m_widthInWindowMode{ 1600 };
	uint32_t     m_heightInWindowMode{ 900 };
	bool         m_requestClose{ true };
	bool         m_isSizeMove{ false };
	bool         m_isMinimized{ false };
	bool         m_fullScreen{ false };
};