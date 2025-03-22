#include "stdafx.h"
#include "004_Light.h"
#include "Vertex.h"
#include "Transform.h"
#include <tiny_obj_loader.h>
#include <stb/stb_image.h>
#include <imgui/imgui.h>
//=============================================================================
namespace
{
	struct alignas(16) sTransform final
	{
		Vec3 location;
		Vec3 rotation;
		Vec3 scale;
	};

	struct SubMeshData final
	{
		rhi::TexturePtr texture;
		uint32_t        indexCount;
	};

	struct alignas(16) CBufferMVP final
	{
		Mat4 model;
		Mat4 view;
		Mat4 projection;
		Mat4 inverseModel;
	};

	static sTransform sModelTransform
	{
		{ -0.330f, -0.540f, 2.070f }, // location
		{  0.0f,    150.0f, 0.0f   }, // rotation
		{  1.0f,    1.0f,   1.0f   }  // scale
	};

	struct CBufferLightSettings final
	{
		Vec4 camPos;
		Vec4 pos = { 0.9f, 0.0f, 0.6f, 1.0f };
		Vec4 ambientColor = { 0.1f, 0.1f, 0.1f, 1.0f };
		Vec4 lightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		float ambientStrength = 0.1f;
		float specularStrength = 0.7f;
		float specularPow = 256;
		float dummyPadding0;
	};

	static CBufferLightSettings sLightSettings;

	struct Mesh final
	{
		std::vector<SubMeshData> subMeshes;
		std::string debugName;
		rhi::BufferPtr vertexBuffer;
		rhi::BufferPtr indexBuffer;
	};

	static std::vector<Mesh> sMeshes;
	static uint32_t sMeshIndex = 0;

	void LoadMesh(const std::string& meshPath, Mesh& outMesh)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, meshPath.c_str());

		std::vector<VertexPosUVNormals> vertices;
		std::vector<uint32_t> indices;
		std::vector<SubMeshData> subMeshes;

		for (const auto& shape : shapes)
		{
			for (const auto& boh : shape.mesh.indices)
			{
				VertexPosUVNormals& v = vertices.emplace_back();
				v.pos.x = attrib.vertices[3 * boh.vertex_index + 0];
				v.pos.y = attrib.vertices[3 * boh.vertex_index + 1];
				v.pos.z = attrib.vertices[3 * boh.vertex_index + 2];

				if (boh.normal_index != -1)
				{
					v.normal.x = attrib.normals[3 * boh.normal_index + 0];
					v.normal.y = attrib.normals[3 * boh.normal_index + 1];
					v.normal.z = attrib.normals[3 * boh.normal_index + 2];
				}

				if (boh.texcoord_index != -1)
				{
					v.uv.x =  attrib.texcoords[2 * boh.texcoord_index + 0];
					v.uv.y = -attrib.texcoords[2 * boh.texcoord_index + 1];
				}

				indices.push_back(indices.size());
			}

			SubMeshData& s = subMeshes.emplace_back();
			s.indexCount = shape.mesh.indices.size();
			s.texture = nullptr;
		}

		rhi::BufferCreateInfo vbci{};
		vbci.flags = rhi::BufferFlags::VertexBuffer;
		vbci.elementSize = sizeof(VertexPosUVNormals);
		vbci.numElements = vertices.size();
		vbci.memoryData = vertices.data();

		if (auto result = rhi::CreateBuffer(vbci); result.has_value())
			outMesh.vertexBuffer = result.value();
		else
		{
			Fatal(result.error());
			return;
		}

		rhi::BufferCreateInfo ibci{};
		ibci.flags = rhi::BufferFlags::IndexBuffer;
		ibci.elementSize = sizeof(uint32_t);
		ibci.numElements = indices.size();
		ibci.memoryData = indices.data();

		if (auto result = rhi::CreateBuffer(ibci); result.has_value())
			outMesh.indexBuffer = result.value();
		else
		{
			Fatal(result.error());
			return;
		}

		outMesh.subMeshes = std::move(subMeshes);
		outMesh.debugName = meshPath;
	}

	void LoadTexture(const std::string& texturePath, rhi::TexturePtr& outTexture)
	{
		int texWidth, texHeight, texChannels;
		stbi_uc* texData = stbi_load(texturePath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		
		rhi::Texture2DCreateInfo tci{};
		tci.width = texWidth;
		tci.height = texHeight;
		tci.mipCount = 1;
		tci.memoryData = texData;
		if (auto result = rhi::CreateTexture2D(tci); result.has_value())
			outTexture = result.value();
		else
			Fatal(result.error());

		stbi_image_free(texData);
	}
}
//=============================================================================
bool LightDemo::OnInit()
{
	// Create ShaderProgram
	{
		rhi::ShaderProgramLoadInfo spli{};
		spli.vertexShader = { L"shaders/lightDemo_vs.hlsl", "Main" };
		spli.pixelShader = { L"shaders/lightDemo_ps.hlsl", "Main" };
		spli.inputLayout = VertexPosUVNormals::Layout;

		if (auto result = CreateShaderProgramFromFiles(spli); result.has_value())
			m_shaderProgram = result.value();
		else
		{
			Fatal(result.error());
			return false;
		}
	}

	// Create SamplerState
	{
		rhi::SamplerStateCreateInfo ssci{};
		ssci.filter = rhi::SamplerFilter::MinMagMip_Linear;
		ssci.addressU = rhi::AddressMode::Wrap;
		ssci.addressV = rhi::AddressMode::Wrap;
		ssci.addressW = rhi::AddressMode::Wrap;
		if (auto result = rhi::CreateSamplerState(ssci); result.has_value())
			m_samplerState = result.value();
		else
		{
			Fatal(result.error());
			return false;
		}
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

		if (auto result = CreatePipelineState(psci); result.has_value())
			m_pipelineState = result.value();
		else
		{
			Fatal(result.error());
			return false;
		}
	}

	// Create ConstantBuffer
	{
		rhi::ConstantBufferCreateInfo cbci{};
		cbci.usage = rhi::BufferUsage::Default;
		//cbci.cpuAccessFlags = rhi::CPUAccessFlags::Write;
		cbci.size = sizeof(CBufferMVP)/* + 0xf & 0xfffffff0*/;

		if (auto result = rhi::CreateConstantBuffer(cbci); result.has_value())
			m_cbufferMVP = result.value();
		else
		{
			Fatal(result.error());
			return false;
		}

		cbci.size = sizeof(CBufferLightSettings) + 0xf & 0xfffffff0;

		if (auto result = rhi::CreateConstantBuffer(cbci); result.has_value())
			m_cbufferLight = result.value();
		else
		{
			Fatal(result.error());
			return false;
		}
	}

	LoadMesh("meshes/dbd.obj", sMeshes.emplace_back());
	LoadMesh("meshes/cube.obj", sMeshes.emplace_back());
	LoadMesh("meshes/lamp.obj", sMeshes.emplace_back());
	LoadMesh("meshes/negan.obj", sMeshes.emplace_back());

	LoadTexture("textures/dbd/0.png", sMeshes[0].subMeshes[0].texture);
	LoadTexture("textures/dbd/1.png", sMeshes[0].subMeshes[1].texture);
	LoadTexture("textures/dbd/2.png", sMeshes[0].subMeshes[2].texture);
	LoadTexture("textures/dbd/3.png", sMeshes[0].subMeshes[3].texture);
	LoadTexture("textures/dbd/4.png", sMeshes[0].subMeshes[4].texture);
	LoadTexture("textures/dbd/5.png", sMeshes[0].subMeshes[5].texture);
	LoadTexture("textures/dbd/6.png", sMeshes[0].subMeshes[6].texture);
	LoadTexture("textures/dbd/7.png", sMeshes[0].subMeshes[7].texture);

	LoadTexture("textures/test.png", sMeshes[1].subMeshes[0].texture);

	LoadTexture("textures/lamp/0.jpg", sMeshes[2].subMeshes[0].texture);
	LoadTexture("textures/negan/0.png", sMeshes[3].subMeshes[0].texture);
	

	m_camera = Camera(Vec3(-1.038f, 1.44f, -2.0f), GetWindowAspect(), MathUtils::DegToRad(60.0f), .1f, 1000.0f);
	m_camera.LookAt(Vec3(0.0f, 0.0f, 0.0f) + Vec3(0.0f, 2.0f, 0.0f));

	return true;
}
//=============================================================================
void LightDemo::OnClose()
{
	rhi::DeleteResource(m_shaderProgram);
	rhi::DeleteResource(m_pipelineState);
	rhi::DeleteResource(m_cbufferMVP);
	rhi::DeleteResource(m_cbufferLight);
	rhi::DeleteResource(m_samplerState);
}
//=============================================================================
void LightDemo::OnUpdate(double deltaTime)
{
	m_camera.Update(this);

	constexpr float ToRadians = DirectX::XM_PI / 180.0f;

	auto model =
		DirectX::XMMatrixScaling(sModelTransform.scale.x, sModelTransform.scale.y, sModelTransform.scale.z) *
		DirectX::XMMatrixRotationRollPitchYaw(sModelTransform.rotation.x * ToRadians, sModelTransform.rotation.y * ToRadians, sModelTransform.rotation.z * ToRadians) *
		DirectX::XMMatrixTranslation(sModelTransform.location.x, sModelTransform.location.y, sModelTransform.location.z);

	auto view = m_camera.GetView().Transpose(); // CPU: row major, GPU: col major! -> We have to transpose
	auto proj = m_camera.GetViewProjection().Transpose(); // CPU: row major, GPU: col major! -> We have to transpose

	CBufferMVP mvp;
	mvp.model = DirectX::XMMatrixTranspose(model);
	mvp.view = view;
	mvp.projection = proj;
	mvp.inverseModel = DirectX::XMMatrixInverse(nullptr, model); // not transposed!

	rhi::UpdateBuffer(m_cbufferMVP, &mvp);

	// light settings
	sLightSettings.camPos = { m_camera.GetPosition().x, m_camera.GetPosition().y, m_camera.GetPosition().z, 1.0f };
	rhi::UpdateBuffer(m_cbufferLight, &sLightSettings);
}
//=============================================================================
void LightDemo::OnFrame()
{
	rhi::SetMainFrame(rhi::Color{ 0.392156899f, 0.584313750f, 0.929411829f, 1.f }, 1.0f, 0);
	
	rhi::BindShaderProgram(m_shaderProgram);
	rhi::BindSamplerState(m_samplerState, rhi::ShaderType::Pixel, 0);
	rhi::BindPipelineState(m_pipelineState);
	rhi::BindConstantBuffer(m_cbufferMVP, rhi::ShaderType::Vertex, 0);
	rhi::BindConstantBuffer(m_cbufferLight, rhi::ShaderType::Pixel, 0);

	const Mesh& mesh = sMeshes[sMeshIndex];
	rhi::BindVertexBuffer(mesh.vertexBuffer, 0);
	rhi::BindIndexBuffer(mesh.indexBuffer);

	uint32_t indexOffset = 0;
	for (auto s : sMeshes[sMeshIndex].subMeshes)
	{
		rhi::BindShaderResource(s.texture, rhi::ShaderType::Pixel, 0);
		rhi::DrawIndexed(rhi::PrimitiveTopology::TriangleList, s.indexCount, indexOffset, 0);
		indexOffset += s.indexCount;
	}


	ImGui::Begin("Settings");

	ImGui::PushID("model");
	ImGui::Text("Model");

	if (ImGui::BeginCombo("Mesh", sMeshes[sMeshIndex].debugName.c_str()))
	{
		for (size_t i = 0; i < sMeshes.size(); i++)
		{
			if (ImGui::Selectable(sMeshes[i].debugName.c_str(), i == sMeshIndex))
			{
				sMeshIndex = i;
				const Mesh& mesh = sMeshes[sMeshIndex];
				rhi::BindVertexBuffer(mesh.vertexBuffer, 0);
				rhi::BindIndexBuffer(mesh.indexBuffer);
			}
		}

		ImGui::EndCombo();
	}

	ImGui::DragFloat3("Location", &sModelTransform.location.x, 0.03f);
	ImGui::DragFloat3("Rotation", &sModelTransform.rotation.x, 0.5f);
	ImGui::DragFloat3("Scale", &sModelTransform.scale.x, 0.05f);

	ImGui::PopID();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::PushID("light");
	ImGui::Text("Light settings");
	ImGui::DragFloat3("Position", &sLightSettings.pos.x, 0.03f);
	ImGui::ColorEdit3("Ambient Color", &sLightSettings.ambientColor.x);
	ImGui::ColorEdit3("Light Color", &sLightSettings.lightColor.x);
	ImGui::DragFloat("Ambient intensity", &sLightSettings.ambientStrength);
	ImGui::DragFloat("Specular intensity", &sLightSettings.specularStrength);
	ImGui::DragFloat("Specular power", &sLightSettings.specularPow);
	ImGui::PopID();

	ImGui::End();
}
//=============================================================================