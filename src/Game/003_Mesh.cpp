#include "stdafx.h"
#include "003_Mesh.h"
#include "Vertex.h"
#include "Transform.h"
#include <stb/stb_image.h>
//=============================================================================
namespace
{
	struct CBufferPerObject
	{
		Mat4 matWorld;
	};

	struct MeshData final
	{
		static std::shared_ptr<MeshData> LoadFromFile(const std::string& asset_path);

		void Bind();

		rhi::BufferPtr pos;
		rhi::BufferPtr uv;
		rhi::BufferPtr normals;
		rhi::BufferPtr indexBuffer;
	};

	struct Mesh final
	{
		void Update(float dt);
		void Render();

		void Bind();

		float rotationAngle = 0.0f;
		Transform transform;

		std::shared_ptr<MeshData> meshData;
		//std::shared_ptr<Material> material;

		CBufferPerObject perObjectData;
		rhi::ConstantBufferPtr cbufferPerObject;

		std::string name;
	};




	VertexPosUV cube_textured_vertices_[4 * 6] =
	{
		// front
		{ Vec3(-1.0f, -1.0f,  -1.0f), Vec2(0.0f, 1.0f) },
		{ Vec3(-1.0f,  1.0f,  -1.0f), Vec2(0.0f, 0.0f) },
		{ Vec3(1.0f,  1.0f,   -1.0f), Vec2(1.0f, 0.0f) },
		{ Vec3(1.0f, -1.0f,   -1.0f), Vec2(1.0f, 1.0f) },

		// back
		{ Vec3(-1.0f, -1.0f, 1.0f), Vec2(1.0f, 1.0f) },
		{ Vec3(1.0f, -1.0f, 1.0f), Vec2(0.0f, 1.0f) },
		{ Vec3(1.0f,  1.0f, 1.0f), Vec2(0.0f, 0.0f) },
		{ Vec3(-1.0f,  1.0f, 1.0f), Vec2(1.0f, 0.0f) },

		// top
		{ Vec3(-1.0f, 1.0f, -1.0f), Vec2(0.0f, 1.0f) },
		{ Vec3(-1.0f, 1.0f,  1.0f), Vec2(0.0f, 0.0f) },
		{ Vec3(1.0f, 1.0f,  1.0f),  Vec2(1.0f, 0.0f) },
		{ Vec3(1.0f, 1.0f, -1.0f),  Vec2(1.0f, 1.0f) },

		// bottom
		{ Vec3(-1.0f, -1.0f, -1.0f), Vec2(1.0f, 1.0f) },
		{ Vec3(1.0f, -1.0f, -1.0f),  Vec2(0.0f, 1.0f) },
		{ Vec3(1.0f, -1.0f,  1.0f),  Vec2(0.0f, 0.0f) },
		{ Vec3(-1.0f, -1.0f,  1.0f), Vec2(1.0f, 0.0f) },

		// left
		{ Vec3(-1.0f, -1.0f,  1.0f), Vec2(0.0f, 1.0f) },
		{ Vec3(-1.0f,  1.0f,  1.0f), Vec2(0.0f, 0.0f) },
		{ Vec3(-1.0f,  1.0f, -1.0f), Vec2(1.0f, 0.0f) },
		{ Vec3(-1.0f, -1.0f, -1.0f), Vec2(1.0f, 1.0f) },

		// right
		{ Vec3(1.0f, -1.0f, -1.0f), Vec2(0.0f, 1.0f) },
		{ Vec3(1.0f,  1.0f, -1.0f), Vec2(0.0f, 0.0f) },
		{ Vec3(1.0f,  1.0f,  1.0f), Vec2(1.0f, 0.0f) },
		{ Vec3(1.0f, -1.0f,  1.0f), Vec2(1.0f, 1.0f) }
	};

	uint32_t cube_tex_indices_[2 * 3 * 6] = {
		// front
		0,  1,  2,
		0,  2,  3,

		// back
		4,  5,  6,
		4,  6,  7,

		// top
		8,  9, 10,
		8, 10, 11,

		// bottom
		12, 13, 14,
		12, 14, 15,

		// left
		16, 17, 18,
		16, 18, 19,

		// right
		20, 21, 22,
		20, 22, 23
	};

	struct CBufferPerCube final
	{
		Mat4 wvp;
		float alpha = 0.0f;
		float padding[3];
	};

	CBufferPerCube per_object_data_;
}
//=============================================================================
bool MeshDemo::OnInit()
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

		rhi::ShaderProgramLoadInfo spli{};
		spli.vertexShader = { L"TexturedCube.hlsl", "vs_main" };
		spli.pixelShader = { L"TexturedCube.hlsl", "ps_main" };
		spli.inputLayout = VertexPosUV::Layout;

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
		rhi::PipelineStateCreateInfo psci{};

		psci.depthStencilState.depthEnabled = true;
		psci.depthStencilState.writeMask = rhi::DepthWriteMask::All;
		psci.depthStencilState.depthFunc = rhi::DepthFunc::Less;

		psci.rasterizerState.fillMode = rhi::FillMode::Solid;
		psci.rasterizerState.cullMode = rhi::CullMode::Back;
		psci.rasterizerState.frontCounterClockwise = false;

		psci.blendState.blendDesc.blendEnabled = true;
		psci.blendState.blendDesc.srcBlend = rhi::BlendFactor::SrcAlpha;
		psci.blendState.blendDesc.dstBlend = rhi::BlendFactor::OneMinusSrcAlpha;
		psci.blendState.blendDesc.blendOp = rhi::BlendOp::Add;
		psci.blendState.blendDesc.srcBlendAlpha = rhi::BlendFactor::One;
		psci.blendState.blendDesc.dstBlendAlpha = rhi::BlendFactor::One;
		psci.blendState.blendDesc.blendOpAlpha = rhi::BlendOp::Add;
		psci.blendState.blendDesc.writeMask = rhi::ColorWriteMask::All;

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
		rhi::ConstantBufferCreateInfo cbci{};
		cbci.usage = rhi::BufferUsage::Default;
		cbci.size = sizeof(CBufferPerCube)/* + 0xf & 0xfffffff0*/;

		auto resource = rhi::CreateConstantBuffer(cbci);
		if (!resource.has_value())
		{
			Fatal(resource.error());
			return false;
		}
		m_constantBuffer = resource.value();
	}

	// Create VertexBuffer
	{
		rhi::BufferCreateInfo vbci{};
		vbci.flags = rhi::BufferFlags::VertexBuffer;
		vbci.size = sizeof(VertexPosUV) * _countof(cube_textured_vertices_);
		vbci.memoryData = cube_textured_vertices_;
		vbci.stride = sizeof(VertexPosUV);

		auto resource = rhi::CreateBuffer(vbci);
		if (!resource.has_value())
		{
			Fatal(resource.error());
			return false;
		}
		m_vertexBuffer = resource.value();
	}

	// Create IndexBuffer
	{
		rhi::BufferCreateInfo ibci{};
		ibci.flags = rhi::BufferFlags::IndexBuffer;
		ibci.size = sizeof(uint32_t) * _countof(cube_tex_indices_);
		ibci.memoryData = cube_tex_indices_;
		ibci.stride = sizeof(uint32_t);

		auto resource = rhi::CreateBuffer(ibci);
		if (!resource.has_value())
		{
			Fatal(resource.error());
			return false;
		}
		m_indexBuffer = resource.value();
	}

	// Create Texture2D
	{
		int texWidth, texHeight, texChannels;
		stbi_uc* texData = stbi_load("test_texture.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		int imagePitch = texWidth * 4 * sizeof(uint8_t);

		rhi::Texture2DCreateInfo tci{};
		tci.width = texWidth;
		tci.height = texHeight;
		tci.mipCount = 1;
		tci.memoryData = texData;

		auto resource = rhi::CreateTexture2D(tci);
		if (!resource.has_value())
		{
			Fatal(resource.error());
			return false;
		}
		m_texture = resource.value();

		stbi_image_free(texData);
	}

	// Create SamplerState
	{
		rhi::SamplerStateCreateInfo ssci{};
		ssci.filter = rhi::TextureFilter::MinMagMip_Linear;
		ssci.addressU = rhi::AddressMode::Clamp;
		ssci.addressV = rhi::AddressMode::Clamp;
		ssci.addressW = rhi::AddressMode::Clamp;
		auto resource = rhi::CreateSamplerState(ssci);
		if (!resource.has_value())
		{
			Fatal(resource.error());
			return false;
		}
		m_samplerState = resource.value();
	}

	m_camera = Camera(Vec3(0.0f, 0.0f, -10.0f), GetWindowAspect(), MathUtils::DegToRad(45.0f), 0.1f, 1000.0f);

	return true;
}
//=============================================================================
void MeshDemo::OnClose()
{
	rhi::DeleteResource(m_shaderProgram);
	rhi::DeleteResource(m_pipelineState);
	rhi::DeleteResource(m_constantBuffer);
	rhi::DeleteResource(m_vertexBuffer);
	rhi::DeleteResource(m_indexBuffer);
	rhi::DeleteResource(m_texture);
	rhi::DeleteResource(m_samplerState);
}
//=============================================================================
void MeshDemo::OnUpdate(double deltaTime)
{
	// Update our time
	static float angle = 0.0f;
	angle += 20.5f * deltaTime;

	// Update the cbuffer
	Mat4 mat_world = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(angle)) * DirectX::XMMatrixTranslation(.5f, 0.0f, 0.5f);
	Mat4 mat_wvp = mat_world * m_camera.GetViewProjection();
	per_object_data_.wvp = mat_wvp.Transpose(); // CPU: row major, GPU: col major! -> We have to transpose.
	per_object_data_.alpha = 0.25f;
	UpdateBuffer(m_constantBuffer, &per_object_data_);
}
//=============================================================================
void MeshDemo::OnFrame()
{
	rhi::SetMainFrame(rhi::Color{ 0.392156899f, 0.584313750f, 0.929411829f, 1.f }, 1.0f, 0);
	rhi::BindShaderProgram(m_shaderProgram);
	rhi::BindPipelineState(m_pipelineState);

	rhi::BindConstantBuffer(m_constantBuffer, rhi::ShaderType::Vertex, 0);
	rhi::BindConstantBuffer(m_constantBuffer, rhi::ShaderType::Pixel, 0);
	rhi::BindSamplerState(m_samplerState, rhi::ShaderType::Pixel, 0);
	rhi::BindShaderResource(m_texture, rhi::ShaderType::Pixel, 0);

	rhi::BindVertexBuffer(m_vertexBuffer, 0);
	rhi::BindIndexBuffer(m_indexBuffer);
	rhi::DrawIndexed(rhi::PrimitiveTopology::TriangleList, _countof(cube_tex_indices_), 0, 0);
}
//=============================================================================