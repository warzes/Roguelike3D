#include "stdafx.h"
#include "GameApp.h"
//=============================================================================
namespace
{
	uint32_t TextureData[] = { 0xffffffff, 0xff7f7f7f, 0xff7f7f7f, 0xffffffff }; // 2x2 pixels

	float VertexData[] = // pos.x, pos.y, pos.z, nor.x, nor.y, nor.z, tex.u, tex.v, ...
	{
		-1.00f,  1.00f, -1.00f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.00f,  1.00f, -1.00f,  0.0f,  0.0f, -1.0f,  9.5f,  0.0f,
		 0.58f,  0.58f, -1.00f,  0.0f,  0.0f, -1.0f,  7.5f,  2.0f, -0.58f,  0.58f, -1.00f,  0.0f,  0.0f, -1.0f,  2.0f,  2.0f,
		-0.58f,  0.58f, -1.00f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.58f,  0.58f, -1.00f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		 0.58f,  0.58f, -0.58f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f, -0.58f,  0.58f, -0.58f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	};

	uint32_t IndexData[] = { 0, 1, 3, 1, 2, 3, 4, 5, 7, 5, 6, 7 };

	uint32_t InstanceRotationData[] = { 0, 0, 0, 0, 0, 1, 0, 0, 2, 0, 0, 3, 1, 0, 0, 1, 1, 0, 1, 2, 0, 1, 3, 0, 2, 0, 0, 2, 0, 1, 2, 0, 2, 2, 0, 3, 3, 0, 0, 3, 1, 0, 3, 2, 0, 3, 3, 0, 1, 0, 1, 1, 1, 1, 1, 2, 1, 1, 3, 1, 1, 0, 3, 1, 1, 3, 1, 2, 3, 1, 3, 3 }; // rot.x, rot.y, rot.z, ... in multiples of 90 degrees

	float InstanceColorData[] = { 0.973f, 0.480f, 0.002f, 0.897f, 0.163f, 0.011f, 0.612f, 0.000f, 0.069f, 0.127f, 0.116f, 0.408f, 0.000f, 0.254f, 0.637f, 0.001f, 0.447f, 0.067f }; // col.r, col.g, col.b, ...

#define ARRAYSIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

	struct float4 { float x, y, z, w; };

	struct Constants
	{
		float4 projection[4];
		float4 lightVector;
		float4 rotate;
		float4 scale;
		float4 translate;
	};

	Constants constants; // projection matrix
}
//=============================================================================
bool GameApp::OnInit()
{
	// Create ShaderProgram
	{
		ShaderProgramLoadInfo spli{};
		spli.vertexShader = { L"shaders.hlsl", "vs_main" };
		spli.pixelShader =  { L"shaders.hlsl", "ps_main" };
		spli.inputLayout = { 
			// float3 position, float3 normal, float2 texcoord, uint3 rotation, float3 color
			{"POS", 0, DataFormat::RGB32F, 0, 0,                    false},
			{"NOR", 0, DataFormat::RGB32F, 0, AppendAlignedElement, false},
			{"TEX", 0, DataFormat::RG32F,  0, AppendAlignedElement, false},

			{"ROT", 0, DataFormat::RGB32U, 1, AppendAlignedElement, true, 1}, // advance every instance
			{"COL", 0, DataFormat::RGB32F, 2, AppendAlignedElement, true, 4}, // advance every 4th instance, i.e. every face
		};

		auto resource = LoadShaderProgram(spli);
		if (!resource.has_value())
		{
			Fatal(resource.error());
			return false;
		}
		m_shaderProgram = resource.value();
	}

	// Create PipelineState
	{
		PipelineStateCreateInfo psci{};
		auto resource = CreatePipelineState(psci);
		if (!resource.has_value())
		{
			Fatal(resource.error());
			return false;
		}
		m_pipelineState = resource.value();
	}

	// Create SamplerState
	{
		SamplerStateCreateInfo ssci{};
		ssci.filter = TextureFilter::MinMagMip_Point;
		ssci.addressU = AddressMode::Wrap;
		ssci.addressV = AddressMode::Wrap;
		ssci.addressW = AddressMode::Wrap;
		auto resource = CreateSamplerState(ssci);
		if (!resource.has_value())
		{
			Fatal(resource.error());
			return false;
		}
		m_samplerState = resource.value();
	}

	// Create ConstantBuffer
	{
		ConstantBufferCreateInfo cbci{};
		cbci.usage          = BufferUsage::Dynamic;
		cbci.size           = sizeof(Constants) + 0xf & 0xfffffff0;
		cbci.cpuAccessFlags = CPUAccessFlags::Write;

		auto resource = CreateConstantBuffer(cbci);
		if (!resource.has_value())
		{
			Fatal(resource.error());
			return false;
		}
		m_constantBuffer = resource.value();
	}

	// Create VertexBuffer
	{
		BufferCreateInfo vbci{};
		vbci.flags      = BufferFlags::VertexBuffer;
		vbci.size       = sizeof(VertexData);
		vbci.memoryData = VertexData;
		vbci.stride     = 8 * sizeof(float);

		auto resource = CreateBuffer(vbci);
		if (!resource.has_value())
		{
			Fatal(resource.error());
			return false;
		}
		m_vertexBuffer = resource.value();
	}

	// Create InstanceRotationBuffer
	{
		BufferCreateInfo vbci{};
		vbci.flags      = BufferFlags::VertexBuffer;
		vbci.size       = sizeof(InstanceRotationData);
		vbci.memoryData = InstanceRotationData;
		vbci.stride     = 3 * sizeof(float);

		auto resource = CreateBuffer(vbci);
		if (!resource.has_value())
		{
			Fatal(resource.error());
			return false;
		}
		m_instanceRotationBuffer = resource.value();
	}

	// Create InstanceColorBuffer
	{
		BufferCreateInfo vbci{};
		vbci.flags      = BufferFlags::VertexBuffer;
		vbci.size       = sizeof(InstanceColorData);
		vbci.memoryData = InstanceColorData;
		vbci.stride     = 3 * sizeof(float);

		auto resource = CreateBuffer(vbci);
		if (!resource.has_value())
		{
			Fatal(resource.error());
			return false;
		}
		m_instanceColorBuffer = resource.value();
	}

	// Create IndexBuffer
	{
		BufferCreateInfo ibci{};
		ibci.flags      = BufferFlags::IndexBuffer;
		ibci.size       = sizeof(IndexData);
		ibci.memoryData = IndexData;
		ibci.stride     = sizeof(uint32_t);

		auto resource = CreateBuffer(ibci);
		if (!resource.has_value())
		{
			Fatal(resource.error());
			return false;
		}
		m_indexBuffer = resource.value();
	}

	// Create Texture2D
	{
		Texture2DCreateInfo tci{};
		tci.width      = 2;
		tci.height     = 2;
		tci.mipCount   = 1;
		//tci.memoryData = TextureData;

		auto resource = CreateTexture2D(tci);
		if (!resource.has_value())
		{
			Fatal(resource.error());
			return false;
		}
		m_texture = resource.value();

// TODO: доделать загрузку данных при инициализации
		UpdateTexture(
			m_texture, 
			TextureData, 
			0, 
			0, 2,
			0, 2,
			0, 1,
			2 * 4, // Размер строки в байтах (ширина текстуры * количество байт на пиксель), для данного примера textureData.SysMemPitch = 2 * sizeof(UINT);
			0);    // Размер среза в байтах (для 2D текстур обычно 0)
	}

	float w = GetWindowAspect(); // width (aspect ratio)
	float h = 1.0f;              // height
	float n = 1.0f;              // near
	float f = 9.0f;              // far

	constants = { 2 * n / w, 0, 0, 0, 0, 2 * n / h, 0, 0, 0, 0, f / (f - n), 1, 0, 0, n * f / (n - f), 0 }; // projection matrix

	constants.lightVector = { 1.0f, -1.0f,  1.0f };
	constants.rotate = { 0.0f,  0.0f,  0.0f };
	constants.scale = { 1.0f,  1.0f,  1.0f };
	constants.translate = { 0.0f,  0.0f,  4.0f };
	return true;
}
//=============================================================================
void GameApp::OnClose()
{
	DeleteRHIResource(m_shaderProgram);
	DeleteRHIResource(m_pipelineState);
	DeleteRHIResource(m_samplerState);
	DeleteRHIResource(m_constantBuffer);
	DeleteRHIResource(m_vertexBuffer);
	DeleteRHIResource(m_instanceRotationBuffer);
	DeleteRHIResource(m_instanceColorBuffer);
	DeleteRHIResource(m_indexBuffer);
	DeleteRHIResource(m_texture);
}
//=============================================================================
void GameApp::OnUpdate(double deltaTime)
{
	constants.rotate.x += 0.005f;
	constants.rotate.y += 0.009f;
	constants.rotate.z += 0.001f;

	Constants* data = (Constants*)Map(m_constantBuffer, MapType::Write);
	*data = constants;
	Unmap(m_constantBuffer);
}
//=============================================================================
void GameApp::OnFrame()
{
	SetMainFrame(Color{ 0.392156899f, 0.584313750f, 0.929411829f, 1.f }, 1.0f, 0);
	BindShaderProgram(m_shaderProgram);
	BindPipelineState(m_pipelineState);
	BindSamplerState(m_samplerState, ShaderType::Pixel, 0);
	BindConstantBuffer(m_constantBuffer, ShaderType::Vertex, 0);
	BindVertexBuffers({ m_vertexBuffer, m_instanceRotationBuffer, m_instanceColorBuffer });
	BindIndexBuffer(m_indexBuffer);
	BindShaderResource(m_texture, ShaderType::Pixel, 0);
	
	DrawIndexedInstanced(PrimitiveTopology::TriangleList, ARRAYSIZE(IndexData), 24);
}
//=============================================================================