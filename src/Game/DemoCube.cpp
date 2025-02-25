#include "stdafx.h"
#include "DemoCube.h"

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

	struct DemoConstants
	{
		float4 projection[4];
		float4 lightVector;
		float4 rotate;
		float4 scale;
		float4 translate;
	};

	DemoConstants constants; // projection matrix

	struct CommonVertex final
	{
		enum { MaxBones = 4 };
		float position[3];
		float texcoord0[2];
		float texcoord1[2];
		float normal[3];

		uint8_t boneIDs[MaxBones];
		float   boneWeights[MaxBones];
		uint8_t color[4];
	};

	CommonVertex cubeVertices[] =
	{
		{
			{ 1.000000, 1.000000, -1.000000 }, // Vertex
			{ 0.000000, 0.000000 }, // Texcoord 0
			{ 0, 0 }, // Texcoord 1
			{ 0.000000, 1.000000, 0.000000 }  // Normal
		},
		{
			{ -1.000000, 1.000000, -1.000000 }, // Vertex
			{ 0.000000, 1.000000 }, // Texcoord 0
			{ 0, 0 }, // Texcoord 1
			{ 0.000000, 1.000000, 0.000000 }  // Normal
		},
		{
			{ -1.000000, 1.000000, 1.000000 }, // Vertex
			{ 1.000000, 1.000000 }, // Texcoord 0
			{ 0, 0 }, // Texcoord 1
			{ 0.000000, 1.000000, 0.000000 }  // Normal
		},
		{
			{ 1.000000, 1.000000, 1.000000 }, // Vertex
			{ 1.000000, 0.000000 }, // Texcoord 0
			{ 0, 0 }, // Texcoord 1
			{ 0.000000, 1.000000, 0.000000 }  // Normal
		},
		{
			{ 1.000000, -1.000000, 1.000000 }, // Vertex
			{ 0.000000, 0.000000 }, // Texcoord 0
			{ 0, 0 }, // Texcoord 1
			{ 0.000000, -1.000000, 0.000000 }  // Normal
		},
		{
			{ -1.000000, -1.000000, 1.000000 }, // Vertex
			{ 0.000000, 1.000000 }, // Texcoord 0
			{ 0, 0 }, // Texcoord 1
			{ 0.000000, -1.000000, 0.000000 }  // Normal
		},
		{
			{ -1.000000, -1.000000, -1.000000 }, // Vertex
			{ 1.000000, 1.000000 }, // Texcoord 0
			{ 0, 0 }, // Texcoord 1
			{ 0.000000, -1.000000, 0.000000 }  // Normal
		},
		{
			{ 1.000000, -1.000000, -1.000000 }, // Vertex
			{ 1.000000, 0.000000 }, // Texcoord 0
			{ 0, 0 }, // Texcoord 1
			{ 0.000000, -1.000000, 0.000000 }  // Normal
		},
		{
			{ 1.000000, 1.000000, 1.000000 }, // Vertex
			{ 0.000000, 0.000000 }, // Texcoord 0
			{ 0, 0 }, // Texcoord 1
			{ 0.000000, 0.000000, 1.000000 }  // Normal
		},
		{
			{ -1.000000, 1.000000, 1.000000 }, // Vertex
			{ 0.000000, 1.000000 }, // Texcoord 0
			{ 0, 0 }, // Texcoord 1
			{ 0.000000, 0.000000, 1.000000 }  // Normal
		},
		{
			{ -1.000000, -1.000000, 1.000000 }, // Vertex
			{ 1.000000, 1.000000 }, // Texcoord 0
			{ 0, 0 }, // Texcoord 1
			{ 0.000000, 0.000000, 1.000000 }  // Normal
		},
		{
			{ 1.000000, -1.000000, 1.000000 }, // Vertex
			{ 1.000000, 0.000000 }, // Texcoord 0
			{ 0, 0 }, // Texcoord 1
			{ 0.000000, 0.000000, 1.000000 }  // Normal
		},
		{
			{ 1.000000, -1.000000, -1.000000 }, // Vertex
			{ 0.000000, 0.000000 }, // Texcoord 0
			{ 0, 0 }, // Texcoord 1
			{ 0.000000, 0.000000, -1.000000 }  // Normal
		},
		{
			{ -1.000000, -1.000000, -1.000000 }, // Vertex
			{ 0.000000, 1.000000 }, // Texcoord 0
			{ 0, 0 }, // Texcoord 1
			{ 0.000000, 0.000000, -1.000000 }  // Normal
		},
		{
			{ -1.000000, 1.000000, -1.000000 }, // Vertex
			{ 1.000000, 1.000000 }, // Texcoord 0
			{ 0, 0 }, // Texcoord 1
			{ 0.000000, 0.000000, -1.000000 }  // Normal
		},
		{
			{ 1.000000, 1.000000, -1.000000 }, // Vertex
			{ 1.000000, 0.000000 }, // Texcoord 0
			{ 0, 0 }, // Texcoord 1
			{ 0.000000, 0.000000, -1.000000 }  // Normal
		},
		{
			{ -1.000000, 1.000000, 1.000000 }, // Vertex
			{ 0.000000, 0.000000 }, // Texcoord 0
			{ 0, 0 }, // Texcoord 1
			{ -1.000000, 0.000000, 0.000000 }  // Normal
		},
		{
			{ -1.000000, 1.000000, -1.000000 }, // Vertex
			{ 0.000000, 1.000000 }, // Texcoord 0
			{ 0, 0 }, // Texcoord 1
			{ -1.000000, 0.000000, 0.000000 }  // Normal
		},
		{
			{ -1.000000, -1.000000, -1.000000 }, // Vertex
			{ 1.000000, 1.000000 }, // Texcoord 0
			{ 0, 0 }, // Texcoord 1
			{ -1.000000, 0.000000, 0.000000 }  // Normal
		},
		{
			{ -1.000000, -1.000000, 1.000000 }, // Vertex
			{ 1.000000, 0.000000 }, // Texcoord 0
			{ 0, 0 }, // Texcoord 1
			{ -1.000000, 0.000000, 0.000000 }  // Normal
		},
		{
			{ 1.000000, 1.000000, -1.000000 }, // Vertex
			{ 0.000000, 0.000000 }, // Texcoord 0
			{ 0, 0 }, // Texcoord 1
			{ 1.000000, 0.000000, 0.000000 }  // Normal
		},
		{
			{ 1.000000, 1.000000, 1.000000 }, // Vertex
			{ 0.000000, 1.000000 }, // Texcoord 0
			{ 0, 0 }, // Texcoord 1
			{ 1.000000, 0.000000, 0.000000 }  // Normal
		},
		{
			{ 1.000000, -1.000000, 1.000000 }, // Vertex
			{ 1.000000, 1.000000 }, // Texcoord 0
			{ 0, 0 }, // Texcoord 1
			{ 1.000000, 0.000000, 0.000000 }  // Normal
		},
		{
			{ 1.000000, -1.000000, -1.000000 }, // Vertex
			{ 1.000000, 0.000000 }, // Texcoord 0
			{ 0, 0 }, // Texcoord 1
			{ 1.000000, 0.000000, 0.000000 }  // Normal
		}
	};
	size_t verticesSize = sizeof(cubeVertices) / sizeof(CommonVertex);

	static uint32_t cubeIndices[] = { 0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 8, 9, 10, 10, 11, 8, 12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20 };
	size_t indicesSize = sizeof(cubeIndices) / sizeof(uint32_t);

	struct DemoConstantBuffer final
	{
		float mvp[16];
	};
}
//=============================================================================
bool DemoCube::OnInit()
{
	// Create ShaderProgram
	{
		const size_t stride1 = 0;                             // Position
		const size_t stride2 = stride1 + 3 * sizeof(float);   // uv0
		const size_t stride3 = stride2 + 2 * sizeof(float);   // uv1
		const size_t stride4 = stride3 + 2 * sizeof(float);   // normal
		const size_t stride5 = stride4 + 3 * sizeof(float);   // boneIDs
		const size_t stride6 = stride5 + 4 * sizeof(uint8_t); // boneWeights
		const size_t stride7 = stride6 + 4 * sizeof(float);   // color

		ShaderProgramLoadInfo spli{};
		spli.vertexShader = { L"DemoCube.hlsl", "vs_main" };
		spli.pixelShader  = { L"DemoCube.hlsl", "ps_main" };
		spli.inputLayout = {
			{ "POSITION",    0, DataFormat::RGB32F,  0, stride1 },
			{ "TEXCOORDA",   0, DataFormat::RG32F,   0, stride2 },
			{ "TEXCOORDB",   0, DataFormat::RG32F,   0, stride3 },
			{ "NORMAL",      0, DataFormat::RGB32F,  0, stride4 },
			{ "BONEIDS",     0, DataFormat::R32U,    0, stride5 },
			{ "BONEWEIGHTS", 0, DataFormat::RGBA32F, 0, stride6 },
			{ "VCOLOR",      0, DataFormat::R32U,    0, stride7 },
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
		psci.rasterizerState.fillMode = FillMode::Solid;
		psci.rasterizerState.cullMode = CullMode::Back;
		psci.rasterizerState.counterDirection = CounterDirection::CW;

		psci.blendState.blendDesc.blendEnabled                  = false;
		psci.blendState.blendDesc.writeMask                     = ColorWriteMask::All;
		psci.blendState.blendDesc.srcBlend                      = BlendFactor::One;
		psci.blendState.blendDesc.dstBlend                      = BlendFactor::Zero;
		psci.blendState.blendDesc.blendOp                       = BlendOp::Add;
		psci.blendState.blendDesc.srcBlendAlpha                 = BlendFactor::One;
		psci.blendState.blendDesc.dstBlendAlpha                 = BlendFactor::Zero;
		psci.blendState.blendDesc.blendOpAlpha                  = BlendOp::Add;

		psci.depthStencilState.depthEnabled                     = true;
		psci.depthStencilState.writeMask                        = DepthWriteMask::All;
		psci.depthStencilState.depthFunc                        = DepthFunc::Less;

		psci.depthStencilState.stencilEnabled                   = false;
		psci.depthStencilState.stencilRef                       = 0;
		psci.depthStencilState.stencilReadMask                  = 0;
		psci.depthStencilState.stencilWriteMask                 = 0;

		psci.depthStencilState.frontFaceStencilDesc.stencilFunc = StencilFunc::Always;
		psci.depthStencilState.frontFaceStencilDesc.failOp      = StencilOp::Keep;
		psci.depthStencilState.frontFaceStencilDesc.depthFailOp = StencilOp::Keep;
		psci.depthStencilState.frontFaceStencilDesc.passOp      = StencilOp::Keep;

		psci.depthStencilState.backFaceStencilDesc.stencilFunc  = StencilFunc::Always;
		psci.depthStencilState.backFaceStencilDesc.failOp       = StencilOp::Keep;
		psci.depthStencilState.backFaceStencilDesc.depthFailOp  = StencilOp::Keep;
		psci.depthStencilState.backFaceStencilDesc.passOp       = StencilOp::Keep;

		auto resource = CreatePipelineState(psci);
		if (!resource.has_value())
		{
			Fatal(resource.error());
			return false;
		}
		m_pipelineState = resource.value();
	}

	// Create ConstantBuffer
	{
		ConstantBufferCreateInfo cbci{};
		cbci.usage = BufferUsage::Default;
		cbci.size = sizeof(DemoConstantBuffer)/* + 0xf & 0xfffffff0*/;
		//cbci.cpuAccessFlags = CPUAccessFlags::Write;

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
		vbci.flags = BufferFlags::VertexBuffer;
		vbci.size = sizeof(CommonVertex) * verticesSize;
		vbci.memoryData = cubeVertices;
		vbci.stride = sizeof(CommonVertex);

		auto resource = CreateBuffer(vbci);
		if (!resource.has_value())
		{
			Fatal(resource.error());
			return false;
		}
		m_vertexBuffer = resource.value();
	}

	// Create IndexBuffer
	{
		BufferCreateInfo ibci{};
		ibci.flags = BufferFlags::IndexBuffer;
		ibci.size = sizeof(uint32_t) * indicesSize;
		ibci.memoryData = cubeIndices;
		ibci.stride = sizeof(uint32_t);

		auto resource = CreateBuffer(ibci);
		if (!resource.has_value())
		{
			Fatal(resource.error());
			return false;
		}
		m_indexBuffer = resource.value();
	}

	Texture2DCreateInfo tci;
	tci.width = 1600;
	tci.height = 900;
	tci.flags = TextureFlags::RenderTarget;
	auto resource = CreateTexture2D(tci);
	m_rt = resource.value();

	tci.format = TexelsFormat::D32F;
	tci.flags = TextureFlags::DepthStencil;
	resource = CreateTexture2D(tci);
	m_ds = resource.value();

	m_rtv = CreateRenderTarget({ m_rt }, m_ds).value();


	RenderTargetCreateInfo rtci;
	rtci.colorBuffers[0].enable = true;
	rtci.width = 1600;
	rtci.height = 900;

	m_rtv = CreateRenderTarget(rtci).value();

	return true;
}
//=============================================================================
void DemoCube::OnClose()
{
	DeleteRHIResource(m_shaderProgram);
	DeleteRHIResource(m_pipelineState);
	DeleteRHIResource(m_constantBuffer);
	DeleteRHIResource(m_vertexBuffer);
	DeleteRHIResource(m_indexBuffer);
}
//=============================================================================
void DemoCube::OnUpdate(double deltaTime)
{
	// Update our time
	static float t = 0.0f;
	t += deltaTime;

	glm::mat4 projection = glm::perspective(glm::pi<float>() / 2.0F, GetWindowAspect(), 0.01f, 100.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0F, 1.7F, -5.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F));
	glm::mat4 world = glm::rotate(glm::mat4(1.0F), t, glm::vec3(0.0F, 1.0F, 0.0F));

	glm::mat4 mvp = projection * view * world;

	DemoConstantBuffer constants;
	std::memcpy(constants.mvp, glm::value_ptr(mvp), sizeof(constants.mvp));
	UpdateBuffer(m_constantBuffer, &constants);
}
//=============================================================================
void DemoCube::OnFrame()
{
	SetRenderTarget(m_rtv, Color{ 0.9f, 0.584313750f, 0.929411829f, 1.f }, 1.0f, 0);
	BindShaderProgram(m_shaderProgram);
	BindPipelineState(m_pipelineState);
	BindConstantBuffer(m_constantBuffer, ShaderType::Vertex, 0);
	BindVertexBuffer(m_vertexBuffer, 0);
	BindIndexBuffer(m_indexBuffer);
	DrawIndexed(PrimitiveTopology::TriangleList, 36, 0, 0);


	SetMainFrame(Color{ 0.392156899f, 0.584313750f, 0.929411829f, 1.f }, 1.0f, 0);
	BindShaderProgram(m_shaderProgram);
	BindPipelineState(m_pipelineState);
	BindConstantBuffer(m_constantBuffer, ShaderType::Vertex, 0);
	BindVertexBuffer(m_vertexBuffer, 0);
	BindIndexBuffer(m_indexBuffer);
	DrawIndexed(PrimitiveTopology::TriangleList, 36, 0, 0);
}
//=============================================================================