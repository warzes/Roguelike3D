#include "stdafx.h"
#include "ContextD3D11.h"
#include "Log.h"
//=============================================================================
#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")
#if defined(_DEBUG)
#	pragma comment(lib, "dxguid.lib")
#endif
//=============================================================================
ContextD3D11::~ContextD3D11()
{
	assert(!m_d3dDevice);
}
//=============================================================================
bool ContextD3D11::Create(const ContextD3D11CreateInfo& createInfo)
{
	m_backBufferWidth = std::max(createInfo.width, 1u);
	m_backBufferHeight = std::max(createInfo.height, 1u);
	m_vsync = createInfo.vsync;

	if (!selectAdapter())                  return false;
	if (!createDevice())                   return false;
	if (!createSwapChain(createInfo.hwnd)) return false;
	if (!createRenderTargetView())         return false;

	return true;
}
//=============================================================================
void ContextD3D11::Destroy()
{
	if (m_d3dContext) m_d3dContext->OMSetRenderTargets(0, nullptr, nullptr);
	if (m_swapChain) m_swapChain->SetFullscreenState(FALSE, nullptr);

	m_depthStencilView.Reset();
	m_renderTargetView.Reset();
	m_swapChain.Reset();
	m_d3dContext.Reset();
	m_d3dDevice.Reset();
	m_adapter.Reset();

#if defined(_DEBUG)
	Microsoft::WRL::ComPtr<IDXGIDebug1> debug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug))))
	{
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_SUMMARY | DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
	}
#endif
}
//=============================================================================
bool ContextD3D11::Resize(uint32_t width, uint32_t height)
{
	if (m_backBufferWidth == width && m_backBufferHeight == height) return true;

	m_backBufferWidth = std::max(width, 1u);
	m_backBufferHeight = std::max(height, 1u);

	if (m_d3dContext)
	{
		m_d3dContext->OMSetRenderTargets(0, nullptr, nullptr);
		m_renderTargetView.Reset();
		m_depthStencilView.Reset();
		m_d3dContext->Flush();
	}

	if (m_swapChain)
	{
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		HRESULT result = m_swapChain->GetDesc(&swapChainDesc);
		if (FAILED(result))
		{
			Fatal("IDXGISwapChain1::GetDesc() failed: " + HrToString(result));
			return false;
		}

		result = m_swapChain->ResizeBuffers(m_backBufferCount, m_backBufferWidth, m_backBufferHeight, swapChainDesc.BufferDesc.Format, swapChainDesc.Flags);
		if (FAILED(result))
		{
			Fatal("IDXGISwapChain1::ResizeBuffers() failed: " + HrToString(result));
			return false;
		}
	}

	if (!createRenderTargetView()) return false;

	Debug("FrameBuffer resize: " + std::to_string(m_backBufferWidth) + "x" + std::to_string(m_backBufferHeight));

	return true;
}
//=============================================================================
void ContextD3D11::BeginFrame()
{
	m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), DirectX::Colors::CornflowerBlue);
	m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	D3D11_VIEWPORT viewport = { 0.0f, 0.0f, static_cast<float>(m_backBufferWidth), static_cast<float>(m_backBufferHeight), 0.f, 1.f };
	m_d3dContext->RSSetViewports(1, &viewport);
}
//=============================================================================
void ContextD3D11::EndFrame()
{
	const UINT syncInterval = m_vsync ? 1 : 0;
	const UINT presentFlags = m_supportAllowTearing ? DXGI_PRESENT_ALLOW_TEARING : 0;
	HRESULT result = m_swapChain->Present(syncInterval, presentFlags);
	if (FAILED(result))
	{
		Fatal("IDXGISwapChain1::Present failed: " + HrToString(result));
	}
}
//=============================================================================
bool ContextD3D11::selectAdapter()
{
	UINT dxgiFactoryFlags = 0;
#ifdef _DEBUG
	dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	Microsoft::WRL::ComPtr<IDXGIFactory2> DXGIFactory;
	HRESULT result = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&DXGIFactory));
	if (FAILED(result))
	{
		Fatal("CreateDXGIFactory2() failed: " + HrToString(result));
		return false;
	}
	Microsoft::WRL::ComPtr<IDXGIFactory6> DXGIFactory6;
	result = DXGIFactory.As(&DXGIFactory6);
	if (FAILED(result))
	{
		Fatal("IDXGIFactory As IDXGIFactory6 failed: " + HrToString(result));
		return false;
	}

	Microsoft::WRL::ComPtr<IDXGIAdapter> DXGIAdapter;
	result = DXGIFactory6->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&DXGIAdapter));
	if (FAILED(result))
	{
		Fatal("IDXGIFactory6::EnumAdapterByGpuPreference() failed: " + HrToString(result));
		return false;
	}
	result = DXGIAdapter.As(&m_adapter);
	if (FAILED(result))
	{
		Fatal("DXGIAdapter As DXGIAdapter4 failed: " + HrToString(result));
		return false;
	}

	return true;
}
//=============================================================================
bool ContextD3D11::createDevice()
{
	UINT creationFlags = 0;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	static const D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	HRESULT result = D3D11CreateDevice(m_adapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, nullptr, creationFlags, featureLevels, static_cast<UINT>(std::size(featureLevels)), D3D11_SDK_VERSION, device.ReleaseAndGetAddressOf(), nullptr, context.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		Fatal("D3D11CreateDevice failed: " + HrToString(result));
		return false;
	}

#ifdef _DEBUG
	Microsoft::WRL::ComPtr<ID3D11Debug> d3dDebug;
	if (SUCCEEDED(device.As(&d3dDebug)))
	{
		Microsoft::WRL::ComPtr<ID3D11InfoQueue> d3dInfoQueue;
		if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
		{
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
			D3D11_MESSAGE_ID hide[] =
			{
				D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
				// TODO: Add more message IDs here as needed.
			};
			D3D11_INFO_QUEUE_FILTER filter = {};
			filter.DenyList.NumIDs = static_cast<UINT>(std::size(hide));
			filter.DenyList.pIDList = hide;
			d3dInfoQueue->AddStorageFilterEntries(&filter);
		}
	}
#endif

	if (FAILED(device.As(&m_d3dDevice)))
	{
		Fatal("ID3D11Device as ID3D11Device5 failed");
		return false;
	}
	if (FAILED(context.As(&m_d3dContext)))
	{
		Fatal("ID3D11DeviceContext as ID3D11DeviceContext4 failed");
		return false;
	}

	return true;
}
//=============================================================================
bool ContextD3D11::createSwapChain(HWND hwnd)
{
	Microsoft::WRL::ComPtr<IDXGIFactory2> dxgiFactory;
	HRESULT result = m_adapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf()));
	if (FAILED(result))
	{
		Fatal("IDXGIAdapter::GetParent failed: " + HrToString(result));
		return false;
	}

	m_supportAllowTearing = false;
	if (!m_vsync)
	{
		Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory6;
		if (SUCCEEDED(dxgiFactory.As(&dxgiFactory6)))
		{
			BOOL allowTearing = FALSE;
			if (SUCCEEDED(dxgiFactory6->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing))))
				m_supportAllowTearing = (allowTearing == TRUE);
		}
	}

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = m_backBufferWidth;
	swapChainDesc.Height = m_backBufferHeight;
	swapChainDesc.Format = m_backBufferFormat;
	swapChainDesc.SampleDesc = { 1, 0 };
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = m_backBufferCount;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
	swapChainDesc.Flags = m_supportAllowTearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
	fsSwapChainDesc.Windowed = TRUE;

	result = dxgiFactory->CreateSwapChainForHwnd(m_d3dDevice.Get(), hwnd, &swapChainDesc, &fsSwapChainDesc, nullptr, m_swapChain.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		Fatal("IDXGIFactory2::CreateSwapChainForHwnd failed: " + HrToString(result));
		return false;
	}

	result = dxgiFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);
	if (FAILED(result))
	{
		Fatal("IDXGIFactory2::MakeWindowAssociation failed: " + HrToString(result));
		return false;
	}

	return true;
}
//=============================================================================
bool ContextD3D11::createRenderTargetView()
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	HRESULT result = m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
	if (FAILED(result))
	{
		Fatal("IDXGISwapChain1::GetBuffer failed: " + HrToString(result));
		return false;
	}

	result = m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		Fatal("ID3D11Device5::CreateRenderTargetView failed: " + HrToString(result));
		return false;
	}

	const CD3D11_TEXTURE2D_DESC depthStencilDesc(m_depthBufferFormat, m_backBufferWidth, m_backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencil;
	result = m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf());
	if (FAILED(result))
	{
		Fatal("ID3D11Device5::CreateTexture2D failed: " + HrToString(result));
		return false;
	}

	result = m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), nullptr, m_depthStencilView.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		Fatal("ID3D11Device5::CreateDepthStencilView failed: " + HrToString(result));
		return false;
	}

	return true;
}
//=============================================================================