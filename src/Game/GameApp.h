#pragma once

class GameApp final : public CuteEngineApp
{
public:
	bool OnInit() final;
	void OnClose() final;
	void OnUpdate(double deltaTime) final;
	void OnFrame() final;

private:
	rhi::ShaderProgramPtr  m_shaderProgram{ nullptr };
	rhi::PipelineStatePtr  m_pipelineState{ nullptr };
	rhi::SamplerStatePtr   m_samplerState{ nullptr };
	rhi::ConstantBufferPtr m_constantBuffer{ nullptr };

	rhi::BufferPtr         m_vertexBuffer{ nullptr };
	rhi::BufferPtr         m_instanceRotationBuffer{ nullptr };
	rhi::BufferPtr         m_instanceColorBuffer{ nullptr };
	rhi::BufferPtr         m_indexBuffer{ nullptr };

	rhi::TexturePtr        m_texture{ nullptr };
};