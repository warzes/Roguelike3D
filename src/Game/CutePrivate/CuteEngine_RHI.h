#pragma once

//=============================================================================
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#if defined(_DEBUG)
#	pragma comment(lib, "dxguid.lib")
#endif
#pragma comment(lib, "d3dcompiler.lib")
//=============================================================================
extern "C"
{
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
//=============================================================================
namespace rhiData
{
	using Microsoft::WRL::ComPtr;

	ComPtr<IDXGIAdapter4>          adapter;
	ComPtr<ID3D11Device5>          d3dDevice;
	ComPtr<ID3D11DeviceContext4>   d3dContext;

	ComPtr<IDXGISwapChain4>        swapChain;
	DXGI_FORMAT                    backBufferFormat{ };
	DXGI_COLOR_SPACE_TYPE          colorSpace{};
	uint32_t                       backBufferWidth{ 0 };
	uint32_t                       backBufferHeight{ 0 };
	D3D11_VIEWPORT                 viewport{};

	ComPtr<ID3D11RenderTargetView> renderTargetView;
	ComPtr<ID3D11DepthStencilView> depthStencilView;
	const DXGI_FORMAT              depthBufferFormat{ DXGI_FORMAT_D24_UNORM_S8_UINT };
	const UINT                     backBufferCount{ 2 };
	bool                           vsync{ false };
	bool                           supportAllowTearing{ false };
} // namespace rhiData
//=============================================================================
bool setBackBufferSize(uint32_t width, uint32_t height);
bool selectAdapter();
bool createDevice();
bool createSwapChain(HWND hwnd);
bool createRenderTargetView();
void setColorSpace1();
//=============================================================================
bool InitRHI(CreateRHIFlags flags)
{
	setBackBufferSize(windowData::width, windowData::height);
	rhiData::vsync = (flags & rhi_vsync) != 0;

	if (!selectAdapter())                   return false;
	if (!createDevice())                    return false;
	if (!createSwapChain(windowData::hwnd)) return false;
	if (!createRenderTargetView())          return false;

	return true;
}
//=============================================================================
void CloseRHI()
{
	using namespace rhiData;

	if (d3dContext) d3dContext->OMSetRenderTargets(0, nullptr, nullptr);
	if (swapChain) swapChain->SetFullscreenState(FALSE, nullptr);

	depthStencilView.Reset();
	renderTargetView.Reset();
	swapChain.Reset();
	d3dContext.Reset();
	d3dDevice.Reset();
	adapter.Reset();

#if defined(_DEBUG)
	ComPtr<IDXGIDebug1> debug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug))))
	{
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_SUMMARY | DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
	}
#endif
}
//=============================================================================
void PresentRHI()
{
	using namespace rhiData;

	const UINT syncInterval = vsync ? 1 : 0;
	const UINT presentFlags = supportAllowTearing ? DXGI_PRESENT_ALLOW_TEARING : 0;
	HRESULT result = swapChain->Present(syncInterval, presentFlags);
	if (FAILED(result))
	{
		DX_ERR("IDXGISwapChain1::Present failed: ", result);
	}
}
//=============================================================================
bool ResizeRHI(uint32_t width, uint32_t height)
{
	using namespace rhiData;

	if (!setBackBufferSize(width, height)) return true;

	if (d3dContext)
	{
		d3dContext->OMSetRenderTargets(0, nullptr, nullptr);
		renderTargetView.Reset();
		depthStencilView.Reset();
		d3dContext->Flush();
	}

	if (swapChain)
	{
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		HRESULT result = swapChain->GetDesc(&swapChainDesc);
		if (FAILED(result))
		{
			DX_ERR("IDXGISwapChain1::GetDesc() failed: ", result);
			return false;
		}

		result = swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, swapChainDesc.BufferDesc.Format, swapChainDesc.Flags);
		if (FAILED(result))
		{
			DX_ERR("IDXGISwapChain1::ResizeBuffers() failed: ", result);
			return false;
		}
		setColorSpace1();
	}

	if (!createRenderTargetView()) return false;

	return true;
}
//=============================================================================
bool setBackBufferSize(uint32_t width, uint32_t height)
{
	using namespace rhiData;

	if (backBufferWidth == width && backBufferHeight == height) return false;

	backBufferWidth = std::max(width, 1u);
	backBufferHeight = std::max(height, 1u);
	viewport = { 0.0f, 0.0f, static_cast<float>(backBufferWidth), static_cast<float>(backBufferHeight), 0.f, 1.f };

	return true;
}
//=============================================================================
bool selectAdapter()
{
	using namespace rhiData;

	UINT dxgiFactoryFlags = 0;
#ifdef _DEBUG
	dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	ComPtr<IDXGIFactory2> DXGIFactory;
	HRESULT result = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&DXGIFactory));
	if (FAILED(result))
	{
		DX_ERR("CreateDXGIFactory2() failed: ", result);
		return false;
	}
	ComPtr<IDXGIFactory6> DXGIFactory6;
	result = DXGIFactory.As(&DXGIFactory6);
	if (FAILED(result))
	{
		DX_ERR("IDXGIFactory As IDXGIFactory6 failed: ", result);
		return false;
	}

	ComPtr<IDXGIAdapter> DXGIAdapter;
	result = DXGIFactory6->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&DXGIAdapter));
	if (FAILED(result))
	{
		DX_ERR("IDXGIFactory6::EnumAdapterByGpuPreference() failed: ", result);
		return false;
	}
	result = DXGIAdapter.As(&adapter);
	if (FAILED(result))
	{
		DX_ERR("DXGIAdapter As DXGIAdapter4 failed: ", result);
		return false;
	}

	return true;
}
//=============================================================================
bool createDevice()
{
	using namespace rhiData;

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
	HRESULT result = D3D11CreateDevice(adapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, nullptr, creationFlags, featureLevels, static_cast<UINT>(std::size(featureLevels)), D3D11_SDK_VERSION, device.ReleaseAndGetAddressOf(), nullptr, context.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		DX_ERR("D3D11CreateDevice failed: ", result);
		return false;
	}

#ifdef _DEBUG
	ComPtr<ID3D11Debug> d3dDebug;
	if (SUCCEEDED(device.As(&d3dDebug)))
	{
		ComPtr<ID3D11InfoQueue> d3dInfoQueue;
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

	if (FAILED(device.As(&d3dDevice)))
	{
		Fatal("ID3D11Device as ID3D11Device5 failed");
		return false;
	}
	if (FAILED(context.As(&d3dContext)))
	{
		Fatal("ID3D11DeviceContext as ID3D11DeviceContext4 failed");
		return false;
	}

	return true;
}
//=============================================================================
bool createSwapChain(HWND hwnd)
{
	using namespace rhiData;

	ComPtr<IDXGIFactory2> dxgiFactory;
	HRESULT result = adapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf()));
	if (FAILED(result))
	{
		DX_ERR("IDXGIAdapter::GetParent failed: ", result);
		return false;
	}

	supportAllowTearing = false;
	if (!vsync)
	{
		ComPtr<IDXGIFactory5> dxgiFactory5;
		if (SUCCEEDED(dxgiFactory.As(&dxgiFactory5)))
		{
			BOOL allowTearing = FALSE;
			result = dxgiFactory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));
			supportAllowTearing = (SUCCEEDED(result) && allowTearing) ? true : false;
		}
	}

	// Color space:
	//  - BT.709 - LDR https://en.wikipedia.org/wiki/Rec._709
	//  - BT.2020 - HDR https://en.wikipedia.org/wiki/Rec._2020
	// Transfer function:
	//  - G10 - linear (gamma 1.0)
	//  - G22 - sRGB (gamma ~2.2)
	//  - G2084 - SMPTE ST.2084 (Perceptual Quantization)
	// Bits per channel:
	//  - 8, 10, 16 (float)
	enum class SwapChainFormat : uint8_t
	{
		BT709_G10_16BIT,
		BT709_G22_8BIT,
		BT709_G22_10BIT,
		BT2020_G2084_10BIT
	};
	SwapChainFormat swapChainFormat{ SwapChainFormat::BT709_G22_8BIT };

	constexpr DXGI_FORMAT cSwapChainFormat[] = {
		DXGI_FORMAT_R16G16B16A16_FLOAT, // BT709_G10_16BIT
		DXGI_FORMAT_R8G8B8A8_UNORM,     // BT709_G22_8BIT
		DXGI_FORMAT_R10G10B10A2_UNORM,  // BT709_G22_10BIT
		DXGI_FORMAT_R10G10B10A2_UNORM,  // BT2020_G2084_10BIT
	};

	constexpr DXGI_COLOR_SPACE_TYPE cColorSpace[] = {
		DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709,    // BT709_G10_16BIT
		DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709,    // BT709_G22_8BIT
		DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709,    // BT709_G22_10BIT
		DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020, // BT2020_G2084_10BIT
	};

	backBufferFormat = cSwapChainFormat[(uint32_t)swapChainFormat];
	colorSpace = cColorSpace[(uint32_t)swapChainFormat];

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = backBufferWidth;
	swapChainDesc.Height = backBufferHeight;
	swapChainDesc.Format = backBufferFormat;
	swapChainDesc.SampleDesc = { 1, 0 };
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = backBufferCount;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
	swapChainDesc.Flags = supportAllowTearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
	fsSwapChainDesc.Windowed = TRUE;

	ComPtr<IDXGISwapChain1> swapChain1;
	result = dxgiFactory->CreateSwapChainForHwnd(d3dDevice.Get(), hwnd, &swapChainDesc, &fsSwapChainDesc, nullptr, swapChain1.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		DX_ERR("IDXGIFactory2::CreateSwapChainForHwnd failed: ", result);
		return false;
	}

	result = dxgiFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);
	if (FAILED(result))
	{
		DX_ERR("IDXGIFactory2::MakeWindowAssociation failed: ", result);
		return false;
	}

	if (FAILED(swapChain1.As(&swapChain)))
	{
		Fatal("IDXGISwapChain1 as IDXGISwapChain4 failed");
		return false;
	}

	setColorSpace1();

	// Background color
	DXGI_RGBA color = { 0.0f, 0.0f, 0.0f, 1.0f };
	swapChain->SetBackgroundColor(&color);

	return true;
}
//=============================================================================
bool createRenderTargetView()
{
	using namespace rhiData;

	ComPtr<ID3D11Texture2D> backBuffer;
	HRESULT result = swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
	if (FAILED(result))
	{
		DX_ERR("IDXGISwapChain1::GetBuffer failed: ", result);
		return false;
	}

	result = d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, rhiData::renderTargetView.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		DX_ERR("ID3D11Device5::CreateRenderTargetView failed: ", result);
		return false;
	}

	const CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencil;
	result = d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf());
	if (FAILED(result))
	{
		DX_ERR("ID3D11Device5::CreateTexture2D failed: ", result);
		return false;
	}

	result = d3dDevice->CreateDepthStencilView(depthStencil.Get(), nullptr, depthStencilView.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		DX_ERR("ID3D11Device5::CreateDepthStencilView failed: ", result);
		return false;
	}

	return true;
}
//=============================================================================
void setColorSpace1()
{
	using namespace rhiData;

	uint32_t colorSpaceSupport = 0;
	HRESULT result = swapChain->CheckColorSpaceSupport(colorSpace, &colorSpaceSupport);

	if (!(colorSpaceSupport & DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT))
		result = E_FAIL;
	if (SUCCEEDED(result))
		swapChain->SetColorSpace1(colorSpace);
}
//=============================================================================
void CuteEngineApp::SetMainFrame()
{
	// TODO: сделать возможность устанавливать цвет очистки. В виде optional и в случае нуля очистка вообще не выполняется

	rhiData::d3dContext->ClearRenderTargetView(rhiData::renderTargetView.Get(), DirectX::Colors::CornflowerBlue);
	rhiData::d3dContext->ClearDepthStencilView(rhiData::depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	rhiData::d3dContext->OMSetRenderTargets(1, rhiData::renderTargetView.GetAddressOf(), rhiData::depthStencilView.Get());
	rhiData::d3dContext->RSSetViewports(1, &rhiData::viewport);
}
//=============================================================================
void CuteEngineApp::Temp()
{
	rhiData::d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	rhiData::d3dContext->Draw(3, 0);
}
//=============================================================================