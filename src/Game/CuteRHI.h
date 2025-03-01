#pragma once

//=============================================================================
#pragma region [ Header ]

#include <cstdint>
#include <expected>
#include <memory>
#include <string>
#include <vector>

#pragma endregion

namespace rhi
{
//=============================================================================
#pragma region [ Base ]



#pragma endregion
//=============================================================================
#pragma region [ Enum ]

	enum class DataFormat : uint8_t
	{
		R1, R8, R16, R16F, R32I, R32U, R32F,               // R
		RG8, RG16, RG16F, RG32I, RG32U, RG32F,             // RG
		RGB32I, RGB32U, RGB32F,                            // RGB
		RGBA8, RGBA16, RGBA16F, RGBA32I, RGBA32U, RGBA32F, // RGBA
		R11G11B10F
	};

#pragma endregion
//=============================================================================
#pragma region [ Shader ]

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

	struct ShaderProgram;
	using ShaderProgramPtr = std::shared_ptr<ShaderProgram>;

	std::expected<ShaderProgramPtr, std::string> LoadShaderProgram(const ShaderProgramLoadInfo& loadInfo);
	void DeleteResource(ShaderProgramPtr& resource);
	void BindShaderProgram(ShaderProgramPtr resource);

#pragma endregion
//=============================================================================
#pragma region [ Pipeline State ]

#pragma endregion
//...
/*
- Buffer
- Texture
- Sampler
- RenderTarget
- Draw

*/

} // namespace rhi