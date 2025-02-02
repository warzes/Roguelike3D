#pragma once

#include "CoreD3D11.h"

struct ContextD3D11CreateInfo final
{
	HWND     hwnd{ nullptr };
	uint32_t width{ 1 };
	uint32_t height{ 1 };

	bool     vsync{ false };
};

class ContextD3D11 final
{
public:
	~ContextD3D11();

	[[nodiscard]]
	bool Create(const ContextD3D11CreateInfo& createInfo);
	void Destroy();

	bool Resize(uint32_t width, uint32_t height);

	void BeginFrame();
	void EndFrame();

private:
	bool selectAdapter();
	bool createDevice();
	bool createSwapChain(HWND hwnd);
	bool createRenderTargetView();

	Microsoft::WRL::ComPtr<IDXGIAdapter4>          m_adapter;
	Microsoft::WRL::ComPtr<ID3D11Device5>          m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext4>   m_d3dContext;

	Microsoft::WRL::ComPtr<IDXGISwapChain1>        m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	uint32_t                                       m_backBufferWidth{ 0 };
	uint32_t                                       m_backBufferHeight{ 0 };
	const DXGI_FORMAT                              m_backBufferFormat{ DXGI_FORMAT_B8G8R8A8_UNORM };
	const DXGI_FORMAT                              m_depthBufferFormat{ DXGI_FORMAT_D24_UNORM_S8_UINT };
	const UINT                                     m_backBufferCount{ 2 };
	bool                                           m_vsync{ false };
	bool                                           m_supportAllowTearing{ false };
};