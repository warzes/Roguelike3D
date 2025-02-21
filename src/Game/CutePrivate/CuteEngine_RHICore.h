#pragma once

//=============================================================================
#define DX_ERR_STR(str,hr) std::string(str + std::string(HRToString(hr)) + ".\n\tFile: " + std::string(__FILE__) + ", Line: " + std::to_string(__LINE__))
#define DX_ERR(str,hr) Fatal(DX_ERR_STR(str, hr))
//=============================================================================

//=============================================================================
// Constants
//=============================================================================
constexpr size_t MaxVertexBuffers = 16;

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
DXGI_FORMAT ConvertToD3D11(TexelsFormat format)
{
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
	case TexelsFormat::D16:               return DXGI_FORMAT_D16_UNORM;        // D16
	case TexelsFormat::D24S8:             return DXGI_FORMAT_D24_UNORM_S8_UINT;// D24S8
	case TexelsFormat::D32F:              return DXGI_FORMAT_D32_FLOAT;        // D32F
	default:                              return DXGI_FORMAT_UNKNOWN;
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
inline D3D11_PRIMITIVE_TOPOLOGY ConvertToD3D11(PrimitiveTopology topology)
{
	switch (topology)
	{
	case PrimitiveTopology::TriangleList:  return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	case PrimitiveTopology::TriangleStrip: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	case PrimitiveTopology::PointList:     return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	default: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}
}
//=============================================================================