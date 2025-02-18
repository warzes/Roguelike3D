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
inline D3D11_FILL_MODE ConvertToD3D11(FillMode fillMode)
{
	switch (fillMode)
	{
	case FillMode::Solid:     return D3D11_FILL_SOLID;
	case FillMode::Wireframe: return D3D11_FILL_WIREFRAME;
	}
}
//=============================================================================
inline D3D11_CULL_MODE ConvertToD3D11(CullMode cullMode)
{
	switch (cullMode)
	{
	case CullMode::Back:  return D3D11_CULL_BACK;
	case CullMode::Front: return D3D11_CULL_FRONT;
	case CullMode::None:  return D3D11_CULL_NONE;
	}
}
//=============================================================================
inline D3D11_FILTER ConvertToD3D11(TextureFilter filter)
{
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
inline D3D11_TEXTURE_ADDRESS_MODE ConvertToD3D11(AddressMode mode)
{
	switch (mode)
	{
	case AddressMode::Wrap:   return D3D11_TEXTURE_ADDRESS_WRAP;
	case AddressMode::Mirror: return D3D11_TEXTURE_ADDRESS_MIRROR;
	case AddressMode::Clamp:  return D3D11_TEXTURE_ADDRESS_CLAMP;
	case AddressMode::Border: return D3D11_TEXTURE_ADDRESS_BORDER;
	default: return D3D11_TEXTURE_ADDRESS_WRAP;
	}
}
//=============================================================================
inline D3D11_COMPARISON_FUNC ConvertToD3D11(ComparisonFunc func)
{
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
inline D3D11_DEPTH_WRITE_MASK ConvertToD3D11(DepthWriteMask mask)
{
	switch (mask)
	{
	case DepthWriteMask::Zero: return D3D11_DEPTH_WRITE_MASK_ZERO;
	case DepthWriteMask::All:  return D3D11_DEPTH_WRITE_MASK_ALL;
	default:                   return D3D11_DEPTH_WRITE_MASK_ZERO;
	}
}
//=============================================================================
inline D3D11_STENCIL_OP ConvertToD3D11(StencilOp op)
{
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
inline D3D11_USAGE ConvertToD3D11(BufferUsage usage)
{
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
inline UINT ConvertToD3D11(CPUAccessFlags flags)
{
	switch (flags)
	{
	case CPUAccessFlags::None:      return 0;
	case CPUAccessFlags::Write:     return D3D11_CPU_ACCESS_WRITE;
	case CPUAccessFlags::Read:      return D3D11_CPU_ACCESS_READ;
	case CPUAccessFlags::WriteRead: return D3D11_CPU_ACCESS_WRITE |D3D11_CPU_ACCESS_READ;
	default:                        return 0;
	}
}
//=============================================================================
inline D3D11_MAP ConvertToD3D11(MapType type)
{
	switch (type)
	{
	case MapType::Read:  return D3D11_MAP_READ;
	case MapType::Write: return D3D11_MAP_WRITE_DISCARD;
	default:             return D3D11_MAP_READ; // Значение по умолчанию
	}
}
//=============================================================================
inline D3D11_BLEND ConvertToD3D11(BlendFactor type)
{
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
	default: return D3D11_BLEND_ONE;
	}
}
//=============================================================================
inline D3D11_BLEND_OP ConvertToD3D11(BlendOp type)
{
	switch (type)
	{
	case BlendOp::Add:         return D3D11_BLEND_OP_ADD;
	case BlendOp::Subtract:    return D3D11_BLEND_OP_SUBTRACT;
	case BlendOp::RevSubtract: return D3D11_BLEND_OP_REV_SUBTRACT;
	case BlendOp::Min:         return D3D11_BLEND_OP_MIN;
	case BlendOp::Max:         return D3D11_BLEND_OP_MAX;
	default: return D3D11_BLEND_OP_ADD;
	}
}
//=============================================================================