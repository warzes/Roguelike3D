#pragma once

struct float4 { float x, y, z, w; };

struct Constants
{
	float4 projection[4];
	float4 lightVector;
	float4 rotate;
	float4 scale;
	float4 translate;
};

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

	VertexBufferPtr   m_vertexBuffer{ nullptr };
	VertexBufferPtr   m_instanceRotationBuffer{ nullptr };
	VertexBufferPtr   m_instanceColorBuffer{ nullptr };
	IndexBufferPtr    m_indexBuffer{ nullptr };

	Texture2DPtr      m_texture{ nullptr };
};