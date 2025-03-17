#pragma once

#include <cstdint>
#include <expected>
#include <optional>
#include <memory>
#include <string>
#include <vector>

namespace rhi
{
	void Print(const std::string& message);
	void Print(const std::wstring& message);
	void Fatal(const std::string& error);

	struct Color final
	{
		const float* Get() const { return &r; }
		operator float* () { return &r; }

		float r{ 1.0f };
		float g{ 1.0f };
		float b{ 1.0f };
		float a{ 1.0f };
	};

	constexpr const auto RenderTargetSlotCount = 8u;
	constexpr const auto MaxVertexBuffers = 16u;
	constexpr const auto AppendAlignedElement = 0xffffffff;

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

		DepthNone,
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
		IncrementSAT,
		DecrementSAT,
		Invert
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

	enum class BlendFactor : uint8_t
	{
		Zero,
		One,
		SrcAlpha,
		DestAlpha,
		InvSrcAlpha,
		InvDestAlpha,
		SrcColor,
		DestColor,
		InvSrcColor,
		InvDestColor,
		SrcAlphaSat,
		Factor,
		InvFactor,
		Src1Color,
		InvSrc1Color,
		Src1Alpha,
		InvSrc1Alpha
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
			Debug = (1ul << 0),
			IEEStrict = (1ul << 1),
			Optimize0 = (1ul << 2),
			Optimize1 = (1ul << 3),
			Optimize2 = (1ul << 4),
			Optimize3 = (1ul << 5)
		};
	}

	namespace TextureFlags
	{
		enum : uint32_t {
			RenderTarget = (1u << 0),
			DepthStencil = (1u << 1),
			CPURead = (1u << 2),
			GPUWrite = (1u << 3),
			GPUCounter = (1u << 4)
		};
	}

	namespace BufferFlags
	{
		enum : uint32_t {
			VertexBuffer = (1u << 0), // can be set as a vertex buffer
			IndexBuffer = (1u << 1), // can be set as an index buffer
			StructuredBuffer = (1u << 2), // can be set as a structured buffer
			CPURead = (1u << 3), // can be mapped to be read by the CPU
			CPUWrite = (1u << 4), // can be mapped to be written by the CPU
			GPUWrite = (1u << 5), // can be written by the GPU
			GPUCounter = (1u << 6), // can be written by the GPU with atomic counter usage
			GPUAppend = (1u << 7), // can be appended by the GPU
			StreamOutput = (1u << 8), // can be used as a stream output buffer
			IndirectArgs = (1u << 9), // can be used as a drawIndirect args buffer
		};
	}

	struct BlendTargetDesc final
	{
		bool           enable{ false };
		ColorWriteMask writeMask{ ColorWriteMask::All };
		BlendFactor    srcColor{ BlendFactor::One };
		BlendFactor    dstColor{ BlendFactor::Zero };
		BlendOp        opColor{ BlendOp::Add };
		BlendFactor    srcAlpha{ BlendFactor::One };
		BlendFactor    dstAlpha{ BlendFactor::Zero };
		BlendOp        opAlpha{ BlendOp::Add };
	};

	struct StencilDesc final
	{
		StencilFunc comparison{ StencilFunc::Always };
		StencilOp   failOp{ StencilOp::Keep };
		StencilOp   depthFailOp{ StencilOp::Keep };
		StencilOp   passOp{ StencilOp::Keep };
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
		FillMode fillMode{ FillMode::Solid };
		CullMode cullMode{ CullMode::Back };
		bool     frontCounterClockwise{ false };
		int32_t  depthBias{ 0 };
		float    depthBiasClamp{ 0.0f };
		float    slopeScaledDepthBias{ 0.0f };
		bool     enableDepthClip{ true };
		bool     enableScissor{ false };
		bool     enableMultisample{ false };
		bool     enableAntialiasedLine{ false };
	};

	struct BlendStateDescriptor final
	{
		BlendTargetDesc blendDesc{};

		bool            independentBlendEnabled{ false };
		BlendTargetDesc renderTargetBlendDesc[RenderTargetSlotCount]{};

		bool            alphaToCoverageEnabled{ false };
		Color           blendColor{ 0.0f, 0.0f, 0.0f, 0.0f };
		uint32_t        sampleMask{ 0xFFFFFFFFu };
	};

	struct DepthStencilStateDescriptor final
	{
		bool           depthEnabled{ true };
		DepthWriteMask writeMask{ DepthWriteMask::All };
		DepthFunc      depthFunc{ DepthFunc::Less };
		bool           stencilEnabled{ false };
		uint8_t        stencilReadMask{ 0xFF };
		uint8_t        stencilWriteMask{ 0xFF };
		StencilDesc    frontFaceStencilDesc{};
		StencilDesc    backFaceStencilDesc{};
		uint32_t       stencilReference{ 0 };
	};

	struct RenderTargetColorDescriptor final
	{
		TexelsFormat format{ TexelsFormat::RGBA8 };
	};

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
		void* memoryData{ nullptr };
		size_t         size{ 0 };
	};

	struct Texture1DCreateInfo final
	{
		uint32_t     width{ 0 };
		uint32_t     mipCount{ 1 };
		TexelsFormat format{ TexelsFormat::RGBA8 };
		uint32_t     flags{ 0 };
		void*        memoryData{ nullptr };
	};

	struct Texture2DCreateInfo final
	{
		uint32_t     width{ 0 };
		uint32_t     height{ 0 };
		uint32_t     mipCount{ 1 };
		TexelsFormat format{ TexelsFormat::RGBA8 };
		uint32_t     flags{ 0 };
		void*        memoryData{ nullptr };
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
	};

	struct TextureArrayCreateInfo final
	{
	};

	struct RenderTargetCreateInfo final
	{
		uint8_t                     rtSlotCount{ 1 };
		RenderTargetColorDescriptor colorBuffers[RenderTargetSlotCount];
		TexelsFormat                depthFormat{ TexelsFormat::D32F }; // DepthNone - нет буфера глубины
		uint32_t                    width{ 0 };
		uint32_t                    height{ 0 };
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

	inline bool IsCompressedFormat(TexelsFormat format) { return format < TexelsFormat::UnknownCompressed; }
	inline bool IsDepthFormat(TexelsFormat format) { return format > TexelsFormat::UnknownDepth; }

	// Setup and misc functions
	bool Setup(void* hwnd, uint32_t frameWidth, uint32_t frameHeight, bool vsync, bool enableImGui);
	void Shutdown();
	bool IsValid();
	void BeginFrame();
	void EndFrame();
	bool Resize(uint32_t width, uint32_t height);
	void BeginPerfEvent(const wchar_t* name);
	void EndPerfEvent();

	// Rendering functions
	void SetMainFrame(const std::optional<Color>& clearColor, const std::optional<float>& clearDepth, const std::optional<uint8_t>& clearStencil);

	void Draw(PrimitiveTopology topology, uint32_t vertexCount, uint32_t startVertexLocation);
	void DrawIndexed(PrimitiveTopology topology, uint32_t indexCount, uint32_t startIndexLocation, uint32_t baseVertexLocation);
	void DrawInstanced(PrimitiveTopology topology, uint32_t vertexCountPerInstance, uint32_t instanceCount, uint32_t startVertexLocation, uint32_t startInstanceLocation);
	void DrawIndexedInstanced(PrimitiveTopology topology, uint32_t indexCountPerInstance, uint32_t instanceCount, uint32_t startIndexLocation = 0, uint32_t baseVertexLocation = 0, uint32_t startInstanceLocation = 0);

	void DrawInstancedIndirect(PrimitiveTopology topology, BufferPtr indirectArgs, size_t argsOffset);
	void DrawIndexedInstancedIndirect(PrimitiveTopology topology, BufferPtr indirectArgs, size_t argsOffset);

	void Flush();

	// RHI Resources Create
	std::expected<ShaderProgramPtr, std::string>  LoadShaderProgram(const ShaderProgramLoadInfo& loadInfo);
	std::expected<PipelineStatePtr, std::string>  CreatePipelineState(const PipelineStateCreateInfo& createInfo);
	std::expected<SamplerStatePtr, std::string>   CreateSamplerState(const SamplerStateCreateInfo& createInfo);
	std::expected<BufferPtr, std::string>         CreateBuffer(const BufferCreateInfo& createInfo);
	std::expected<ConstantBufferPtr, std::string> CreateConstantBuffer(const ConstantBufferCreateInfo& createInfo);
	std::expected<TexturePtr, std::string>        CreateTexture1D(const Texture1DCreateInfo& createInfo);
	std::expected<TexturePtr, std::string>        CreateTexture2D(const Texture2DCreateInfo& createInfo);
	std::expected<TexturePtr, std::string>        CreateTexture3D(const Texture3DCreateInfo& createInfo);
	std::expected<RenderTargetPtr, std::string>   CreateRenderTarget(const RenderTargetCreateInfo& createInfo);

	// RHI Resources Delete
	template<typename T>
	void DeleteResource(T& resource)
	{
		resource.reset();
	}

	// RHI Resources Updating
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

	// RHI Resources Binding
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

} // namespace rhi
#ifdef CUTE_RHI_IMPLEMENTATION_DEFINE
#	include "CuteRHI.cpp"
#endif