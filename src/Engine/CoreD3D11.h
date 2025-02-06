#pragma once

//=============================================================================
// Constants
//=============================================================================

constexpr const int RenderTargetSlotCount = 8;

//=============================================================================
// enums
//=============================================================================

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

enum class TexelsFormat : uint8_t 
{
	R_U8,     // An 8 bits per pixel red channel texture format.
	R_U16,    // A 16 bits per pixel red channel texture format.
	RG_U8,    // An 8 bits per pixel red and green channel texture format.
	RG_U16,   // A 16 bits per pixel red and green channel texture format.
	RGB_U8,   // An 8 bits per pixel red, green, and blue channel texture format.
	RGBA_U8,  // An 8 bits per pixel red, green, blue, and alpha channel texture format.
	RGBA_U16, // A 16 bits per pixel red, green, blue, and alpha channel texture format.

	Depth_U16,
	DepthStencil_U16,
	Depth_U24,
	DepthStencil_U24, // A format to be used with the depth and stencil buffers where the depth buffer gets 24 bits and the stencil buffer gets 8 bits.
};

enum class TextureSamplerFilter : uint8_t
{
	MinMagLinear,
	MinMagNearest,
	MinLinearMagNearest,
	MinNearestMagLinear,
	MinTrilinearMagLinear,
	MinTrilinearMagNearest
};

enum class TextureSamplerWrap : uint8_t
{
	Repeat,
	Mirror,
	Clamp,
	BorderColor
};

//=============================================================================
// core utils
//=============================================================================

std::string HrToString(HRESULT hr);

//=============================================================================
// Pipeline State
//=============================================================================

enum class FillMode : uint8_t
{
	Solid,
	Wireframe,

	Count
};

enum class CullMode : uint8_t
{
	Back,
	Front,
	None,

	Count
};

enum class CounterDirection : size_t
{
	CW,
	CCW,

	Count
};

enum class ColorWriteMask : uint8_t
{
	Red = (1U << 0),
	Green = (1U << 1),
	Blue = (1U << 2),
	Alpha = (1U << 3),

	All = Red | Green | Blue | Alpha
};

enum class BlendFactor : uint8_t
{
	Zero,
	One,
	SrcAlpha,
	DstAlpha,
	OneMinusSrcAlpha,
	OneMinusDstAlpha,
	SrcColor,
	DstColor,
	OneMinusSrcColor,
	OneMinusDstColor,

	Count
};

enum class BlendOp : uint8_t
{
	Add,
	Subtract,
	RevSubtract,
	Min,
	Max,

	Count
};

enum class DepthWriteMask : uint8_t
{
	Zero,
	All,

	Count
};

enum class ComparisonFunc : size_t
{
	Always,
	Never,
	Less,
	LessEqual,
	Greater,
	GreaterEqual,
	Equal,
	NotEqual,

	Count
};
typedef ComparisonFunc DepthFunc;
typedef ComparisonFunc StencilFunc;
typedef ComparisonFunc SamplerFunc;

enum class StencilOp : size_t
{
	Keep,
	Zero,
	Replace,
	Increment,
	Decrement,

	Count
};

struct RasterizerState final
{
	FillMode         fillMode = FillMode::Solid;
	CullMode         cullMode = CullMode::Back;
	CounterDirection counterDirection = CounterDirection::CCW;
};

struct BlendDesc final
{
	bool           blendEnabled = false;
	ColorWriteMask writeMask = ColorWriteMask::All;
	BlendFactor    srcBlend = BlendFactor::One;
	BlendFactor    dstBlend = BlendFactor::Zero;
	BlendOp        blendOp = BlendOp::Add;
	BlendFactor    srcBlendAlpha = BlendFactor::One;
	BlendFactor    dstBlendAlpha = BlendFactor::Zero;
	BlendOp        blendOpAlpha = BlendOp::Add;
};

struct BlendState final
{
	BlendDesc blendDesc;

	bool      separateBlendEnabled = false;
	BlendDesc renderTargetBlendDesc[RenderTargetSlotCount];

	bool      alphaToCoverageEnabled = false;
};

struct StencilDesc final
{
	StencilFunc stencilFunc = StencilFunc::Always;
	StencilOp   failOp = StencilOp::Keep;
	StencilOp   depthFailOp = StencilOp::Keep;
	StencilOp   passOp = StencilOp::Keep;
};

struct DepthStencilState final
{
	bool           depthEnabled = true;
	DepthWriteMask writeMask = DepthWriteMask::All;
	DepthFunc      depthFunc = DepthFunc::Less;

	bool           stencilEnabled = false;
	uint32_t       stencilRef;
	uint8_t        stencilReadMask; // not implemented
	uint8_t        stencilWriteMask;
	StencilDesc    frontFaceStencilDesc;
	StencilDesc    backFaceStencilDesc;
};

struct PipelineStateDescriptor final
{
	RasterizerState     rasterizerState;
	BlendState          blendState;
	DepthStencilState   depthStencilState;
	//SurfaceShaderHandle shader;
	//VertexFormatHandle  vertexFormat;
};