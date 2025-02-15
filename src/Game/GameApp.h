#pragma once

class GameApp final : public CuteEngineApp
{
public:
	bool OnInit() final;
	void OnClose() final;
	void OnFrame() final;

private:
	ShaderProgramPtr m_shaderProgram{ nullptr };
	PipelineStatePtr m_pipelineState{ nullptr };
};