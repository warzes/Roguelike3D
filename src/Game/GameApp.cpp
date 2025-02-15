#include "stdafx.h"
#include "GameApp.h"
//=============================================================================
bool GameApp::OnInit()
{
	// Create ShaderProgram
	{
		ShaderProgramLoadInfo spli{};
		spli.vertexShader = { L"shaders.hlsl", "vs_main", "vs_5_0" };
		spli.pixelShader =  { L"shaders.hlsl", "ps_main", "ps_5_0" };
		spli.inputLayout = { 
			// float3 position, float3 normal, float2 texcoord, uint3 rotation, float3 color
			{"POS", 0, DataFormat::RGB32F, 0, 0,                    false},
			{"NOR", 0, DataFormat::RGB32F, 0, AppendAlignedElement, false},
			{"TEX", 0, DataFormat::RG32F,  0, AppendAlignedElement, false},

			{"ROT", 0, DataFormat::RGB32U, 1, AppendAlignedElement, true, 1}, // advance every instance
			{"COL", 0, DataFormat::RGB32F, 2, AppendAlignedElement, true, 4}, // advance every 4th instance, i.e. every face
		};

		auto sp = LoadShaderProgram(spli);
		if (!sp.has_value())
		{
			Fatal(sp.error());
			return false;
		}
		m_shaderProgram = sp.value();
	}

	// Create PipelineState
	{
		PipelineStateCreateInfo psci{};
	}




	return true;
}
//=============================================================================
void GameApp::OnClose()
{
	DeleteRHIResource(m_shaderProgram);
}
//=============================================================================
void GameApp::OnFrame()
{
	SetMainFrame();
	BindShaderProgram(m_shaderProgram);
	Temp();
}
//=============================================================================