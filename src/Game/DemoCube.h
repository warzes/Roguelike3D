#pragma once

class DemoCube final : public CuteEngineApp
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

	rhi::TexturePtr m_rt;
	rhi::TexturePtr m_ds;
	rhi::RenderTargetPtr m_rtv;
};