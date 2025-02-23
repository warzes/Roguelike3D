#pragma once

class GameApp final : public CuteEngineApp
{
public:
	bool OnInit() final;
	void OnClose() final;
	void OnUpdate(double deltaTime) final;
	void OnFrame() final;

private:
	ShaderProgramPtr  m_shaderProgram{ nullptr };
	PipelineStatePtr  m_pipelineState{ nullptr };
	SamplerStatePtr   m_samplerState{ nullptr };
	ConstantBufferPtr m_constantBuffer{ nullptr };

	BufferPtr         m_vertexBuffer{ nullptr };
	BufferPtr         m_instanceRotationBuffer{ nullptr };
	BufferPtr         m_instanceColorBuffer{ nullptr };
	BufferPtr         m_indexBuffer{ nullptr };

	TexturePtr        m_texture{ nullptr };
};