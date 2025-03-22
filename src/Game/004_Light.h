#pragma once

#include "Camera.h"

class LightDemo final : public CuteEngineApp
{
public:
	bool OnInit() final;
	void OnClose() final;
	void OnUpdate(double deltaTime) final;
	void OnFrame() final;

private:
	rhi::ConstantBufferPtr m_cbufferMVP{ nullptr };
	rhi::ConstantBufferPtr m_cbufferLight{ nullptr };
	rhi::ShaderProgramPtr  m_shaderProgram{ nullptr };
	rhi::PipelineStatePtr  m_pipelineState{ nullptr };
	Camera                 m_camera;
	rhi::SamplerStatePtr   m_samplerState{ nullptr };
};