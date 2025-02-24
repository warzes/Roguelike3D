#pragma once

//=============================================================================
#pragma region [ DOC ]

/*
DOC:
...
*/

/*
TODO:
...
*/

/*
HISTORY:

Version 0.0.1
- Основные файлы
- Главный класс приложения CuteEngineApp
- Создание и управление окном
- Инициализация и управление контекстом Direct3D11
- Основные ресурсы Direct3D11
*/

#pragma endregion
//=============================================================================
#pragma region [ Config Engine ]

#pragma endregion
//=============================================================================
#pragma region [ Constant Engine ]

constexpr const auto CuteEngineVersion = "0.0.1";

#pragma endregion
//=============================================================================
#pragma region [ Header ]

#define _USE_MATH_DEFINES

#include <cstdint>
#include <optional>
#include <expected>
#include <memory>
#include <string_view>
#include <string>
#include <vector>

#pragma endregion
//=============================================================================
#pragma region [ Base ]

void Print(const std::string& message);
void Print(const std::wstring& message);
void Fatal(const std::string& error);

struct Color final
{
	const float* Get() const { return &r; }

	float r{ 1.0f };
	float g{ 1.0f };
	float b{ 1.0f };
	float a{ 1.0f };
};

struct Viewport final
{
	float TopLeftX{ 0.0f };
	float TopLeftY{ 0.0f };
	float Width{ 0.0f };
	float Height{ 0.0f };
	float MinDepth{ 0.0f };
	float MaxDepth{ 1.0f };
};

#pragma endregion
//=============================================================================
#pragma region [ Input ]

namespace Input
{
	enum MouseButton
	{
		MOUSE_BUTTON_1 = 0,
		MOUSE_BUTTON_2 = 1,
		MOUSE_BUTTON_3 = 2,
		MOUSE_BUTTON_4 = 3,
		MOUSE_BUTTON_5 = 4,
		MOUSE_BUTTON_6 = 5,
		MOUSE_BUTTON_7 = 6,
		MOUSE_BUTTON_8 = 7,

		MOUSE_BUTTON_LAST = MOUSE_BUTTON_8,
		MOUSE_BUTTON_LEFT = MOUSE_BUTTON_1,
		MOUSE_BUTTON_RIGHT = MOUSE_BUTTON_2,
		MOUSE_BUTTON_MIDDLE = MOUSE_BUTTON_3,

		MOUSE_BUTTONS_MAX = 8,
	};

	enum Key
	{
		UNKNOWN = -1,

		KEY_SPACE = 32,
		KEY_APOSTROPHE = 39,
		KEY_COMMA = 44,
		KEY_MINUS = 45,
		KEY_PERIOD = 46,
		KEY_SLASH = 47,
		KEY_ZERO = 48,
		KEY_1 = 49,
		KEY_2 = 50,
		KEY_3 = 51,
		KEY_4 = 52,
		KEY_5 = 53,
		KEY_6 = 54,
		KEY_7 = 55,
		KEY_8 = 56,
		KEY_9 = 57,
		KEY_SEMICOLON = 59,
		KEY_EQUAL = 61,
		KEY_A = 65,
		KEY_B = 66,
		KEY_C = 67,
		KEY_D = 68,
		KEY_E = 69,
		KEY_F = 70,
		KEY_G = 71,
		KEY_H = 72,
		KEY_I = 73,
		KEY_J = 74,
		KEY_K = 75,
		KEY_L = 76,
		KEY_M = 77,
		KEY_N = 78,
		KEY_O = 79,
		KEY_P = 80,
		KEY_Q = 81,
		KEY_R = 82,
		KEY_S = 83,
		KEY_T = 84,
		KEY_U = 85,
		KEY_V = 86,
		KEY_W = 87,
		KEY_X = 88,
		KEY_Y = 89,
		KEY_Z = 90,
		KEY_LEFT_BRACKET = 91,
		KEY_BACKSLASH = 92,
		KEY_RIGHT_BRACKET = 93,
		KEY_GRAVE_ACCENT = 96,
		KEY_WORLD_1 = 161,
		KEY_WORLD_2 = 162,

		KEY_ESCAPE = 256,
		KEY_ENTER = 257,
		KEY_TAB = 258,
		KEY_BACKSPACE = 259,
		KEY_INSERT = 260,
		KEY_DELETE = 261,
		KEY_RIGHT = 262,
		KEY_LEFT = 263,
		KEY_DOWN = 264,
		KEY_UP = 265,
		KEY_PAGE_UP = 266,
		KEY_PAGE_DOW = 267,
		KEY_HOME = 268,
		KEY_END = 269,
		KEY_CAPS_LOCK = 280,
		KEY_SCROLL_LOCK = 281,
		KEY_NUM_LOCK = 282,
		KEY_PRINT_SCREEN = 283,
		KEY_PAUSE = 284,

		KEY_F1 = 290,
		KEY_F2 = 291,
		KEY_F3 = 292,
		KEY_F4 = 293,
		KEY_F5 = 294,
		KEY_F6 = 295,
		KEY_F7 = 296,
		KEY_F8 = 297,
		KEY_F9 = 298,
		KEY_F10 = 299,
		KEY_F11 = 300,
		KEY_F12 = 301,
		KEY_F13 = 302,
		KEY_F14 = 303,
		KEY_F15 = 304,
		KEY_F16 = 305,
		KEY_F17 = 306,
		KEY_F18 = 307,
		KEY_F19 = 308,
		KEY_F20 = 309,
		KEY_F21 = 310,
		KEY_F22 = 311,
		KEY_F23 = 312,
		KEY_F24 = 313,
		KEY_F25 = 314,

		KEY_KP_0 = 320,
		KEY_KP_1 = 321,
		KEY_KP_2 = 322,
		KEY_KP_3 = 323,
		KEY_KP_4 = 324,
		KEY_KP_5 = 325,
		KEY_KP_6 = 326,
		KEY_KP_7 = 327,
		KEY_KP_8 = 328,
		KEY_KP_9 = 329,
		KEY_KP_DECIMAL = 330,
		KEY_KP_DIVIDE = 331,
		KEY_KP_MULTIPLY = 332,
		KEY_KP_SUBTRACT = 333,
		KEY_KP_ADD = 334,
		KEY_KP_ENTER = 335,
		KEY_KP_EQUAL = 336,

		KEY_LEFT_SHIFT = 340,
		KEY_LEFT_CONTROL = 341,
		KEY_LEFT_ALT = 342,
		KEY_LEFT_SUPER = 343,
		KEY_RIGHT_SHIFT = 344,
		KEY_RIGHT_CONTROL = 345,
		KEY_RIGHT_ALT = 346,
		KEY_RIGHT_SUPER = 347,
		KEY_MENU = 348,

		KEYS_MAX = 349
	};

} // namespace Input

#pragma endregion
//=============================================================================
#pragma region [ RHI Core ]

constexpr const auto RenderTargetSlotCount = 8u;
constexpr const auto MaxVertexBuffers = 16u;
constexpr const auto AppendAlignedElement = 0xffffffff;

#pragma region [ RHI Enum ]

enum class DataFormat : uint8_t
{
	R1, R8, R16, R16F, R32I, R32U, R32F,               // R
	RG8, RG16, RG16F, RG32I, RG32U, RG32F,             // RG
	RGB32I, RGB32U, RGB32F,                            // RGB
	RGBA8, RGBA16, RGBA16F, RGBA32I, RGBA32U, RGBA32F, // RGBA
	R11G11B10F
};

enum class TexelsFormat : uint8_t
{
	BC1,    // DXT1
	BC2,    // DXT3
	BC3,    // DXT5
	BC4,    // LATC1/ATI1
	BC5,    // LATC2/ATI2
	BC6H,   // BC6H
	BC7,    // BC7

	UnknownCompressed, // compressed formats above

	R1, R8, R16, R16F, R32I, R32U, R32F,               // R
	RG8, RG16, RG16F, RG32I, RG32U, RG32F,             // RG
	RGB32I, RGB32U, RGB32F,                            // RGB
	RGBA8, RGBA16, RGBA16F, RGBA32I, RGBA32U, RGBA32F, // RGBA
	R11G11B10F,

	UnknownDepth, // depth formats below

	D16,
	D24S8,
	D32F,
};

enum class TextureType : uint8_t
{
	Texture1D,
	Texture2D,
	Texture3D,
};

enum class ShaderType : uint8_t
{
	Vertex,
	Pixel,
	Hull,
	Domain,
	Geometry,
	Compute,
};

inline bool IsCompressedFormat(TexelsFormat format) { return format < TexelsFormat::UnknownCompressed; }
inline bool IsDepthFormat(TexelsFormat format) { return format > TexelsFormat::UnknownDepth; }

enum class ComparisonFunc : uint8_t
{
	Always,
	Never,
	Less,
	LessEqual,
	Greater,
	GreaterEqual,
	Equal,
	NotEqual,
};
typedef ComparisonFunc DepthFunc;
typedef ComparisonFunc StencilFunc;
typedef ComparisonFunc SamplerFunc;

enum class DepthWriteMask : uint8_t
{
	Zero,
	All,
};

enum class StencilOp : uint8_t
{
	Keep,
	Zero,
	Replace,
	Increment,
	Decrement,
};

enum class FillMode : uint8_t
{
	Solid,
	Wireframe,
};

enum class CullMode : uint8_t
{
	Back,
	Front,
	None,
};

enum class CounterDirection : size_t
{
	CW,
	CCW,
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
};

enum class BlendOp : uint8_t
{
	Add,
	Subtract,
	RevSubtract,
	Min,
	Max,
};

enum class ColorWriteMask : uint8_t
{
	Red   = (1u << 0),
	Green = (1u << 1),
	Blue  = (1u << 2),
	Alpha = (1u << 3),
	All   = Red | Green | Blue | Alpha
};

enum class TextureFilter : size_t
{
	MinMagMip_Point,
	MinMag_Point_Mip_Linear,
	Min_Point_Mag_Linear_Mip_Point,
	Min_Point_MagMip_Linear,
	Min_Linear_MagMip_Point,
	Min_Linear_Mag_Point_Mip_Linear,
	MinMag_Linear_Mip_Point,
	MinMagMip_Linear,
	Anisotropic,
};

enum class AddressMode : uint8_t // ??? TextureSamplerWrap
{
	Wrap,
	Mirror,
	Clamp,
	Border,
};

enum BufferUsage : uint8_t
{
	Default,
	Immutable,
	Dynamic,
	Staging
};

enum CPUAccessFlags : uint8_t
{
	None,
	Write,
	Read,
	WriteRead
};

enum class MapType : uint8_t
{
	Read,
	Write,
};

enum class PrimitiveTopology : uint8_t
{
	TriangleList,
	TriangleStrip,
	PointList
};

namespace ShaderCompileFlags
{
	enum : uint64_t {
		Debug     = (1ul << 0),
		Strict    = (1ul << 1),
		IEEStrict = (1ul << 2),
		Optimize0 = (1ul << 3),
		Optimize1 = (1ul << 4),
		Optimize2 = (1ul << 5),
		Optimize3 = (1ul << 6)
	};
}

namespace TextureFlags
{
	enum : uint32_t {
		RenderTarget = (1u << 0),
		DepthStencil = (1u << 1),
		CPURead      = (1u << 2),
		GPUWrite     = (1u << 3),
		GPUCounter   = (1u << 4)
	};
}

namespace BufferFlags
{
	enum : uint32_t {
		VertexBuffer     = (1u << 0), // can be set as a vertex buffer
		IndexBuffer      = (1u << 1), // can be set as an index buffer
		StructuredBuffer = (1u << 2), // can be set as a structured buffer
		CPURead          = (1u << 3), // can be mapped to be read by the CPU
		CPUWrite         = (1u << 4), // can be mapped to be written by the CPU
		GPUWrite         = (1u << 5), // can be written by the GPU
		GPUCounter       = (1u << 6), // can be written by the GPU with atomic counter usage
		GPUAppend        = (1u << 7), // can be appended by the GPU
		StreamOutput     = (1u << 8), // can be used as a stream output buffer
		IndirectArgs     = (1u << 9), // can be used as a drawIndirect args buffer
	};
}

#pragma endregion

#pragma region [ RHI Core Desc]

struct BlendDesc final
{
	bool           blendEnabled  = false;
	ColorWriteMask writeMask     = ColorWriteMask::All;
	BlendFactor    srcBlend      = BlendFactor::One;
	BlendFactor    dstBlend      = BlendFactor::Zero;
	BlendOp        blendOp       = BlendOp::Add;
	BlendFactor    srcBlendAlpha = BlendFactor::One;
	BlendFactor    dstBlendAlpha = BlendFactor::Zero;
	BlendOp        blendOpAlpha  = BlendOp::Add;
};

struct StencilDesc final
{
	StencilFunc stencilFunc = StencilFunc::Always;
	StencilOp   failOp      = StencilOp::Keep;
	StencilOp   depthFailOp = StencilOp::Keep;
	StencilOp   passOp      = StencilOp::Keep;
};

struct ShaderCompileMacro final
{
	const char* name{};
	const char* value{};
};

struct ShaderLoadInfo final
{
	std::wstring                    file{};       // example: L"gpu.hlsl"
	std::string                     entryPoint{}; // example: "vsMain"
	std::vector<ShaderCompileMacro> macros{};
	uint64_t                        flags{ 0 };
};

#pragma endregion

#pragma region [ RHI Descriptors ]

struct InputLayoutDescriptor final
{
	std::string semanticName{};
	uint32_t    semanticIndex{ 0 };
	DataFormat  format;
	uint32_t    slot{ 0 };
	uint64_t    offset{ 0 };
	bool        perInstanceData{ false };
	uint32_t    instanceDataStepRate{ 0 };
};

struct RasterizerStateDescriptor final
{
	FillMode         fillMode = FillMode::Solid;
	CullMode         cullMode = CullMode::Back;
	CounterDirection counterDirection = CounterDirection::CCW;
};

struct BlendStateDescriptor final
{
	BlendDesc blendDesc;

	bool      separateBlendEnabled = false;
	BlendDesc renderTargetBlendDesc[RenderTargetSlotCount];

	bool      alphaToCoverageEnabled = false;
};

struct DepthStencilStateDescriptor final
{
	bool           depthEnabled = true;
	DepthWriteMask writeMask = DepthWriteMask::All;
	DepthFunc      depthFunc = DepthFunc::Less;

	bool           stencilEnabled = false;
	uint32_t       stencilRef{ 0 };
	uint8_t        stencilReadMask{ 0xff };
	uint8_t        stencilWriteMask{ 0xff };
	StencilDesc    frontFaceStencilDesc{};
	StencilDesc    backFaceStencilDesc{};
};

#pragma endregion

#pragma region [ RHI CreateInfo structs ]

struct ShaderProgramLoadInfo final
{
	ShaderLoadInfo                     vertexShader{};
	std::vector<InputLayoutDescriptor> inputLayout{};
	ShaderLoadInfo                     pixelShader{};
	ShaderLoadInfo                     hullShader{};
	ShaderLoadInfo                     domainShader{};
	ShaderLoadInfo                     geometryShader{};
	ShaderLoadInfo                     computeShader{};
};
struct ShaderProgramCreateInfo final
{
};

struct PipelineStateCreateInfo final
{
	RasterizerStateDescriptor   rasterizerState;
	BlendStateDescriptor        blendState;
	DepthStencilStateDescriptor depthStencilState;
};

struct SamplerStateCreateInfo final
{
	TextureFilter  filter = TextureFilter::MinMagMip_Linear;
	AddressMode    addressU = AddressMode::Clamp;
	AddressMode    addressV = AddressMode::Clamp;
	AddressMode    addressW = AddressMode::Clamp;
	float          lodBias = 0.0F;
	uint32_t       maxAnisotropy = 1;
	ComparisonFunc comparisonFunc = ComparisonFunc::Never;
	uint32_t       borderColor = 0xFFFFFFFF;
	float          minLod = -FLT_MAX;
	float          maxLod = FLT_MAX;
};

struct BufferCreateInfo final
{
	uint32_t flags{ 0 };
	void*    memoryData{ nullptr };
	size_t   size{ 0 };
	size_t   stride{ 0 };
};

struct ConstantBufferCreateInfo final
{
	BufferUsage    usage{ BufferUsage::Default };
	CPUAccessFlags cpuAccessFlags{ CPUAccessFlags::None };
	void*          memoryData{ nullptr };
	size_t         size{ 0 };
};

struct Texture1DCreateInfo final
{
	uint32_t     width{ 0 };
	uint32_t     mipCount{ 1 };
	TexelsFormat format{ TexelsFormat::RGBA8 };
	uint32_t     flags{ 0 };
	void*        memoryData{ nullptr };
	size_t       size{ 0 };
};

struct Texture2DCreateInfo final
{
	uint32_t     width{ 0 };
	uint32_t     height{ 0 };
	uint32_t     mipCount{ 1 };
	TexelsFormat format{ TexelsFormat::RGBA8 };
	uint32_t     flags{ 0 };
	void*        memoryData{ nullptr };
	size_t       size{ 0 };
};

struct Texture3DCreateInfo final
{
	uint32_t     width{ 0 };
	uint32_t     height{ 0 };
	uint32_t     depth{ 0 };
	uint32_t     mipCount{ 1 };
	TexelsFormat format{ TexelsFormat::RGBA8 };
	uint32_t     flags{ 0 };
	void*        memoryData{ nullptr };
	size_t       size{ 0 };
};

struct TextureArrayCreateInfo final
{
};

#pragma endregion

#pragma region [ RHI Resources ]

struct ShaderProgram;
using ShaderProgramPtr = std::shared_ptr<ShaderProgram>;

struct PipelineState;
using PipelineStatePtr = std::shared_ptr<PipelineState>;

struct SamplerState;
using SamplerStatePtr = std::shared_ptr<SamplerState>;

struct Buffer;
using BufferPtr = std::shared_ptr<Buffer>;

struct ConstantBuffer;
using ConstantBufferPtr = std::shared_ptr<ConstantBuffer>;

struct Texture;
using TexturePtr = std::shared_ptr<Texture>;

struct RenderTarget;
using RenderTargetPtr = std::shared_ptr<RenderTarget>;

#pragma endregion

#pragma endregion
//=============================================================================
#pragma region [ Engine App ]

struct CuteEngineCreateInfo final
{
	struct Window
	{
		uint32_t          width{ 1600 };
		uint32_t          height{ 900 };
		std::wstring_view title{ L"Game" };
		bool              resizable{ true };
		bool              maximize{ false };
		bool              fullScreen{ false };
	} window;

	struct RHI
	{
		bool              vsync{ false };
	} rhi;
};

class ICuteEngineApp
{
public:
	virtual ~ICuteEngineApp() = default;

	virtual CuteEngineCreateInfo GetCreateInfo() { return {}; }
	virtual bool OnInit() { return true; }
	virtual void OnClose() {}

	virtual void OnUpdate([[maybe_unused]] double deltaTime) {}
	virtual void OnFixedUpdate() {}
	virtual void OnFrame() {}

	// Window Events
	virtual void OnWindowResize(
		[[maybe_unused]] uint32_t width, 
		[[maybe_unused]] uint32_t height) {}

	// Mouse Events
	virtual bool OnMouseMove(
		[[maybe_unused]] int x, 
		[[maybe_unused]] int y, 
		[[maybe_unused]] int deltaX, 
		[[maybe_unused]] int deltaY) { return true; }

	virtual bool OnMouseButton(
		[[maybe_unused]] int x, 
		[[maybe_unused]] int y, 
		[[maybe_unused]] Input::MouseButton button,
		[[maybe_unused]] bool pressed) { return true; }
		
	virtual bool OnMouseWheel([[maybe_unused]] int scroll) { return true; }

	// Keyboard Events
	virtual bool OnKey(
		[[maybe_unused]] uint32_t key, 
		[[maybe_unused]] bool pressed) { return true; }
};

class CuteEngineApp : protected ICuteEngineApp
{
public:
	void Run();

	void Exit();

	// Window
	void SetWindowTitle(std::wstring_view title);
	void RegisterDropCallback(void (*callback)(char const*, uint32_t, void*), void* data);
	void UnregisterDropCallback();

	[[nodiscard]] uint32_t GetWindowWidth() const;
	[[nodiscard]] uint32_t GetWindowHeight() const;
	[[nodiscard]] float GetWindowAspect() const;

	// Input
	bool IsKeyDown(uint32_t keyCode) const;
	bool IsKeyPressed(uint32_t keyCode) const;
	bool IsKeyReleased(uint32_t keyCode) const;

	// RHI Core
	void BeginPerfEvent(const wchar_t* name);
	void EndPerfEvent();
	void SetMainFrame(const std::optional<Color>& clearColor, const std::optional<float>& clearDepth, const std::optional<uint8_t>& clearStencil);

	void Draw(PrimitiveTopology topology, uint32_t vertexCount, uint32_t startVertexLocation);
	void DrawIndexed(PrimitiveTopology topology, uint32_t indexCount, uint32_t startIndexLocation, uint32_t baseVertexLocation);
	void DrawInstanced(PrimitiveTopology topology, uint32_t vertexCountPerInstance, uint32_t instanceCount, uint32_t startVertexLocation, uint32_t startInstanceLocation);
	void DrawIndexedInstanced(PrimitiveTopology topology, uint32_t indexCountPerInstance, uint32_t instanceCount, uint32_t startIndexLocation = 0, uint32_t baseVertexLocation = 0, uint32_t startInstanceLocation = 0);

	void DrawInstancedIndirect(PrimitiveTopology topology, BufferPtr indirectArgs, size_t argsOffset);
	void DrawIndexedInstancedIndirect(PrimitiveTopology topology, BufferPtr indirectArgs, size_t argsOffset);

	void RHIDeviceFlush();

	// RHI Resources Create
	std::expected<ShaderProgramPtr, std::string>  LoadShaderProgram(const ShaderProgramLoadInfo& loadInfo);
	std::expected<PipelineStatePtr, std::string>  CreatePipelineState(const PipelineStateCreateInfo& createInfo);
	std::expected<SamplerStatePtr, std::string>   CreateSamplerState(const SamplerStateCreateInfo& createInfo);
	std::expected<BufferPtr, std::string>         CreateBuffer(const BufferCreateInfo& createInfo);
	std::expected<ConstantBufferPtr, std::string> CreateConstantBuffer(const ConstantBufferCreateInfo& createInfo);
	std::expected<TexturePtr, std::string>        CreateTexture1D(const Texture1DCreateInfo& createInfo);
	std::expected<TexturePtr, std::string>        CreateTexture2D(const Texture2DCreateInfo& createInfo);
	std::expected<TexturePtr, std::string>        CreateTexture3D(const Texture3DCreateInfo& createInfo);
	std::expected<RenderTargetPtr, std::string>   CreateRenderTarget(const std::vector<TexturePtr> colorTextures, TexturePtr depthStencilTexture = nullptr);

	// RHI Resources Delete
	void DeleteRHIResource(ShaderProgramPtr& resource);
	void DeleteRHIResource(PipelineStatePtr& resource);
	void DeleteRHIResource(SamplerStatePtr& resource);
	void DeleteRHIResource(BufferPtr& resource);
	void DeleteRHIResource(ConstantBufferPtr& resource);
	void DeleteRHIResource(TexturePtr& resource);
	void DeleteRHIResource(RenderTargetPtr& resource);

	// RHI Resources Mod
	void ClearBufferRW(BufferPtr buffer, uint32_t value);
	void ClearBufferRW(BufferPtr buffer, float value);
	void* Map(BufferPtr buffer, MapType type);
	void Unmap(BufferPtr buffer);
	void UpdateBuffer(BufferPtr buffer, const void* mem);
	void CopyBufferData(BufferPtr buffer, size_t offset, size_t size, const void* mem);
	void CopyResource(BufferPtr src, BufferPtr dst);

	void* Map(ConstantBufferPtr buffer, MapType type);
	void Unmap(ConstantBufferPtr buffer);
	void UpdateBuffer(ConstantBufferPtr buffer, const void* mem);
	void CopyResource(ConstantBufferPtr src, ConstantBufferPtr dst);

	void ClearTextureRW(TexturePtr texture, uint32_t value);
	void ClearTextureRW(TexturePtr texture, float value);
	void* Map(TexturePtr handle, MapType type);
	void Unmap(TexturePtr handle);
	void UpdateTexture(
		TexturePtr handle, const void* mem,
		uint32_t mip,
		size_t offsetX, size_t sizeX,
		size_t offsetY, size_t sizeY,
		size_t offsetZ, size_t sizeZ,
		size_t rowPitch, size_t depthPitch
	);
	void CopyResource(TexturePtr src, TexturePtr dst);

	// RHI Resources Bind
	void BindShaderProgram(ShaderProgramPtr resource);
	void BindPipelineState(PipelineStatePtr resource);
	void BindSamplerState(SamplerStatePtr resource, ShaderType shaderType, uint32_t slot);
	void BindConstantBuffer(ConstantBufferPtr resource, ShaderType shaderType, uint32_t slot);
	void BindShaderResource(BufferPtr resource, ShaderType shaderType, uint32_t slot);
	void BindShaderResource(TexturePtr resource, ShaderType shaderType, uint32_t slot);
	
	void BindVertexBuffer(BufferPtr resource, uint32_t slot = 0);
	void BindVertexBuffers(const std::vector<BufferPtr>& resources);
	void BindIndexBuffer(BufferPtr resource);

	void SetRenderTarget(RenderTargetPtr rt, const std::optional<Color>& clearColor, const std::optional<float>& clearDepth, const std::optional<uint8_t>& clearStencil);
};

#pragma endregion
//=============================================================================
#ifdef CUTE_IMPLEMENTATION_DEFINE
#	include "CuteEngine.cpp"
#endif
//=============================================================================