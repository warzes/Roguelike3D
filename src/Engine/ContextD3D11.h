#pragma once

#include "CoreD3D11.h"

struct ContextD3D11CreateInfo final
{
	HWND     hwnd{ nullptr };
	uint32_t width{ 0 };
	uint32_t height{ 0 };

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

	auto GetD3DAdapter() const { return m_adapter; }

	auto GetD3DDevice() const { return m_d3dDevice; }
	auto GetD3DDevicePtr() const { return m_d3dDevice.Get(); }

	auto GetD3DContext() const { return m_d3dContext; }
	auto GetD3DContextPtr() const { return m_d3dContext.Get(); }


private:
	bool setBackBufferSize(uint32_t width, uint32_t height);
	bool selectAdapter();
	bool createDevice();
	bool createSwapChain(HWND hwnd);
	bool createRenderTargetView();
	void setColorSpace1();

	Microsoft::WRL::ComPtr<IDXGIAdapter4>          m_adapter;
	Microsoft::WRL::ComPtr<ID3D11Device5>          m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext4>   m_d3dContext;

	SwapChainFormat                                m_swapChainFormat{ SwapChainFormat::BT709_G22_8BIT };
	Microsoft::WRL::ComPtr<IDXGISwapChain4>        m_swapChain;
	DXGI_FORMAT                                    m_backBufferFormat{ };
	DXGI_COLOR_SPACE_TYPE                          m_colorSpace{};
	uint32_t                                       m_backBufferWidth{ 0 };
	uint32_t                                       m_backBufferHeight{ 0 };
	D3D11_VIEWPORT                                 m_viewport{};

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	const DXGI_FORMAT                              m_depthBufferFormat{ DXGI_FORMAT_D24_UNORM_S8_UINT };
	const UINT                                     m_backBufferCount{ 2 };
	bool                                           m_vsync{ false };
	bool                                           m_supportAllowTearing{ false };
};