#include "stdafx.h"
#include "004_Model.h"
#include "Vertex.h"
#include "Transform.h"
#include <stb/stb_image.h>
//=============================================================================
namespace
{
	struct CBufferPerView
	{
		Mat4 mat_view;
		Mat4 mat_inv_view;
		Mat4 mat_view_projection;
		Mat4 mat_inv_view_projection;
		Vec4 pos_camera_ws;
	};

	struct CBufferPerObject
	{
		Mat4 matWorld;
	};

	struct MeshData final
	{
		static std::shared_ptr<MeshData> LoadFromFile(const std::string& asset_path);

		void Bind();

		rhi::BufferPtr vertexBuffer;
		rhi::BufferPtr indexBuffer;
		uint32_t       indexCount;
	};

	struct Mesh final
	{
		void Update(float dt);
		void Render();

		void Bind();

		float rotationAngle = 0.0f;
		Transform transform;

		std::shared_ptr<MeshData> meshData;

		CBufferPerObject perObjectData;
		rhi::ConstantBufferPtr cbufferPerObject;

		std::string name;
	};

	Mesh mesh;
	CBufferPerView perViewData;

	std::shared_ptr<MeshData> MeshData::LoadFromFile(const std::string& asset_path)
	{
		Print("Loading mesh: " + asset_path);

		std::shared_ptr<MeshData> mesh_data = std::make_shared<MeshData>();

		std::vector<uint32_t> indices;
		std::vector<VertexPosUVNormals> vertexData;

		Assimp::Importer importer;

		uint32_t importer_flags = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
		const aiScene* scene = importer.ReadFile(asset_path, importer_flags);

		if (!scene)
		{
			Fatal(importer.GetErrorString());
		}
		assert(scene != nullptr, "Failed to load mesh from file: {}", asset_path);

		for (uint32_t mesh_idx = 0; mesh_idx < scene->mNumMeshes; ++mesh_idx)
		{
			aiMesh* mesh = scene->mMeshes[mesh_idx];
			for (uint32_t vertex_id = 0; vertex_id < mesh->mNumVertices; ++vertex_id)
			{
				const aiVector3D& vertex = mesh->mVertices[vertex_id];
				const aiVector3D& normal = mesh->HasNormals() ? mesh->mNormals[vertex_id] : aiVector3D(0.0f, 0.0f, 0.0f);
				const aiVector3D& uv = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][vertex_id] : aiVector3D(0.0f, 0.0f, 0.0f);

				VertexPosUVNormals v;
				v.pos = { vertex.x, vertex.y, vertex.z };
				v.normal = { normal.x, normal.y, normal.z };
				v.uv = { uv.x, uv.y };

				vertexData.push_back(v);
			}

			for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
			{
				const aiFace& face = mesh->mFaces[i];
				assert(face.mNumIndices == 3);
				indices.push_back(face.mIndices[0]);
				indices.push_back(face.mIndices[1]);
				indices.push_back(face.mIndices[2]);
			}
		}

		rhi::BufferCreateInfo vbci{};
		vbci.flags = rhi::BufferFlags::VertexBuffer;
		vbci.elementSize = sizeof(VertexPosUVNormals);
		vbci.numElements = vertexData.size();
		vbci.memoryData = vertexData.data();

		auto resourcevb = rhi::CreateBuffer(vbci);
		if (!resourcevb.has_value())
		{
			Fatal(resourcevb.error());
			return nullptr;
		}
		mesh_data->vertexBuffer = resourcevb.value();

		rhi::BufferCreateInfo ibci{};
		ibci.flags = rhi::BufferFlags::IndexBuffer;
		ibci.elementSize = sizeof(uint32_t);
		ibci.numElements = indices.size();
		ibci.memoryData = indices.data();

		auto resourceib = rhi::CreateBuffer(ibci);
		if (!resourceib.has_value())
		{
			Fatal(resourceib.error());
			return nullptr;
		}
		mesh_data->indexBuffer = resourceib.value();

		mesh_data->indexCount = indices.size();

		return mesh_data;
	}

	void MeshData::Bind()
	{
		rhi::BindVertexBuffer(vertexBuffer, 0);
		rhi::BindIndexBuffer(indexBuffer);
	}

	void Mesh::Update(float dt)
	{
		Quat rot = transform.GetWorldRotation() * Quat::FromAxisAngle(Vec3::UP, MathUtils::DegToRad(40.5f) * dt);
		rot.Normalize();
		transform.SetWorldRotation(rot);
		perObjectData.matWorld = transform.GetWorldMatrix().Transpose();
	}

	void Mesh::Bind()
	{
		meshData->Bind();

		if (cbufferPerObject == nullptr)
		{
			rhi::ConstantBufferCreateInfo cbci{};
			cbci.usage = rhi::BufferUsage::Default;
			cbci.size = sizeof(CBufferPerObject)/* + 0xf & 0xfffffff0*/;

			auto resource = rhi::CreateConstantBuffer(cbci);
			if (!resource.has_value())
			{
				Fatal(resource.error());
				return;
			}
			cbufferPerObject = resource.value();
		}

		rhi::UpdateBuffer(cbufferPerObject, &perObjectData);
		static constexpr int CBUFFER_PER_OBJECT_SLOT = 1;
		rhi::BindConstantBuffer(cbufferPerObject, rhi::ShaderType::Vertex, CBUFFER_PER_OBJECT_SLOT);
		rhi::BindConstantBuffer(cbufferPerObject, rhi::ShaderType::Pixel, CBUFFER_PER_OBJECT_SLOT);
	}

	void Mesh::Render()
	{
		Bind();
		rhi::DrawIndexed(rhi::PrimitiveTopology::TriangleList, meshData->indexCount, 0, 0);
	}
}
//=============================================================================
bool ModelDemo::OnInit()
{
	// Create ShaderProgram
	{
		rhi::ShaderProgramLoadInfo spli{};
		spli.vertexShader = { L"shaders/meshDemo_vs.hlsl", "Main" };
		spli.pixelShader = { L"shaders/meshDemo_ps.hlsl", "Main" };
		spli.inputLayout = VertexPosUVNormals::Layout;

		if (auto result = CreateShaderProgramFromFiles(spli); result.has_value())
		{
			m_shaderProgram = result.value();
		}
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

		auto resource = CreatePipelineState(psci);
		if (!resource.has_value())
		{
			Fatal(resource.error());
			return false;
		}
		m_pipelineState = resource.value();
	}


	// Create Texture2D
	{
		int texWidth, texHeight, texChannels;
		stbi_uc* texData = stbi_load("textures/BOSS_texture_final.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
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
		ssci.addressU = rhi::AddressMode::Wrap;
		ssci.addressV = rhi::AddressMode::Wrap;
		ssci.addressW = rhi::AddressMode::Wrap;
		auto resource = rhi::CreateSamplerState(ssci);
		if (!resource.has_value())
		{
			Fatal(resource.error());
			return false;
		}
		m_samplerState = resource.value();
	}

	// Create ConstantBuffer
	{
		rhi::ConstantBufferCreateInfo cbci{};
		cbci.usage = rhi::BufferUsage::Default;
		cbci.size = sizeof(CBufferPerView)/* + 0xf & 0xfffffff0*/;

		auto resource = rhi::CreateConstantBuffer(cbci);
		if (!resource.has_value())
		{
			Fatal(resource.error());
			return false;
		}
		m_cbufferPerView = resource.value();
	}

	mesh.meshData = MeshData::LoadFromFile("meshes/BOSS_model_final.fbx");
	mesh.transform.SetWorldTranslation({ 0.0f, -1.0f, 0.0f });

	m_camera = Camera(Vec3(0.0f, 5.0f, -10.0f), GetWindowAspect(), MathUtils::DegToRad(45.0f), .1f, 1000.0f);
	m_camera.LookAt(Vec3(0.0f, 0.0f, 0.0f) + Vec3(0.0f, 2.0f, 0.0f));

	return true;
}
//=============================================================================
void ModelDemo::OnClose()
{
	rhi::DeleteResource(m_shaderProgram);
	rhi::DeleteResource(m_pipelineState);
	rhi::DeleteResource(m_cbufferPerView);
	rhi::DeleteResource(m_texture);
	rhi::DeleteResource(m_samplerState);
}
//=============================================================================
void ModelDemo::OnUpdate(double deltaTime)
{
	mesh.Update(deltaTime);

	// Update per-view cbuffer
	perViewData.mat_view = m_camera.GetView().Transpose(); // CPU: row major, GPU: col major! -> We have to transpose
	perViewData.mat_inv_view = m_camera.GetView().Invert().Transpose();
	perViewData.mat_view_projection = m_camera.GetViewProjection().Transpose();
	perViewData.mat_inv_view_projection = m_camera.GetViewProjection().Invert().Transpose();
	perViewData.pos_camera_ws = Vec4(m_camera.GetPosition());

	rhi::UpdateBuffer(m_cbufferPerView, &perViewData);
}
//=============================================================================
void ModelDemo::OnFrame()
{
	rhi::SetMainFrame(rhi::Color{ 0.392156899f, 0.584313750f, 0.929411829f, 1.f }, 1.0f, 0);
	rhi::BindShaderProgram(m_shaderProgram);
	rhi::BindPipelineState(m_pipelineState);

	rhi::BindConstantBuffer(m_cbufferPerView, rhi::ShaderType::Vertex, 0);
	rhi::BindConstantBuffer(m_cbufferPerView, rhi::ShaderType::Pixel, 0);

	rhi::BindSamplerState(m_samplerState, rhi::ShaderType::Pixel, 0);
	rhi::BindShaderResource(m_texture, rhi::ShaderType::Pixel, 0);

	mesh.Render();
}
//=============================================================================