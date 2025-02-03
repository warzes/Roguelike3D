#pragma once

struct ShaderProgram final
{
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};

struct ComputeProgram final
{
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> computeShader;
};

struct MeshBuffer final
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	UINT stride;
	UINT offset;
	UINT numElements;
};

struct FrameBuffer final
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> colorTexture;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilTexture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> dsv;
	UINT width, height;
	UINT samples;
};
