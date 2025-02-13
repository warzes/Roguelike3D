#pragma once

struct Shader final
{
	Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
};