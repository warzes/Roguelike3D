#include "CuteRHI.h"

#if defined(_MSC_VER)
#	pragma warning(push, 3)
#	pragma warning(disable : 4061)
#	pragma warning(disable : 4365)
#	pragma warning(disable : 4668)
#	pragma warning(disable : 5039)
#endif

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define NOBITMAP
#define NOGDICAPMASKS
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCTLMGR
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
#define NOGDI
#define NOWINSTYLE
#define NOKEYSTATES
#define NOCOLOR
#define NOMB
#define NOWINOFFSETS
#define NOCRYPT
#define NOIME

#include <wrl/client.h>

#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#if defined(_DEBUG)
#	include <dxgidebug.h>
#endif

#include <DirectXMath.h>
#include <DirectXColors.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>

#include <DXError.h>

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif
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
inline DXGI_FORMAT ConvertToD3D11(rhi::DataFormat format)
{
	using namespace rhi;
	switch (format)
	{
	case DataFormat::R1:         return DXGI_FORMAT_R1_UNORM;
	case DataFormat::R8:         return DXGI_FORMAT_R8_UNORM;
	case DataFormat::R16:        return DXGI_FORMAT_R16_UNORM;
	case DataFormat::R16F:       return DXGI_FORMAT_R16_FLOAT;
	case DataFormat::R32I:       return DXGI_FORMAT_R32_SINT;
	case DataFormat::R32U:       return DXGI_FORMAT_R32_UINT;
	case DataFormat::R32F:       return DXGI_FORMAT_R32_FLOAT;
	case DataFormat::RG8:        return DXGI_FORMAT_R8G8_UNORM;
	case DataFormat::RG16:       return DXGI_FORMAT_R16G16_UNORM;
	case DataFormat::RG16F:      return DXGI_FORMAT_R16G16_FLOAT;
	case DataFormat::RG32I:      return DXGI_FORMAT_R32G32_SINT;
	case DataFormat::RG32U:      return DXGI_FORMAT_R32G32_UINT;
	case DataFormat::RG32F:      return DXGI_FORMAT_R32G32_FLOAT;
	case DataFormat::RGB32I:     return DXGI_FORMAT_R32G32B32_SINT;
	case DataFormat::RGB32U:     return DXGI_FORMAT_R32G32B32_UINT;
	case DataFormat::RGB32F:     return DXGI_FORMAT_R32G32B32_FLOAT;
	case DataFormat::RGBA8:      return DXGI_FORMAT_R8G8B8A8_UNORM;
	case DataFormat::RGBA16:     return DXGI_FORMAT_R16G16B16A16_UNORM;
	case DataFormat::RGBA16F:    return DXGI_FORMAT_R16G16B16A16_FLOAT;
	case DataFormat::RGBA32I:    return DXGI_FORMAT_R32G32B32A32_SINT;
	case DataFormat::RGBA32U:    return DXGI_FORMAT_R32G32B32A32_UINT;
	case DataFormat::RGBA32F:    return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case DataFormat::R11G11B10F: return DXGI_FORMAT_R11G11B10_FLOAT;
	default:                     return DXGI_FORMAT_UNKNOWN;
	}
}
//=============================================================================
inline DXGI_FORMAT ConvertToD3D11(rhi::TexelsFormat format)
{
	using namespace rhi;
	switch (format)
	{
	case TexelsFormat::BC1:               return DXGI_FORMAT_BC1_UNORM;        // DXT1
	case TexelsFormat::BC2:               return DXGI_FORMAT_BC2_UNORM;        // DXT3
	case TexelsFormat::BC3:               return DXGI_FORMAT_BC3_UNORM;        // DXT5
	case TexelsFormat::BC4:               return DXGI_FORMAT_BC4_UNORM;        // LATC1/ATI1
	case TexelsFormat::BC5:               return DXGI_FORMAT_BC5_UNORM;        // LATC2/ATI2
	case TexelsFormat::BC6H:              return DXGI_FORMAT_BC6H_TYPELESS;    // BC6H
	case TexelsFormat::BC7:               return DXGI_FORMAT_BC7_UNORM;        // BC7	
	case TexelsFormat::UnknownCompressed: return DXGI_FORMAT_UNKNOWN; // compressed formats above
	case TexelsFormat::R1:                return DXGI_FORMAT_R1_UNORM;         // R1
	case TexelsFormat::R8:                return DXGI_FORMAT_R8_UNORM;         // R8
	case TexelsFormat::R16:               return DXGI_FORMAT_R16_UNORM;        // R16
	case TexelsFormat::R16F:              return DXGI_FORMAT_R16_FLOAT;        // R16F
	case TexelsFormat::R32I:              return DXGI_FORMAT_R32_SINT;         // R32I
	case TexelsFormat::R32U:              return DXGI_FORMAT_R32_UINT;         // R32U
	case TexelsFormat::R32F:              return DXGI_FORMAT_R32_FLOAT;        // R32F
	case TexelsFormat::RG8:               return DXGI_FORMAT_R8G8_UNORM;       // RG8
	case TexelsFormat::RG16:              return DXGI_FORMAT_R16G16_UNORM;     // RG16
	case TexelsFormat::RG16F:             return DXGI_FORMAT_R16G16_FLOAT;     // RG16F
	case TexelsFormat::RG32I:             return DXGI_FORMAT_R32G32_SINT;      // RG32I
	case TexelsFormat::RG32U:             return DXGI_FORMAT_R32G32_UINT;      // RG32U
	case TexelsFormat::RG32F:             return DXGI_FORMAT_R32G32_FLOAT;     // RG32F
	case TexelsFormat::RGB32I:            return DXGI_FORMAT_R32G32B32_SINT;   // RGB32I
	case TexelsFormat::RGB32U:            return DXGI_FORMAT_R32G32B32_UINT;   // RGB32U
	case TexelsFormat::RGB32F:            return DXGI_FORMAT_R32G32B32_FLOAT;  // RGB32F
	case TexelsFormat::RGBA8:             return DXGI_FORMAT_R8G8B8A8_UNORM;   // RGBA8
	case TexelsFormat::RGBA16:            return DXGI_FORMAT_R16G16B16A16_UNORM;// RGBA16
	case TexelsFormat::RGBA16F:           return DXGI_FORMAT_R16G16B16A16_FLOAT;// RGBA16F
	case TexelsFormat::RGBA32I:           return DXGI_FORMAT_R32G32B32A32_SINT;// RGBA32I
	case TexelsFormat::RGBA32U:           return DXGI_FORMAT_R32G32B32A32_UINT;// RGBA32U
	case TexelsFormat::RGBA32F:           return DXGI_FORMAT_R32G32B32A32_FLOAT;// RGBA32F
	case TexelsFormat::R11G11B10F:        return DXGI_FORMAT_R11G11B10_FLOAT; // R11G11B10F
	case TexelsFormat::UnknownDepth:      return DXGI_FORMAT_UNKNOWN;     // depth formats below
	case TexelsFormat::DepthNone:         return DXGI_FORMAT_UNKNOWN;     // depth formats below
	case TexelsFormat::D16:               return DXGI_FORMAT_D16_UNORM;        // D16
	case TexelsFormat::D24S8:             return DXGI_FORMAT_D24_UNORM_S8_UINT;// D24S8
	case TexelsFormat::D32F:              return DXGI_FORMAT_D32_FLOAT;        // D32F
	default:                              return DXGI_FORMAT_UNKNOWN;
	}
}
//=============================================================================
inline D3D11_FILL_MODE ConvertToD3D11(rhi::FillMode fillMode)
{
	using namespace rhi;
	switch (fillMode)
	{
	case FillMode::Solid:     return D3D11_FILL_SOLID;
	case FillMode::Wireframe: return D3D11_FILL_WIREFRAME;
	default:                  return D3D11_FILL_SOLID;
	}
}
//=============================================================================
inline D3D11_CULL_MODE ConvertToD3D11(rhi::CullMode cullMode)
{
	using namespace rhi;
	switch (cullMode)
	{
	case CullMode::Back:  return D3D11_CULL_BACK;
	case CullMode::Front: return D3D11_CULL_FRONT;
	case CullMode::None:  return D3D11_CULL_NONE;
	default:              return D3D11_CULL_BACK;
	}
}
//=============================================================================
inline D3D11_FILTER ConvertToD3D11(rhi::TextureFilter filter)
{
	using namespace rhi;
	switch (filter)
	{
	case TextureFilter::MinMagMip_Point:                 return D3D11_FILTER_MIN_MAG_MIP_POINT;
	case TextureFilter::MinMag_Point_Mip_Linear:         return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	case TextureFilter::Min_Point_Mag_Linear_Mip_Point:  return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
	case TextureFilter::Min_Point_MagMip_Linear:         return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
	case TextureFilter::Min_Linear_MagMip_Point:         return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
	case TextureFilter::Min_Linear_Mag_Point_Mip_Linear: return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
	case TextureFilter::MinMag_Linear_Mip_Point:         return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	case TextureFilter::MinMagMip_Linear:                return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	case TextureFilter::Anisotropic:                     return D3D11_FILTER_ANISOTROPIC;
	default: return D3D11_FILTER_MIN_MAG_MIP_POINT;
	}
}
//=============================================================================
inline D3D11_TEXTURE_ADDRESS_MODE ConvertToD3D11(rhi::AddressMode mode)
{
	using namespace rhi;
	switch (mode)
	{
	case AddressMode::Wrap:   return D3D11_TEXTURE_ADDRESS_WRAP;
	case AddressMode::Mirror: return D3D11_TEXTURE_ADDRESS_MIRROR;
	case AddressMode::Clamp:  return D3D11_TEXTURE_ADDRESS_CLAMP;
	case AddressMode::Border: return D3D11_TEXTURE_ADDRESS_BORDER;
	default:                  return D3D11_TEXTURE_ADDRESS_WRAP;
	}
}
//=============================================================================
inline D3D11_COMPARISON_FUNC ConvertToD3D11(rhi::ComparisonFunc func)
{
	using namespace rhi;
	switch (func)
	{
	case ComparisonFunc::Always:       return D3D11_COMPARISON_ALWAYS;
	case ComparisonFunc::Never:        return D3D11_COMPARISON_NEVER;
	case ComparisonFunc::Less:         return D3D11_COMPARISON_LESS;
	case ComparisonFunc::LessEqual:    return D3D11_COMPARISON_LESS_EQUAL;
	case ComparisonFunc::Greater:      return D3D11_COMPARISON_GREATER;
	case ComparisonFunc::GreaterEqual: return D3D11_COMPARISON_GREATER_EQUAL;
	case ComparisonFunc::Equal:        return D3D11_COMPARISON_EQUAL;
	case ComparisonFunc::NotEqual:     return D3D11_COMPARISON_NOT_EQUAL;
	default:                           return D3D11_COMPARISON_ALWAYS;
	}
}
//=============================================================================
inline D3D11_DEPTH_WRITE_MASK ConvertToD3D11(rhi::DepthWriteMask mask)
{
	using namespace rhi;
	switch (mask)
	{
	case DepthWriteMask::Zero: return D3D11_DEPTH_WRITE_MASK_ZERO;
	case DepthWriteMask::All:  return D3D11_DEPTH_WRITE_MASK_ALL;
	default:                   return D3D11_DEPTH_WRITE_MASK_ZERO;
	}
}
//=============================================================================
inline D3D11_STENCIL_OP ConvertToD3D11(rhi::StencilOp op)
{
	using namespace rhi;
	switch (op)
	{
	case StencilOp::Keep:      return D3D11_STENCIL_OP_KEEP;
	case StencilOp::Zero:      return D3D11_STENCIL_OP_ZERO;
	case StencilOp::Replace:   return D3D11_STENCIL_OP_REPLACE;
	case StencilOp::Increment: return D3D11_STENCIL_OP_INCR;
	case StencilOp::Decrement: return D3D11_STENCIL_OP_DECR;
	default:                   return D3D11_STENCIL_OP_KEEP;
	}
}
//=============================================================================
inline D3D11_USAGE ConvertToD3D11(rhi::BufferUsage usage)
{
	using namespace rhi;
	switch (usage)
	{
	case BufferUsage::Default:   return D3D11_USAGE_DEFAULT;
	case BufferUsage::Immutable: return D3D11_USAGE_IMMUTABLE;
	case BufferUsage::Dynamic:   return D3D11_USAGE_DYNAMIC;
	case BufferUsage::Staging:   return D3D11_USAGE_STAGING;
	default:                     return D3D11_USAGE_DEFAULT;
	}
}
//=============================================================================
inline UINT ConvertToD3D11(rhi::CPUAccessFlags flags)
{
	using namespace rhi;
	switch (flags)
	{
	case CPUAccessFlags::None:      return 0;
	case CPUAccessFlags::Write:     return D3D11_CPU_ACCESS_WRITE;
	case CPUAccessFlags::Read:      return D3D11_CPU_ACCESS_READ;
	case CPUAccessFlags::WriteRead: return D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	default:                        return 0;
	}
}
//=============================================================================
inline D3D11_MAP ConvertToD3D11(rhi::MapType type)
{
	using namespace rhi;
	switch (type)
	{
	case MapType::Read:  return D3D11_MAP_READ;
	case MapType::Write: return D3D11_MAP_WRITE_DISCARD;
	default:             return D3D11_MAP_READ; // Значение по умолчанию
	}
}
//=============================================================================
inline D3D11_BLEND ConvertToD3D11(rhi::BlendFactor type)
{
	using namespace rhi;
	switch (type)
	{
	case BlendFactor::Zero:             return D3D11_BLEND_ZERO;
	case BlendFactor::One:              return D3D11_BLEND_ONE;
	case BlendFactor::SrcAlpha:         return D3D11_BLEND_SRC_ALPHA;
	case BlendFactor::DstAlpha:         return D3D11_BLEND_DEST_ALPHA;
	case BlendFactor::OneMinusSrcAlpha: return D3D11_BLEND_INV_SRC_ALPHA;
	case BlendFactor::OneMinusDstAlpha: return D3D11_BLEND_INV_DEST_ALPHA;
	case BlendFactor::SrcColor:         return D3D11_BLEND_SRC_COLOR;
	case BlendFactor::DstColor:         return D3D11_BLEND_DEST_COLOR;
	case BlendFactor::OneMinusSrcColor: return D3D11_BLEND_INV_SRC_COLOR;
	case BlendFactor::OneMinusDstColor: return D3D11_BLEND_INV_DEST_COLOR;
	default:                            return D3D11_BLEND_ONE;
	}
}
//=============================================================================
inline D3D11_BLEND_OP ConvertToD3D11(rhi::BlendOp type)
{
	using namespace rhi;
	switch (type)
	{
	case BlendOp::Add:         return D3D11_BLEND_OP_ADD;
	case BlendOp::Subtract:    return D3D11_BLEND_OP_SUBTRACT;
	case BlendOp::RevSubtract: return D3D11_BLEND_OP_REV_SUBTRACT;
	case BlendOp::Min:         return D3D11_BLEND_OP_MIN;
	case BlendOp::Max:         return D3D11_BLEND_OP_MAX;
	default:                   return D3D11_BLEND_OP_ADD;
	}
}
//=============================================================================
inline D3D11_PRIMITIVE_TOPOLOGY ConvertToD3D11(rhi::PrimitiveTopology topology)
{
	using namespace rhi;
	switch (topology)
	{
	case PrimitiveTopology::TriangleList:  return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	case PrimitiveTopology::TriangleStrip: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	case PrimitiveTopology::PointList:     return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	default: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}
}
//=============================================================================
#define DX_ERR_STR(str,hr) std::string(str + std::string(HRToString(hr)) + ".\n\tFile: " + std::string(__FILE__) + ", Line: " + std::to_string(__LINE__))
#define DX_ERR(str,hr) rhi::Fatal(DX_ERR_STR(str, hr))
//=============================================================================
namespace rhi
{
	using Microsoft::WRL::ComPtr;

	ComPtr<ID3D11Device5>             d3dDevice;
	ComPtr<ID3D11DeviceContext4>      d3dContext;
	ComPtr<ID3DUserDefinedAnnotation> d3dDebugAnnotation;

	ComPtr<IDXGISwapChain4>           swapChain;
	DXGI_FORMAT                       backBufferFormat{ };
	DXGI_COLOR_SPACE_TYPE             colorSpace{};
	uint32_t                          backBufferWidth{ 0 };
	uint32_t                          backBufferHeight{ 0 };
	D3D11_VIEWPORT                    viewport{};

	ComPtr<ID3D11RenderTargetView>    renderTargetView;
	ComPtr<ID3D11DepthStencilView>    depthStencilView;
	const DXGI_FORMAT                 depthBufferFormat{ DXGI_FORMAT_D24_UNORM_S8_UINT };
	const UINT                        backBufferCount{ 2 };
	bool                              vsync{ false };
	bool                              supportAllowTearing{ false };

	bool                              shouldClose{ true };
}
//=============================================================================
namespace stateCache
{
	void Clear();

	ID3D11Buffer* vertexBuffer[rhi::MaxVertexBuffers] = { nullptr };
	UINT          vertexBufferStride[rhi::MaxVertexBuffers] = { 0 };
	UINT          vertexBufferOffset[rhi::MaxVertexBuffers] = { 0 };
	uint32_t      currentFreeVertexBuffer = 0;
} // namespace stateCache
//=============================================================================
struct rhi::ShaderProgram final
{
	Microsoft::WRL::ComPtr<ID3D11VertexShader>   vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>    inputLayout;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>    pixelShader;

	Microsoft::WRL::ComPtr<ID3D11HullShader>     hullShader;
	Microsoft::WRL::ComPtr<ID3D11DomainShader>   domainShader;

	Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometryShader;

	Microsoft::WRL::ComPtr<ID3D11ComputeShader>  computeShader;
};
//=============================================================================
struct rhi::PipelineState final
{
	Microsoft::WRL::ComPtr<ID3D11RasterizerState2>  rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11BlendState1>       blendState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	uint32_t                                        stencilRef;
};
//=============================================================================
struct rhi::SamplerState final
{
	Microsoft::WRL::ComPtr<ID3D11SamplerState> state;
};
//=============================================================================
struct rhi::Buffer final
{
	Microsoft::WRL::ComPtr<ID3D11Buffer>               buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>               indirectBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   dataView;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView1> dataUAV;
	UINT                                               bindFlags;
	UINT                                               dataBufferSize;
	UINT                                               dataBufferStride;
};
//=============================================================================
struct rhi::ConstantBuffer final
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
};
//=============================================================================
struct rhi::Texture final
{
	Microsoft::WRL::ComPtr<ID3D11Resource>             texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   dataView;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView1> dataUAV;
	TextureType                                        type;
};
//=============================================================================
struct rhi::RenderTarget final
{
	UINT                                            numRenderTargets{ 0 };
	TexturePtr                                      renderTargetTextures[RenderTargetSlotCount];
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView1> renderTargetViews[RenderTargetSlotCount];
	TexturePtr                                      depthStencilTexture;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  depthStencilView;
	uint32_t                                        width{ 0 };
	uint32_t                                        height{ 0 };
};
//=============================================================================
bool setBackBufferSize(uint32_t width, uint32_t height)
{
	if (rhi::backBufferWidth == width && rhi::backBufferHeight == height) return false;

	rhi::backBufferWidth = std::max(width, 1u);
	rhi::backBufferHeight = std::max(height, 1u);
	rhi::viewport = { 0.0f, 0.0f, static_cast<float>(rhi::backBufferWidth), static_cast<float>(rhi::backBufferHeight), 0.f, 1.f };

	return true;
}
//=============================================================================
bool selectAdapter(Microsoft::WRL::ComPtr<IDXGIFactory6>& DXGIFactory6, Microsoft::WRL::ComPtr<IDXGIAdapter4>& DXGIAdapter4)
{
	using namespace rhi;

	UINT dxgiFactoryFlags = 0;
#ifdef _DEBUG
	dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	Microsoft::WRL::ComPtr<IDXGIFactory2> DXGIFactory;
	HRESULT result = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&DXGIFactory));
	if (FAILED(result))
	{
		DX_ERR("CreateDXGIFactory2() failed: ", result);
		return false;
	}
	result = DXGIFactory.As(&DXGIFactory6);
	if (FAILED(result))
	{
		DX_ERR("IDXGIFactory As IDXGIFactory6 failed: ", result);
		return false;
	}

	Microsoft::WRL::ComPtr<IDXGIAdapter> DXGIAdapter;
	result = DXGIFactory6->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&DXGIAdapter));
	if (FAILED(result))
	{
		DX_ERR("IDXGIFactory6::EnumAdapterByGpuPreference() failed: ", result);
		return false;
	}
	result = DXGIAdapter.As(&DXGIAdapter4);
	if (FAILED(result))
	{
		DX_ERR("DXGIAdapter As DXGIAdapter4 failed: ", result);
		return false;
	}

	return true;
}
//=============================================================================
bool createDevice(Microsoft::WRL::ComPtr<IDXGIAdapter4> DXGIAdapter)
{
	using namespace rhi;

	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
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
	HRESULT result = D3D11CreateDevice(DXGIAdapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, nullptr, creationFlags, featureLevels, static_cast<UINT>(std::size(featureLevels)), D3D11_SDK_VERSION, &device, nullptr, &context);
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

	if (FAILED(d3dContext.As(&d3dDebugAnnotation)))
	{
		Fatal("ID3D11DeviceContext as ID3DUserDefinedAnnotation failed");
		return false;
	}

	return true;
}
//=============================================================================
void setColorSpace1()
{
	uint32_t colorSpaceSupport = 0;
	HRESULT result = rhi::swapChain->CheckColorSpaceSupport(rhi::colorSpace, &colorSpaceSupport);

	if (!(colorSpaceSupport & DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT))
		result = E_FAIL;
	if (SUCCEEDED(result))
		rhi::swapChain->SetColorSpace1(rhi::colorSpace);
}
//=============================================================================
bool createSwapChain(HWND hwnd, Microsoft::WRL::ComPtr<IDXGIFactory6> DXGIFactory)
{
	using namespace rhi;

	supportAllowTearing = false;
	if (!vsync)
	{
		BOOL allowTearing = FALSE;
		HRESULT result = DXGIFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));
		supportAllowTearing = (SUCCEEDED(result) && allowTearing) ? true : false;
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
	HRESULT result = DXGIFactory->CreateSwapChainForHwnd(d3dDevice.Get(), hwnd, &swapChainDesc, &fsSwapChainDesc, nullptr, &swapChain1);
	if (FAILED(result))
	{
		DX_ERR("IDXGIFactory2::CreateSwapChainForHwnd failed: ", result);
		return false;
	}

	result = DXGIFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);
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
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	HRESULT result = rhi::swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
	if (FAILED(result))
	{
		DX_ERR("IDXGISwapChain1::GetBuffer failed: ", result);
		return false;
	}

	result = rhi::d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, rhi::renderTargetView.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		DX_ERR("ID3D11Device5::CreateRenderTargetView failed: ", result);
		return false;
	}

	const CD3D11_TEXTURE2D_DESC depthStencilDesc(rhi::depthBufferFormat, rhi::backBufferWidth, rhi::backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencil;
	result = rhi::d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf());
	if (FAILED(result))
	{
		DX_ERR("ID3D11Device5::CreateTexture2D failed: ", result);
		return false;
	}

	result = rhi::d3dDevice->CreateDepthStencilView(depthStencil.Get(), nullptr, &rhi::depthStencilView);
	if (FAILED(result))
	{
		DX_ERR("ID3D11Device5::CreateDepthStencilView failed: ", result);
		return false;
	}

	return true;
}
//=============================================================================
bool initImGui(void* hwnd)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	if (!ImGui_ImplWin32_Init(hwnd))
		return false;
	if (!ImGui_ImplDX11_Init(rhi::d3dDevice.Get(), rhi::d3dContext.Get()))
		return false;

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != nullptr);

	return true;
}
//=============================================================================
void rhi::Print(const std::string& message)
{
	// TODO: пользовательская функция
	puts(message.c_str());
}
//=============================================================================
void rhi::Print(const std::wstring& message)
{
	// TODO: пользовательская функция
	_putws(message.c_str());
}
//=============================================================================
void rhi::Fatal(const std::string& error)
{
	// TODO: пользовательская функция
	puts(("FATAL: " + error).c_str());
	shouldClose = true;
}
//=============================================================================
bool rhi::Create(void* hwnd, uint32_t frameWidth, uint32_t frameHeight, bool vSync)
{
	if (!DirectX::XMVerifyCPUSupport())
	{
		Fatal("DirectX::XMVerifyCPUSupport() failed.");
		return false;
	}

	if (FAILED(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED)))
	{
		Fatal("CoInitializeEx() failed.");
		return false;
	}

	setBackBufferSize(frameWidth, frameHeight);
	rhi::vsync = vSync;

	Microsoft::WRL::ComPtr<IDXGIFactory6> DXGIFactory;
	Microsoft::WRL::ComPtr<IDXGIAdapter4> DXGIAdapter;

	if (!selectAdapter(DXGIFactory, DXGIAdapter))  return false;
	if (!createDevice(DXGIAdapter))                return false;
	if (!createSwapChain((HWND)hwnd, DXGIFactory)) return false;
	if (!createRenderTargetView())                 return false;

	if (!initImGui(hwnd))
	{
		Fatal("initImGui() return false");
		return false;
	}

	shouldClose = false;
	return true;
}
//=============================================================================
void rhi::Destroy()
{
	if (ImGui::GetCurrentContext() != nullptr && ImGui::GetIO().BackendPlatformUserData != nullptr)
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	if (d3dContext) d3dContext->OMSetRenderTargets(0, nullptr, nullptr);
	if (swapChain)  swapChain->SetFullscreenState(FALSE, nullptr);

	depthStencilView.Reset();
	renderTargetView.Reset();
	swapChain.Reset();
	d3dDebugAnnotation.Reset();
	d3dContext.Reset();

#if defined(_DEBUG)
	ComPtr<ID3D11Debug> debugDevice;
	if (SUCCEEDED(d3dDevice.As(&debugDevice)))
	{
		debugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	}
	debugDevice.Reset();
#endif
	d3dDevice.Reset();

#if defined(_DEBUG)
	ComPtr<IDXGIDebug1> debug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug))))
	{
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_SUMMARY | DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
	}
#endif

	shouldClose = true;
}
//=============================================================================
bool rhi::IsValid()
{
	return shouldClose;
}
//=============================================================================
void rhi::BeginFrame()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	bool show_demo_window = true;
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	ImGui::Begin("Hello, world!");
	ImGui::Text("This is some useful text.");
	ImGui::SameLine();
	ImGui::Text("2This is some useful text.");
	ImGui::End();

	ImGui::Render();

	rhi::d3dContext->ClearRenderTargetView(rhi::renderTargetView.Get(), DirectX::Colors::CornflowerBlue);
	rhi::d3dContext->ClearDepthStencilView(rhi::depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	rhi::d3dContext->OMSetRenderTargets(1, rhi::renderTargetView.GetAddressOf(), rhi::depthStencilView.Get());
	rhi::d3dContext->RSSetViewports(1, &rhi::viewport);
}
//=============================================================================
void rhi::EndFrame()
{
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	const UINT syncInterval = vsync ? 1 : 0;
	const UINT presentFlags = supportAllowTearing ? DXGI_PRESENT_ALLOW_TEARING : 0;
	HRESULT result = swapChain->Present(syncInterval, presentFlags);
	if (FAILED(result))
	{
		DX_ERR("IDXGISwapChain1::Present failed: ", result);
	}
}
//=============================================================================
bool rhi::Resize(uint32_t width, uint32_t height)
{
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
void rhi::BeginPerfEvent(const wchar_t* name)
{
	rhi::d3dDebugAnnotation->BeginEvent(name);
}
//=============================================================================
void rhi::EndPerfEvent()
{
	rhi::d3dDebugAnnotation->EndEvent();
}
//=============================================================================
void rhi::SetMainFrame(const std::optional<Color>& clearColor, const std::optional<float>& clearDepth, const std::optional<uint8_t>& clearStencil)
{
	if (clearColor.has_value())
	{
		rhi::d3dContext->ClearRenderTargetView(rhi::renderTargetView.Get(), clearColor.value().Get());
	}
	if (clearDepth.has_value())
	{
		uint8_t stencil = clearStencil.has_value() ? clearStencil.value() : 0;
		rhi::d3dContext->ClearDepthStencilView(rhi::depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, clearDepth.value(), stencil);
	}

	rhi::d3dContext->OMSetRenderTargets(1, rhi::renderTargetView.GetAddressOf(), rhi::depthStencilView.Get());
	rhi::d3dContext->RSSetViewports(1, &rhi::viewport);
}
//=============================================================================
void rhi::Draw(PrimitiveTopology topology, uint32_t vertexCount, uint32_t startVertexLocation)
{
	rhi::d3dContext->IASetPrimitiveTopology(ConvertToD3D11(topology));
	rhi::d3dContext->Draw(vertexCount, startVertexLocation);
}
//=============================================================================
void rhi::DrawIndexed(PrimitiveTopology topology, uint32_t indexCount, uint32_t startIndexLocation, uint32_t baseVertexLocation)
{
	rhi::d3dContext->IASetPrimitiveTopology(ConvertToD3D11(topology));
	rhi::d3dContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}
//=============================================================================
void rhi::DrawInstanced(PrimitiveTopology topology, uint32_t vertexCountPerInstance, uint32_t instanceCount, uint32_t startVertexLocation, uint32_t startInstanceLocation)
{
	rhi::d3dContext->IASetPrimitiveTopology(ConvertToD3D11(topology));
	rhi::d3dContext->DrawInstanced(vertexCountPerInstance, instanceCount, startVertexLocation, startInstanceLocation);
}
//=============================================================================
void rhi::DrawIndexedInstanced(PrimitiveTopology topology, uint32_t indexCountPerInstance, uint32_t instanceCount, uint32_t startIndexLocation, uint32_t baseVertexLocation, uint32_t startInstanceLocation)
{
	rhi::d3dContext->IASetPrimitiveTopology(ConvertToD3D11(topology));
	rhi::d3dContext->DrawIndexedInstanced(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
}
//=============================================================================
void rhi::Flush()
{
	rhi::d3dContext->Flush();
}
//=============================================================================
inline std::expected<Microsoft::WRL::ComPtr<ID3DBlob>, std::string> CompileShaderFromFile(const rhi::ShaderLoadInfo& loadInfo, const std::string& target)
{
	using namespace rhi;

	if (loadInfo.entryPoint.empty()) return std::unexpected("Entry point in ShaderLoadInfo is empty.");

	D3D_SHADER_MACRO* d3dmacros = nullptr;
	if (!loadInfo.macros.empty())
	{
		d3dmacros = new D3D_SHADER_MACRO[loadInfo.macros.size()];
		for (size_t i = 0; i < loadInfo.macros.size(); ++i)
		{
			d3dmacros[i].Name = loadInfo.macros[i].name;
			d3dmacros[i].Definition = loadInfo.macros[i].value;
		}
	}

	UINT d3dFlags = 0;
	if (loadInfo.flags & static_cast<UINT>(ShaderCompileFlags::Debug))     d3dFlags |= D3DCOMPILE_DEBUG;
	if (loadInfo.flags & static_cast<UINT>(ShaderCompileFlags::Strict))    d3dFlags |= D3DCOMPILE_ENABLE_STRICTNESS;
	if (loadInfo.flags & static_cast<UINT>(ShaderCompileFlags::IEEStrict)) d3dFlags |= D3DCOMPILE_IEEE_STRICTNESS;
	if (loadInfo.flags & static_cast<UINT>(ShaderCompileFlags::Optimize0)) d3dFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL0;
	if (loadInfo.flags & static_cast<UINT>(ShaderCompileFlags::Optimize1)) d3dFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL1;
	if (loadInfo.flags & static_cast<UINT>(ShaderCompileFlags::Optimize2)) d3dFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL2;
	if (loadInfo.flags & static_cast<UINT>(ShaderCompileFlags::Optimize3)) d3dFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;

	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
	HRESULT result = D3DCompileFromFile(loadInfo.file.c_str(), d3dmacros, 0, loadInfo.entryPoint.c_str(), target.c_str(), d3dFlags, 0, &blob, &errorBlob);
	delete[] d3dmacros;
	if (FAILED(result))
	{
		std::string errorText = DX_ERR_STR("D3DCompileFromFile() failed: ", result);
		if (errorBlob != nullptr)
		{
			std::string error;
			error.resize(errorBlob->GetBufferSize());
			std::memcpy(error.data(), errorBlob->GetBufferPointer(), errorBlob->GetBufferSize());
			errorText += "\n";
			errorText += error;
		}

		return std::unexpected(errorText);
	}
	return blob;
}
//=============================================================================
inline void* Map(ID3D11Resource* resource, rhi::MapType type)
{
	D3D11_MAPPED_SUBRESOURCE mappedData = { 0 };
	HRESULT result = rhi::d3dContext->Map(resource, 0, ConvertToD3D11(type), 0, &mappedData);
	if (FAILED(result))
	{
		DX_ERR("ID3D11Device5::Map() failed: ", result);
		return nullptr;
	}
	return mappedData.pData;
}
//=============================================================================
inline void UpdateSubresource(ID3D11Resource* resource, const void* mem, uint32_t mip, size_t offsetX, size_t sizeX, size_t offsetY, size_t sizeY, size_t offsetZ, size_t sizeZ, size_t rowPitch, size_t depthPitch)
{
	D3D11_BOX box{};
	box.left = static_cast<UINT>(offsetX);
	box.right = static_cast<UINT>(offsetX + sizeX);
	box.top = static_cast<UINT>(offsetY);
	box.bottom = static_cast<UINT>(offsetY + sizeY);
	box.front = static_cast<UINT>(offsetZ);
	box.back = static_cast<UINT>(offsetZ + sizeZ);

	rhi::d3dContext->UpdateSubresource(resource, mip, &box, mem, static_cast<UINT>(rowPitch), static_cast<UINT>(depthPitch));
}
//=============================================================================
inline void ClearRW(ID3D11UnorderedAccessView1* uav, uint32_t value)
{
	uint32_t d3dValues[4] = { value, 0, 0, 0 };
	if (uav) rhi::d3dContext->ClearUnorderedAccessViewUint(uav, d3dValues);
}
//=============================================================================
inline void ClearRW(ID3D11UnorderedAccessView1* uav, float value)
{
	float d3dValues[4] = { value, 0.0f, 0.0f, 0.0f };
	if (uav) rhi::d3dContext->ClearUnorderedAccessViewFloat(uav, d3dValues);
}
//=============================================================================
inline void SetShaderResource(ID3D11ShaderResourceView* const* resource, rhi::ShaderType shaderType, uint32_t slot)
{
	switch (shaderType)
	{
	case rhi::ShaderType::Vertex:
		rhi::d3dContext->VSSetShaderResources(slot, 1, resource);
		break;
	case rhi::ShaderType::Pixel:
		rhi::d3dContext->PSSetShaderResources(slot, 1, resource);
		break;
	case rhi::ShaderType::Hull:
		rhi::d3dContext->HSSetShaderResources(slot, 1, resource);
		break;
	case rhi::ShaderType::Domain:
		rhi::d3dContext->DSSetShaderResources(slot, 1, resource);
		break;
	case rhi::ShaderType::Geometry:
		rhi::d3dContext->GSSetShaderResources(slot, 1, resource);
		break;
	case rhi::ShaderType::Compute:
		rhi::d3dContext->CSSetShaderResources(slot, 1, resource);
		break;
	}
}
//=============================================================================
std::expected<rhi::ShaderProgramPtr, std::string> rhi::LoadShaderProgram(const ShaderProgramLoadInfo& loadInfo)
{
	ShaderProgramPtr program = std::make_shared<ShaderProgram>();

	// Create Vertex Shader
	{
		if (!loadInfo.vertexShader.file.empty())
		{
			Print(L"Load Vertex Shader in File: " + loadInfo.vertexShader.file);
			auto blobRet = CompileShaderFromFile(loadInfo.vertexShader, "vs_5_0");
			if (!blobRet.has_value()) return std::unexpected(blobRet.error());
			auto blob = blobRet.value();
			HRESULT result = rhi::d3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &program->vertexShader);
			if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateVertexShader() failed: ", result));

			// Create Input Layout
			{
				if (loadInfo.inputLayout.empty())
				{
					// TODO: сделать получение из шейдера если не введены данные
				}
				else
				{
					std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc(loadInfo.inputLayout.size());

					for (size_t i = 0; i < loadInfo.inputLayout.size(); i++)
					{
						const auto& elem = loadInfo.inputLayout[i];

						inputElementDesc[i].SemanticName = elem.semanticName.c_str();
						inputElementDesc[i].SemanticIndex = elem.semanticIndex;
						inputElementDesc[i].Format = ConvertToD3D11(elem.format);
						inputElementDesc[i].InputSlot = elem.slot;
						inputElementDesc[i].AlignedByteOffset = static_cast<UINT>(elem.offset);
						inputElementDesc[i].InputSlotClass = elem.perInstanceData ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;
						inputElementDesc[i].InstanceDataStepRate = elem.perInstanceData ? elem.instanceDataStepRate : 0;
					}
					HRESULT result = rhi::d3dDevice->CreateInputLayout(inputElementDesc.data(), inputElementDesc.size(), blob->GetBufferPointer(), blob->GetBufferSize(), &program->inputLayout);
					if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateInputLayout() failed: ", result));
				}
			}
		}
	}

	// Create Pixel Shader
	{
		if (!loadInfo.pixelShader.file.empty())
		{
			Print(L"Load Pixel Shader in File: " + loadInfo.pixelShader.file);
			auto blobRet = CompileShaderFromFile(loadInfo.pixelShader, "ps_5_0");
			if (!blobRet.has_value()) return std::unexpected(blobRet.error());
			auto blob = blobRet.value();
			HRESULT result = rhi::d3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &program->pixelShader);
			if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreatePixelShader() failed: ", result));
		}
	}

	// Create Hull Shader
	{
		if (!loadInfo.hullShader.file.empty())
		{
			Print(L"Load Hull Shader in File: " + loadInfo.hullShader.file);
			auto blobRet = CompileShaderFromFile(loadInfo.hullShader, "hs_5_0");
			if (!blobRet.has_value()) return std::unexpected(blobRet.error());
			auto blob = blobRet.value();
			HRESULT result = rhi::d3dDevice->CreateHullShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &program->hullShader);
			if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateHullShader() failed: ", result));
		}
	}

	// Create Domain Shader
	{
		if (!loadInfo.domainShader.file.empty())
		{
			Print(L"Load Domain Shader in File: " + loadInfo.domainShader.file);
			auto blobRet = CompileShaderFromFile(loadInfo.domainShader, "ds_5_0");
			if (!blobRet.has_value()) return std::unexpected(blobRet.error());
			auto blob = blobRet.value();
			HRESULT result = rhi::d3dDevice->CreateDomainShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &program->domainShader);
			if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateDomainShader() failed: ", result));
		}
	}

	// Create Geometry Shader
	{
		if (!loadInfo.geometryShader.file.empty())
		{
			Print(L"Load Geometry Shader in File: " + loadInfo.geometryShader.file);
			auto blobRet = CompileShaderFromFile(loadInfo.geometryShader, "gs_5_0");
			if (!blobRet.has_value()) return std::unexpected(blobRet.error());
			auto blob = blobRet.value();
			HRESULT result = rhi::d3dDevice->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &program->geometryShader);
			if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateGeometryShader() failed: ", result));
		}
	}

	// Create Compute Shader
	{
		if (!loadInfo.computeShader.file.empty())
		{
			Print(L"Load Compute Shader in File: " + loadInfo.computeShader.file);
			auto blobRet = CompileShaderFromFile(loadInfo.geometryShader, "cs_5_0");
			if (!blobRet.has_value()) return std::unexpected(blobRet.error());
			auto blob = blobRet.value();
			HRESULT result = rhi::d3dDevice->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &program->computeShader);
			if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateComputeShader() failed: ", result));
		}
	}

	return program;
}
//=============================================================================
std::expected<rhi::PipelineStatePtr, std::string> rhi::CreatePipelineState(const PipelineStateCreateInfo& createInfo)
{
	PipelineStatePtr pipelineState = std::make_shared<PipelineState>();

	// create rasterizer state
	{
		const auto& state = createInfo.rasterizerState;

		D3D11_RASTERIZER_DESC2 rasterizerDesc{};
		rasterizerDesc.FillMode = ConvertToD3D11(state.fillMode);
		rasterizerDesc.CullMode = ConvertToD3D11(state.cullMode);
		rasterizerDesc.FrontCounterClockwise = state.counterDirection == CounterDirection::CW ? TRUE : FALSE;
		rasterizerDesc.DepthBias = 0;
		rasterizerDesc.DepthBiasClamp = 0.0f;
		rasterizerDesc.SlopeScaledDepthBias = 0.0f;
		rasterizerDesc.DepthClipEnable = TRUE;
		rasterizerDesc.ScissorEnable = FALSE;
		rasterizerDesc.MultisampleEnable = FALSE;
		rasterizerDesc.AntialiasedLineEnable = FALSE;
		rasterizerDesc.ForcedSampleCount = 0;
		rasterizerDesc.ConservativeRaster = D3D11_CONSERVATIVE_RASTERIZATION_MODE_OFF;

		HRESULT result = rhi::d3dDevice->CreateRasterizerState2(&rasterizerDesc, &pipelineState->rasterizerState);
		if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateRasterizerState2() failed: ", result));
	}

	// create blend state
	{
		const auto& state = createInfo.blendState;

		D3D11_BLEND_DESC1 blendDesc = { 0 };
		blendDesc.AlphaToCoverageEnable = state.alphaToCoverageEnabled;
		blendDesc.IndependentBlendEnable = state.separateBlendEnabled;

		//if (state.blendDesc.blendEnabled) {
		blendDesc.RenderTarget[0].BlendEnable = state.blendDesc.blendEnabled;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = static_cast<UINT8>(state.renderTargetBlendDesc[0].writeMask);
		blendDesc.RenderTarget[0].SrcBlend = ConvertToD3D11(state.blendDesc.srcBlend);
		blendDesc.RenderTarget[0].DestBlend = ConvertToD3D11(state.blendDesc.dstBlend);
		blendDesc.RenderTarget[0].BlendOp = ConvertToD3D11(state.blendDesc.blendOp);
		blendDesc.RenderTarget[0].SrcBlendAlpha = ConvertToD3D11(state.blendDesc.srcBlendAlpha);
		blendDesc.RenderTarget[0].DestBlendAlpha = ConvertToD3D11(state.blendDesc.dstBlendAlpha);
		blendDesc.RenderTarget[0].BlendOpAlpha = ConvertToD3D11(state.blendDesc.blendOpAlpha);
		//}
		// TODO: D3D11_RENDER_TARGET_BLEND_DESC1::LogicOp and D3D11_RENDER_TARGET_BLEND_DESC1::LogicOpEnable

		if (state.separateBlendEnabled)
		{
			for (size_t i = 0; i < 8; ++i)
			{
				blendDesc.RenderTarget[i].BlendEnable = state.renderTargetBlendDesc[i].blendEnabled;
				blendDesc.RenderTarget[i].RenderTargetWriteMask = static_cast<UINT8>(state.renderTargetBlendDesc[i].writeMask);
				blendDesc.RenderTarget[i].SrcBlend = ConvertToD3D11(state.renderTargetBlendDesc[i].srcBlend);
				blendDesc.RenderTarget[i].DestBlend = ConvertToD3D11(state.renderTargetBlendDesc[i].dstBlend);
				blendDesc.RenderTarget[i].BlendOp = ConvertToD3D11(state.renderTargetBlendDesc[i].blendOp);
				blendDesc.RenderTarget[i].SrcBlendAlpha = ConvertToD3D11(state.renderTargetBlendDesc[i].srcBlendAlpha);
				blendDesc.RenderTarget[i].DestBlendAlpha = ConvertToD3D11(state.renderTargetBlendDesc[i].dstBlendAlpha);
				blendDesc.RenderTarget[i].BlendOpAlpha = ConvertToD3D11(state.renderTargetBlendDesc[i].blendOpAlpha);
				// TODO: D3D11_RENDER_TARGET_BLEND_DESC1::LogicOp and D3D11_RENDER_TARGET_BLEND_DESC1::LogicOpEnable
			}
		}

		HRESULT result = rhi::d3dDevice->CreateBlendState1(&blendDesc, &pipelineState->blendState);
		if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateBlendState1() failed: ", result));
	}

	// create depth stencil state
	{
		const auto& state = createInfo.depthStencilState;

		D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
		depthStencilDesc.DepthEnable = state.depthEnabled;
		depthStencilDesc.DepthWriteMask = ConvertToD3D11(state.writeMask);
		depthStencilDesc.DepthFunc = ConvertToD3D11(state.depthFunc);
		depthStencilDesc.StencilEnable = state.stencilEnabled;
		depthStencilDesc.StencilReadMask = state.stencilReadMask;
		depthStencilDesc.StencilWriteMask = state.stencilWriteMask;
		depthStencilDesc.FrontFace.StencilFunc = ConvertToD3D11(state.frontFaceStencilDesc.stencilFunc);
		depthStencilDesc.FrontFace.StencilFailOp = ConvertToD3D11(state.frontFaceStencilDesc.failOp);
		depthStencilDesc.FrontFace.StencilDepthFailOp = ConvertToD3D11(state.frontFaceStencilDesc.depthFailOp);
		depthStencilDesc.FrontFace.StencilPassOp = ConvertToD3D11(state.frontFaceStencilDesc.passOp);
		depthStencilDesc.BackFace.StencilFunc = ConvertToD3D11(state.backFaceStencilDesc.stencilFunc);
		depthStencilDesc.BackFace.StencilFailOp = ConvertToD3D11(state.backFaceStencilDesc.failOp);
		depthStencilDesc.BackFace.StencilDepthFailOp = ConvertToD3D11(state.backFaceStencilDesc.depthFailOp);
		depthStencilDesc.BackFace.StencilPassOp = ConvertToD3D11(state.backFaceStencilDesc.passOp);

		HRESULT result = rhi::d3dDevice->CreateDepthStencilState(&depthStencilDesc, &pipelineState->depthStencilState);
		if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateDepthStencilState() failed: ", result));

		pipelineState->stencilRef = state.stencilRef;
	}

	return pipelineState;
}
//=============================================================================
std::expected<rhi::SamplerStatePtr, std::string> rhi::CreateSamplerState(const SamplerStateCreateInfo& createInfo)
{
	SamplerStatePtr samplerState = std::make_shared<SamplerState>();

	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = ConvertToD3D11(createInfo.filter);
	samplerDesc.AddressU = ConvertToD3D11(createInfo.addressU);
	samplerDesc.AddressV = ConvertToD3D11(createInfo.addressV);
	samplerDesc.AddressW = ConvertToD3D11(createInfo.addressW);
	samplerDesc.MipLODBias = createInfo.lodBias;
	samplerDesc.MaxAnisotropy = createInfo.maxAnisotropy;
	samplerDesc.ComparisonFunc = ConvertToD3D11(createInfo.comparisonFunc);
	samplerDesc.BorderColor[0] = static_cast<float>((createInfo.borderColor >> 0) & 0xFF) / 255.0f;
	samplerDesc.BorderColor[1] = static_cast<float>((createInfo.borderColor >> 8) & 0xFF) / 255.0f;
	samplerDesc.BorderColor[2] = static_cast<float>((createInfo.borderColor >> 16) & 0xFF) / 255.0f;
	samplerDesc.BorderColor[3] = static_cast<float>((createInfo.borderColor >> 24) & 0xFF) / 255.0f;
	samplerDesc.MinLOD = createInfo.minLod;
	samplerDesc.MaxLOD = createInfo.maxLod;

	HRESULT result = rhi::d3dDevice->CreateSamplerState(&samplerDesc, &samplerState->state);
	if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateSamplerState() failed: ", result));

	return samplerState;
}
//=============================================================================
std::expected<rhi::BufferPtr, std::string> rhi::CreateBuffer(const BufferCreateInfo& createInfo)
{
	BufferPtr buffer = std::make_shared<Buffer>();

	D3D11_USAGE bufferUsage = D3D11_USAGE_IMMUTABLE;
	UINT        bufferCPUFlags = 0;
	UINT        bufferBindFlag = 0;
	UINT        bufferMiscFlag = 0;

	bool isStructured = false;
	bool isUAV = false;
	bool isCounter = false;
	bool isAppend = false;
	bool isIndirect = false;

	if (createInfo.flags & BufferFlags::VertexBuffer)
		bufferBindFlag = D3D11_BIND_VERTEX_BUFFER;
	if (createInfo.flags & BufferFlags::IndexBuffer)
		bufferBindFlag = D3D11_BIND_INDEX_BUFFER;

	if (createInfo.flags & BufferFlags::StructuredBuffer)
	{
		bufferBindFlag = D3D11_BIND_SHADER_RESOURCE;
		bufferMiscFlag |= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		isStructured = true;
	}

	if (createInfo.flags & BufferFlags::GPUWrite)
	{
		bufferUsage = D3D11_USAGE_DEFAULT;
		bufferBindFlag |= D3D11_BIND_UNORDERED_ACCESS;
		bufferMiscFlag |= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		isUAV = true;

		if (createInfo.flags & BufferFlags::GPUCounter)
			isCounter = true;

		if (createInfo.flags & BufferFlags::GPUAppend)
			isAppend = true;
	}
	if (createInfo.flags & BufferFlags::CPURead)
	{
		bufferUsage = D3D11_USAGE_STAGING;
		bufferCPUFlags |= D3D11_CPU_ACCESS_READ;
	}
	if (createInfo.flags & BufferFlags::CPUWrite)
	{
		bufferUsage = D3D11_USAGE_DYNAMIC;
		bufferCPUFlags |= D3D11_CPU_ACCESS_WRITE;
	}

	if (createInfo.flags & BufferFlags::IndirectArgs)
		isIndirect = true;

	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.ByteWidth = static_cast<UINT>(createInfo.size);
	bufferDesc.Usage = bufferUsage;
	bufferDesc.BindFlags = bufferBindFlag;
	bufferDesc.CPUAccessFlags = bufferCPUFlags;
	bufferDesc.MiscFlags = bufferMiscFlag;
	bufferDesc.StructureByteStride = static_cast<UINT>(createInfo.stride);

	D3D11_SUBRESOURCE_DATA bufferData = { 0 };
	bufferData.pSysMem = createInfo.memoryData;

	HRESULT result = rhi::d3dDevice->CreateBuffer(&bufferDesc, (createInfo.memoryData == nullptr) ? nullptr : &bufferData, &buffer->buffer);
	if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateBuffer() failed: ", result));

	buffer->bindFlags = bufferBindFlag;
	buffer->dataBufferSize = static_cast<UINT>(createInfo.size);
	buffer->dataBufferStride = static_cast<UINT>(createInfo.stride);

	const size_t numElements = createInfo.size / createInfo.stride;

	if (isIndirect)
	{
		D3D11_BUFFER_DESC bufferDesc = { 0 };
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.StructureByteStride = 0;
		bufferDesc.ByteWidth = static_cast<UINT>(createInfo.stride);
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
		bufferDesc.BindFlags = 0;

		result = rhi::d3dDevice->CreateBuffer(&bufferDesc, nullptr, &buffer->indirectBuffer);
		if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateBuffer() failed: ", result));
	}
	if (isStructured)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc{};
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		viewDesc.Format = DXGI_FORMAT_UNKNOWN;
		viewDesc.Buffer.ElementWidth = static_cast<UINT>(numElements);

		result = rhi::d3dDevice->CreateShaderResourceView(buffer->buffer.Get(), &viewDesc, &buffer->dataView);
		if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateShaderResourceView() failed: ", result));
	}

	if (isUAV)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC1 uavDesc{};
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.Buffer.NumElements = static_cast<UINT>(numElements);

		if (isCounter) uavDesc.Buffer.Flags |= D3D11_BUFFER_UAV_FLAG_COUNTER;
		if (isAppend)  uavDesc.Buffer.Flags |= D3D11_BUFFER_UAV_FLAG_APPEND;

		result = rhi::d3dDevice->CreateUnorderedAccessView1(buffer->buffer.Get(), &uavDesc, &buffer->dataUAV);
		if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateUnorderedAccessView() failed: ", result));
	}

	return buffer;
}
//=============================================================================
std::expected<rhi::ConstantBufferPtr, std::string> rhi::CreateConstantBuffer(const ConstantBufferCreateInfo& createInfo)
{
	ConstantBufferPtr buffer = std::make_shared<ConstantBuffer>();

	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.ByteWidth = static_cast<UINT>(createInfo.size);
	bufferDesc.Usage = ConvertToD3D11(createInfo.usage);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = ConvertToD3D11(createInfo.cpuAccessFlags);
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = static_cast<UINT>(createInfo.size);

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = createInfo.memoryData;

	HRESULT result = rhi::d3dDevice->CreateBuffer(&bufferDesc, (createInfo.memoryData == nullptr) ? nullptr : &data, &buffer->buffer);
	if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateBuffer() failed: ", result));

	return buffer;
}
//=============================================================================
std::expected<rhi::TexturePtr, std::string> rhi::CreateTexture1D(const Texture1DCreateInfo& createInfo)
{
	TexturePtr texture = std::make_shared<Texture>();
	texture->type = TextureType::Texture1D;

	UINT        bindFlags = D3D11_BIND_SHADER_RESOURCE;
	D3D11_USAGE usageFlags = D3D11_USAGE_DEFAULT; // TODO: а еще есть D3D11_USAGE_IMMUTABLE, сделать
	UINT        cpuAccess = 0;
	bool        isStaging = false;
	bool        isUAV = false;

	if (createInfo.flags & TextureFlags::RenderTarget)
		bindFlags |= D3D11_BIND_RENDER_TARGET;
	if (createInfo.flags & TextureFlags::DepthStencil)
		bindFlags |= D3D11_BIND_DEPTH_STENCIL;

	if (createInfo.flags & TextureFlags::CPURead)
	{
		bindFlags = 0;
		usageFlags = D3D11_USAGE_STAGING;
		cpuAccess = D3D11_CPU_ACCESS_READ;
		isStaging = true;
	}

	if (createInfo.flags & TextureFlags::GPUWrite)
	{
		bindFlags |= D3D11_BIND_UNORDERED_ACCESS;
		isUAV = true;
	}

	D3D11_TEXTURE1D_DESC textureDesc = { 0 };
	textureDesc.Width = createInfo.width;
	textureDesc.MipLevels = createInfo.mipCount;
	textureDesc.ArraySize = 1;
	textureDesc.Format = ConvertToD3D11(createInfo.format);
	textureDesc.Usage = usageFlags;
	textureDesc.BindFlags = bindFlags;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// TODO: доделать загрузку данных при инициализации

	Microsoft::WRL::ComPtr<ID3D11Texture1D> textureRef;
	HRESULT result = rhi::d3dDevice->CreateTexture1D(&textureDesc, nullptr, &textureRef);
	if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateTexture1D() failed: ", result));
	texture->texture = textureRef;

	if (!isStaging)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
		viewDesc.Format = ConvertToD3D11(createInfo.format);
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
		viewDesc.Texture1D.MipLevels = createInfo.mipCount;

		result = rhi::d3dDevice->CreateShaderResourceView(texture->texture.Get(), nullptr, &texture->dataView);
		if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateShaderResourceView() failed: ", result));
	}

	if (isUAV)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC1 uavDesc = {};
		uavDesc.Format = ConvertToD3D11(createInfo.format);
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1D;
		uavDesc.Texture1D.MipSlice = 0;

		result = rhi::d3dDevice->CreateUnorderedAccessView1(texture->texture.Get(), &uavDesc, &texture->dataUAV);
		if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateUnorderedAccessView1() failed: ", result));
	}

	return texture;
}
//=============================================================================
std::expected<rhi::TexturePtr, std::string> rhi::CreateTexture2D(const Texture2DCreateInfo& createInfo)
{
	TexturePtr texture = std::make_shared<Texture>();
	texture->type = TextureType::Texture2D;

	UINT        bindFlags = D3D11_BIND_SHADER_RESOURCE;
	D3D11_USAGE usageFlags = D3D11_USAGE_DEFAULT; // TODO: а еще есть D3D11_USAGE_IMMUTABLE, сделать
	UINT        cpuAccess = 0;
	bool        isStaging = false;
	bool        isUAV = false;

	if (createInfo.flags & TextureFlags::RenderTarget)
		bindFlags |= D3D11_BIND_RENDER_TARGET;
	if (createInfo.flags & TextureFlags::DepthStencil)
		bindFlags |= D3D11_BIND_DEPTH_STENCIL;

	if (createInfo.flags & TextureFlags::CPURead)
	{
		bindFlags = 0;
		usageFlags = D3D11_USAGE_STAGING;
		cpuAccess = D3D11_CPU_ACCESS_READ;
		isStaging = true;
	}

	if (createInfo.flags & TextureFlags::GPUWrite)
	{
		bindFlags |= D3D11_BIND_UNORDERED_ACCESS;
		isUAV = true;
	}

	DXGI_FORMAT dataFormat = ConvertToD3D11(createInfo.format);

	DXGI_FORMAT textureFormat = dataFormat;
	DXGI_FORMAT viewFormat = dataFormat;

	if (createInfo.format == TexelsFormat::D16)
	{
		textureFormat = DXGI_FORMAT_R16_TYPELESS;
		viewFormat = DXGI_FORMAT_R16_UNORM;
	}

	if (createInfo.format == TexelsFormat::D24S8)
	{
		// TODO: X8 part of the texture is not accessible ATM, try to handle this properly
		textureFormat = DXGI_FORMAT_R24G8_TYPELESS;
		viewFormat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	}

	if (createInfo.format == TexelsFormat::D32F)
	{
		textureFormat = DXGI_FORMAT_R32_TYPELESS;
		viewFormat = DXGI_FORMAT_R32_FLOAT;
	}

	D3D11_TEXTURE2D_DESC1 textureDesc = { 0 };
	textureDesc.Width = createInfo.width;
	textureDesc.Height = createInfo.height;
	textureDesc.MipLevels = createInfo.mipCount;
	textureDesc.ArraySize = 1;
	textureDesc.Format = textureFormat;
	textureDesc.SampleDesc = { 1, 0 };
	textureDesc.Usage = usageFlags;
	textureDesc.BindFlags = bindFlags;
	textureDesc.CPUAccessFlags = cpuAccess;
	textureDesc.MiscFlags = 0;

	//D3D11_SUBRESOURCE_DATA textureData{};
	//textureData.pSysMem = createInfo.memoryData;
	//textureData.SysMemPitch = 2 * sizeof(UINT); // texture is 2 pixels wide, 4 bytes per pixel // TODO: доделать
	// TODO: доделать загрузку данных при инициализации

	Microsoft::WRL::ComPtr<ID3D11Texture2D1> textureRef;
	HRESULT result = rhi::d3dDevice->CreateTexture2D1(&textureDesc, nullptr, &textureRef);
	if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateTexture2D1() failed: ", result));
	texture->texture = textureRef;

	if (!isStaging)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
		viewDesc.Format = viewFormat;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MipLevels = createInfo.mipCount;

		result = rhi::d3dDevice->CreateShaderResourceView(texture->texture.Get(), &viewDesc, &texture->dataView);
		if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateShaderResourceView() failed: ", result));
	}

	if (isUAV)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC1 uavDesc = {};
		uavDesc.Format = ConvertToD3D11(createInfo.format);
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;

		result = rhi::d3dDevice->CreateUnorderedAccessView1(texture->texture.Get(), &uavDesc, &texture->dataUAV);
		if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateUnorderedAccessView1() failed: ", result));
	}

	return texture;
}
//=============================================================================
std::expected<rhi::TexturePtr, std::string> rhi::CreateTexture3D(const Texture3DCreateInfo& createInfo)
{
	TexturePtr texture = std::make_shared<Texture>();
	texture->type = TextureType::Texture1D;

	UINT        bindFlags = D3D11_BIND_SHADER_RESOURCE;
	D3D11_USAGE usageFlags = D3D11_USAGE_DEFAULT;
	UINT        cpuAccess = 0;
	bool        isStaging = false;
	bool        isUAV = false;

	if (createInfo.flags & TextureFlags::RenderTarget)
		bindFlags |= D3D11_BIND_RENDER_TARGET;
	if (createInfo.flags & TextureFlags::DepthStencil)
		bindFlags |= D3D11_BIND_DEPTH_STENCIL;

	if (createInfo.flags & TextureFlags::CPURead)
	{
		bindFlags = 0;
		usageFlags = D3D11_USAGE_STAGING;
		cpuAccess = D3D11_CPU_ACCESS_READ;
		isStaging = true;
	}

	if (createInfo.flags & TextureFlags::GPUWrite)
	{
		bindFlags |= D3D11_BIND_UNORDERED_ACCESS;
		isUAV = true;
	}

	D3D11_TEXTURE3D_DESC1 textureDesc = { 0 };
	textureDesc.Width = createInfo.width;
	textureDesc.Height = createInfo.height;
	textureDesc.Depth = createInfo.depth;
	textureDesc.MipLevels = createInfo.mipCount;
	textureDesc.Format = ConvertToD3D11(createInfo.format);
	textureDesc.Usage = usageFlags;
	textureDesc.BindFlags = bindFlags;
	textureDesc.CPUAccessFlags = cpuAccess;
	textureDesc.MiscFlags = 0;

	// TODO: доделать загрузку данных при инициализации

	Microsoft::WRL::ComPtr<ID3D11Texture3D1> textureRef;
	HRESULT result = rhi::d3dDevice->CreateTexture3D1(&textureDesc, nullptr, &textureRef);
	if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateTexture3D1() failed: ", result));
	texture->texture = textureRef;

	if (!isStaging)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
		viewDesc.Format = ConvertToD3D11(createInfo.format);
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
		viewDesc.Texture3D.MipLevels = createInfo.mipCount;

		result = rhi::d3dDevice->CreateShaderResourceView(texture->texture.Get(), nullptr, &texture->dataView);
		if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateShaderResourceView() failed: ", result));
	}

	if (isUAV)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC1 uavDesc = {};
		uavDesc.Format = ConvertToD3D11(createInfo.format);
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;

		result = rhi::d3dDevice->CreateUnorderedAccessView1(texture->texture.Get(), &uavDesc, &texture->dataUAV);
		if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateUnorderedAccessView1() failed: ", result));
	}

	return texture;
}
//=============================================================================
std::expected<rhi::RenderTargetPtr, std::string> rhi::CreateRenderTarget(const RenderTargetCreateInfo& createInfo)
{
	// TODO: проверка на валидацию данных в createInfo

	RenderTargetPtr rt = std::make_shared<RenderTarget>();

	rt->numRenderTargets = createInfo.rtSlotCount;
	rt->width = createInfo.width;
	rt->height = createInfo.height;

	for (size_t i = 0; i < rt->numRenderTargets; i++)
	{
		Texture2DCreateInfo tci;
		tci.width = createInfo.width;
		tci.height = createInfo.height;
		tci.format = createInfo.colorBuffers[i].format;
		tci.flags = TextureFlags::RenderTarget;
		auto resource = CreateTexture2D(tci);
		if (!resource.has_value())
			return std::unexpected(resource.error());
		rt->renderTargetTextures[i] = resource.value();

		D3D11_RENDER_TARGET_VIEW_DESC1 rtDesc{};
		rtDesc.Format = DXGI_FORMAT_UNKNOWN;
		rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		HRESULT result = rhi::d3dDevice->CreateRenderTargetView1(rt->renderTargetTextures[i]->texture.Get(), &rtDesc, rt->renderTargetViews[i].GetAddressOf());
		if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateRenderTargetView1() failed: ", result));
	}

	if (createInfo.depthFormat > TexelsFormat::DepthNone)
	{
		Texture2DCreateInfo tci;
		tci.width = createInfo.width;
		tci.height = createInfo.height;
		tci.format = createInfo.depthFormat;
		tci.flags = TextureFlags::DepthStencil;
		auto resource = CreateTexture2D(tci);
		if (!resource.has_value())
			return std::unexpected(resource.error());
		rt->depthStencilTexture = resource.value();

		D3D11_SHADER_RESOURCE_VIEW_DESC depthTextureDesc;
		rt->depthStencilTexture->dataView->GetDesc(&depthTextureDesc);

		DXGI_FORMAT depthFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		switch (depthTextureDesc.Format)
		{
		case DXGI_FORMAT_R16_UNORM:
			depthFormat = DXGI_FORMAT_D16_UNORM; break;
		case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
			depthFormat = DXGI_FORMAT_D24_UNORM_S8_UINT; break;
		case DXGI_FORMAT_R32_FLOAT:
			depthFormat = DXGI_FORMAT_D32_FLOAT; break;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc{};
		dsDesc.Format = depthFormat;
		dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		HRESULT result = rhi::d3dDevice->CreateDepthStencilView(rt->depthStencilTexture->texture.Get(), &dsDesc, rt->depthStencilView.GetAddressOf());
		if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateDepthStencilView() failed: ", result));
	}

	return rt;
}
//=============================================================================
void rhi::ClearBufferRW(BufferPtr buffer, uint32_t value)
{
	::ClearRW(buffer->dataUAV.Get(), value);
}
//=============================================================================
void rhi::ClearBufferRW(BufferPtr buffer, float value)
{
	::ClearRW(buffer->dataUAV.Get(), value);
}
//=============================================================================
void* rhi::Map(BufferPtr buffer, MapType type)
{
	return ::Map(buffer->buffer.Get(), type);
}
//=============================================================================
void rhi::Unmap(BufferPtr buffer)
{
	rhi::d3dContext->Unmap(buffer->buffer.Get(), 0);
}
//=============================================================================
void rhi::UpdateBuffer(BufferPtr buffer, const void* mem)
{
	rhi::d3dContext->UpdateSubresource(buffer->buffer.Get(), 0, nullptr, mem, 0, 0);
}
//=============================================================================
void rhi::CopyBufferData(BufferPtr buffer, size_t offset, size_t size, const void* mem)
{
	UpdateSubresource(buffer->buffer.Get(), mem, 0, offset, size, 0, 1, 0, 1, 0, 0);
}
//=============================================================================
void rhi::CopyResource(BufferPtr src, BufferPtr dst)
{
	rhi::d3dContext->CopyResource(dst->buffer.Get(), src->buffer.Get());
}
//=============================================================================
void* rhi::Map(ConstantBufferPtr buffer, MapType type)
{
	return ::Map(buffer->buffer.Get(), type);
}
//=============================================================================
void rhi::Unmap(ConstantBufferPtr buffer)
{
	rhi::d3dContext->Unmap(buffer->buffer.Get(), 0);
}
//=============================================================================
void rhi::UpdateBuffer(ConstantBufferPtr buffer, const void* mem)
{
	rhi::d3dContext->UpdateSubresource(buffer->buffer.Get(), 0, nullptr, mem, 0, 0);
}
//=============================================================================
void rhi::CopyResource(ConstantBufferPtr src, ConstantBufferPtr dst)
{
	rhi::d3dContext->CopyResource(dst->buffer.Get(), src->buffer.Get());
}
//=============================================================================
void rhi::ClearTextureRW(TexturePtr texture, uint32_t value)
{
	::ClearRW(texture->dataUAV.Get(), value);
}
//=============================================================================
void rhi::ClearTextureRW(TexturePtr texture, float value)
{
	::ClearRW(texture->dataUAV.Get(), value);
}
//=============================================================================
void* rhi::Map(TexturePtr handle, MapType type)
{
	return ::Map(handle->texture.Get(), type);
}
//=============================================================================
void rhi::Unmap(TexturePtr handle)
{
	rhi::d3dContext->Unmap(handle->texture.Get(), 0);
}
//=============================================================================
void rhi::UpdateTexture(TexturePtr handle, const void* mem, uint32_t mip, size_t offsetX, size_t sizeX, size_t offsetY, size_t sizeY, size_t offsetZ, size_t sizeZ, size_t rowPitch, size_t depthPitch)
{
	UpdateSubresource(handle->texture.Get(), mem, mip, offsetX, sizeX, offsetY, sizeY, offsetZ, sizeZ, rowPitch, depthPitch);
}
//=============================================================================
void rhi::CopyResource(TexturePtr src, TexturePtr dst)
{
	rhi::d3dContext->CopyResource(dst->texture.Get(), src->texture.Get());
}
//=============================================================================
void rhi::BindShaderProgram(ShaderProgramPtr resource)
{
	if (!resource) [[unlikely]] return;

	rhi::d3dContext->IASetInputLayout(resource->inputLayout.Get());

	rhi::d3dContext->VSSetShader(resource->vertexShader.Get(), nullptr, 0);
	rhi::d3dContext->PSSetShader(resource->pixelShader.Get(), nullptr, 0);
	rhi::d3dContext->HSSetShader(resource->hullShader.Get(), nullptr, 0);
	rhi::d3dContext->DSSetShader(resource->domainShader.Get(), nullptr, 0);
	rhi::d3dContext->GSSetShader(resource->geometryShader.Get(), nullptr, 0);
}
//=============================================================================
void rhi::BindPipelineState(PipelineStatePtr resource)
{
	rhi::d3dContext->RSSetState(resource->rasterizerState.Get());
	rhi::d3dContext->OMSetBlendState(resource->blendState.Get(), nullptr, 0xffffffff);
	rhi::d3dContext->OMSetDepthStencilState(resource->depthStencilState.Get(), resource->stencilRef);
}
//=============================================================================
void rhi::BindSamplerState(SamplerStatePtr resource, ShaderType shaderType, uint32_t slot)
{
	switch (shaderType)
	{
	case ShaderType::Vertex:
		rhi::d3dContext->VSSetSamplers(slot, 1, resource->state.GetAddressOf());
		break;
	case ShaderType::Pixel:
		rhi::d3dContext->PSSetSamplers(slot, 1, resource->state.GetAddressOf());
		break;
	case ShaderType::Hull:
		rhi::d3dContext->HSSetSamplers(slot, 1, resource->state.GetAddressOf());
		break;
	case ShaderType::Domain:
		rhi::d3dContext->DSSetSamplers(slot, 1, resource->state.GetAddressOf());
		break;
	case ShaderType::Geometry:
		rhi::d3dContext->GSSetSamplers(slot, 1, resource->state.GetAddressOf());
		break;
	case ShaderType::Compute:
		rhi::d3dContext->CSSetSamplers(slot, 1, resource->state.GetAddressOf());
		break;
	default:
		break;
	}
}
//=============================================================================
void rhi::BindConstantBuffer(ConstantBufferPtr resource, ShaderType shaderType, uint32_t slot)
{
	switch (shaderType)
	{
	case ShaderType::Vertex:
		rhi::d3dContext->VSSetConstantBuffers(slot, 1, resource->buffer.GetAddressOf());
		break;
	case ShaderType::Pixel:
		rhi::d3dContext->PSSetConstantBuffers(slot, 1, resource->buffer.GetAddressOf());
		break;
	case ShaderType::Hull:
		rhi::d3dContext->HSSetConstantBuffers(slot, 1, resource->buffer.GetAddressOf());
		break;
	case ShaderType::Domain:
		rhi::d3dContext->DSSetConstantBuffers(slot, 1, resource->buffer.GetAddressOf());
		break;
	case ShaderType::Geometry:
		rhi::d3dContext->GSSetConstantBuffers(slot, 1, resource->buffer.GetAddressOf());
		break;
	case ShaderType::Compute:
		rhi::d3dContext->CSSetConstantBuffers(slot, 1, resource->buffer.GetAddressOf());
		break;
	default:
		break;
	}
}
//=============================================================================
void rhi::BindShaderResource(BufferPtr resource, ShaderType shaderType, uint32_t slot)
{
	SetShaderResource(resource->dataView.GetAddressOf(), shaderType, slot);
}
//=============================================================================
void rhi::BindShaderResource(TexturePtr resource, ShaderType shaderType, uint32_t slot)
{
	SetShaderResource(resource->dataView.GetAddressOf(), shaderType, slot);
}
//=============================================================================
void rhi::BindVertexBuffer(BufferPtr resource, uint32_t slot)
{
	uint32_t& id = stateCache::currentFreeVertexBuffer;

	id = slot;
	stateCache::vertexBuffer[id] = resource->buffer.Get();
	stateCache::vertexBufferStride[id] = resource->dataBufferStride;
	stateCache::vertexBufferOffset[id] = 0;

	rhi::d3dContext->IASetVertexBuffers(
		slot, 1,
		stateCache::vertexBuffer,
		stateCache::vertexBufferStride,
		stateCache::vertexBufferOffset);
}
//=============================================================================
void rhi::BindVertexBuffers(const std::vector<BufferPtr>& resources)
{
	uint32_t& id = stateCache::currentFreeVertexBuffer;

	id = 0;
	for (size_t i = 0; i < resources.size(); i++)
	{
		stateCache::vertexBuffer[id] = resources[i]->buffer.Get();
		stateCache::vertexBufferStride[id] = resources[i]->dataBufferStride;
		stateCache::vertexBufferOffset[id] = 0;

		id++;
		assert(id < MaxVertexBuffers);
	}
	rhi::d3dContext->IASetVertexBuffers(
		0, id,
		stateCache::vertexBuffer,
		stateCache::vertexBufferStride,
		stateCache::vertexBufferOffset);
}
//=============================================================================
void rhi::BindIndexBuffer(BufferPtr resource)
{
	rhi::d3dContext->IASetIndexBuffer(resource->buffer.Get(), DXGI_FORMAT_R32_UINT, 0); // TODO: DXGI_FORMAT_R32_UINT должно передаваться
}
//=============================================================================
void rhi::SetRenderTarget(RenderTargetPtr rt, const std::optional<Color>& clearColor, const std::optional<float>& clearDepth, const std::optional<uint8_t>& clearStencil)
{
	if (!rt) [[unlikely]]
	{
		SetMainFrame(clearColor, clearDepth, clearStencil);
		return;
	}

	ID3D11RenderTargetView* rtv[RenderTargetSlotCount] = { nullptr };
	for (size_t i = 0; i < rt->numRenderTargets; i++)
	{
		rtv[i] = rt->renderTargetViews[i].Get();

		if (clearColor.has_value() && rtv[i])
		{
			rhi::d3dContext->ClearRenderTargetView(rtv[i], clearColor.value().Get());
		}
	}

	ID3D11DepthStencilView* dsv = rt->depthStencilView.Get();
	if (clearDepth.has_value() && dsv)
	{
		uint8_t stencil = clearStencil.has_value() ? clearStencil.value() : 0;
		rhi::d3dContext->ClearDepthStencilView(rt->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, clearDepth.value(), stencil);
		// TODO: переделать так чтобы можно было очищать что-то одно - дефбуфер или стенсил
	}

	rhi::d3dContext->OMSetRenderTargets(rt->numRenderTargets, rtv, dsv);
}
//=============================================================================