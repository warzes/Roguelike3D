#pragma once

//=============================================================================
#define DX_ERR_STR(str,hr) std::string(str + std::string(HRToString(hr)) + ".\n\tFile: " + std::string(__FILE__) + ", Line: " + std::to_string(__LINE__))
#define DX_ERR(str,hr) Fatal(DX_ERR_STR(str, hr))
//=============================================================================

//=============================================================================
// Constants
//=============================================================================

//=============================================================================

//=============================================================================
// Enums
//=============================================================================
enum CreateRHIFlag
{
	rhi_vsync = 1 << 0,
};
typedef uint32_t CreateRHIFlags;
//=============================================================================
inline DXGI_FORMAT ConvertToD3D11(DataFormat format)
{
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