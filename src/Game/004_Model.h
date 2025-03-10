#pragma once

#include "Camera.h"

class ModelDemo final : public CuteEngineApp
{
public:
	bool OnInit() final;
	void OnClose() final;
	void OnUpdate(double deltaTime) final;
	void OnFrame() final;

private:
	rhi::ConstantBufferPtr m_cbufferPerView{ nullptr };








	rhi::ShaderProgramPtr  m_shaderProgram{ nullptr };

	rhi::PipelineStatePtr  m_pipelineState{ nullptr };

	Camera                 m_camera;
	rhi::TexturePtr        m_texture;
	rhi::SamplerStatePtr   m_samplerState{ nullptr };
};