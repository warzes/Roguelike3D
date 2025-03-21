﻿#include "stdafx.h"
#include "002_TexturedCube.h"
#include "Vertex.h"
#include <stb/stb_image.h>
//=============================================================================
namespace
{
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
bool TexturedCube::OnInit()
{
	// Create ShaderProgram
	{
		rhi::ShaderProgramLoadInfo spli{};
		spli.vertexShader = { L"TexturedCube.hlsl", "vs_main" };
		spli.pixelShader = { L"TexturedCube.hlsl", "ps_main" };
		spli.inputLayout = VertexPosUV::Layout;

		auto resource = CreateShaderProgramFromFiles(spli);
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

		psci.blendState.blendDesc.enable = true;
		psci.blendState.blendDesc.srcColor = rhi::BlendFactor::SrcAlpha;
		psci.blendState.blendDesc.dstColor = rhi::BlendFactor::InvSrcAlpha;
		psci.blendState.blendDesc.opColor = rhi::BlendOp::Add;
		psci.blendState.blendDesc.srcAlpha = rhi::BlendFactor::One;
		psci.blendState.blendDesc.dstAlpha = rhi::BlendFactor::One;
		psci.blendState.blendDesc.opAlpha = rhi::BlendOp::Add;
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
		vbci.elementSize = sizeof(VertexPosUV);
		vbci.numElements = _countof(cube_textured_vertices_);
		vbci.memoryData = cube_textured_vertices_;

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
		ibci.elementSize = sizeof(uint32_t);
		ibci.numElements = _countof(cube_tex_indices_);
		ibci.memoryData = cube_tex_indices_;

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
		ssci.filter = rhi::SamplerFilter::MinMagMip_Linear;
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
void TexturedCube::OnClose()
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
void TexturedCube::OnUpdate(double deltaTime)
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
void TexturedCube::OnFrame()
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