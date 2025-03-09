#pragma once

#include "Camera.h"


class MeshDemo final : public CuteEngineApp
{
public:
	bool OnInit() final;
	void OnClose() final;
	void OnUpdate(double deltaTime) final;
	void OnFrame() final;

private:
	rhi::ShaderProgramPtr  m_shaderProgram{ nullptr };


	rhi::PipelineStatePtr  m_pipelineState{ nullptr };
	rhi::ConstantBufferPtr m_constantBuffer{ nullptr };

	rhi::BufferPtr         m_vertexBuffer{ nullptr };
	rhi::BufferPtr         m_indexBuffer{ nullptr };

	Camera                 m_camera;
	rhi::TexturePtr        m_texture;
	rhi::SamplerStatePtr   m_samplerState{ nullptr };
};