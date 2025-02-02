#include "stdafx.h"
//=============================================================================
#if defined(_MSC_VER)
#	pragma comment( lib, "3rdparty.lib" )
#	pragma comment( lib, "Engine.lib" )
#endif
//=============================================================================
void RequestExit() {}
//=============================================================================
#define TEXTURE_WIDTH  2
#define TEXTURE_HEIGHT 2

// 2x2 pixel checkerboard pattern, 0xAARRGGBB
UINT TextureData[] = { 0xffffffff, 0xff7f7f7f, 0xff7f7f7f, 0xffffffff }; 

float VertexData[] = // pos.x, pos.y, pos.z, nor.x, nor.y, nor.z, tex.u, tex.v, ...
{
	-1.00f,  1.00f, -1.00f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.00f,  1.00f, -1.00f,  0.0f,  0.0f, -1.0f,  9.5f,  0.0f,
	 0.58f,  0.58f, -1.00f,  0.0f,  0.0f, -1.0f,  7.5f,  2.0f, -0.58f,  0.58f, -1.00f,  0.0f,  0.0f, -1.0f,  2.0f,  2.0f,
	-0.58f,  0.58f, -1.00f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.58f,  0.58f, -1.00f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	 0.58f,  0.58f, -0.58f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f, -0.58f,  0.58f, -0.58f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
};

UINT IndexData[] = { 0, 1, 3, 1, 2, 3, 4, 5, 7, 5, 6, 7 };

UINT InstanceRotationData[] = { 0, 0, 0, 0, 0, 1, 0, 0, 2, 0, 0, 3, 1, 0, 0, 1, 1, 0, 1, 2, 0, 1, 3, 0, 2, 0, 0, 2, 0, 1, 2, 0, 2, 2, 0, 3, 3, 0, 0, 3, 1, 0, 3, 2, 0, 3, 3, 0, 1, 0, 1, 1, 1, 1, 1, 2, 1, 1, 3, 1, 1, 0, 3, 1, 1, 3, 1, 2, 3, 1, 3, 3 }; // rot.x, rot.y, rot.z, ... in multiples of 90 degrees

float InstanceColorData[] = { 0.973f, 0.480f, 0.002f, 0.897f, 0.163f, 0.011f, 0.612f, 0.000f, 0.069f, 0.127f, 0.116f, 0.408f, 0.000f, 0.254f, 0.637f, 0.001f, 0.447f, 0.067f }; // col.r, col.g, col.b, ...

//=============================================================================
int main()
{
	if (!DirectX::XMVerifyCPUSupport())
		return 1;

	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
		return 1;

	LogSystemCreateInfo logCI;
	LogSystem log;
	log.Create(logCI);

	WindowSystemCreateInfo windowCI;
	WindowSystem window;
	window.Create(windowCI);

	RHISystemCreateInfo rhiCI;
	rhiCI.context.hwnd = window.GetHWND();
	rhiCI.context.width = window.GetWidth();
	rhiCI.context.height = window.GetHeight();
	RHISystem rhi;
	rhi.Create(rhiCI);

	auto device = rhi.GetContext().GetD3DDevice();
	auto deviceContext = rhi.GetContext().GetD3DContext();

	ID3DBlob* vertexShaderCSO;
	D3DCompileFromFile(L"gpu.hlsl", nullptr, nullptr, "vs_main", "vs_5_0", 0, 0, &vertexShaderCSO, nullptr);
	ID3D11VertexShader* vertexShader;
	device->CreateVertexShader(vertexShaderCSO->GetBufferPointer(), vertexShaderCSO->GetBufferSize(), nullptr, &vertexShader);

	ID3DBlob* pixelShaderCSO;
	D3DCompileFromFile(L"gpu.hlsl", nullptr, nullptr, "ps_main", "ps_5_0", 0, 0, &pixelShaderCSO, nullptr);
	ID3D11PixelShader* pixelShader;
	device->CreatePixelShader(pixelShaderCSO->GetBufferPointer(), pixelShaderCSO->GetBufferSize(), nullptr, &pixelShader);

	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = // float3 position, float3 normal, float2 texcoord, uint3 rotation, float3 color
	{
		{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA,   0 },
		{ "NOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,   0 },
		{ "TEX", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,   0 },
		{ "ROT", 0, DXGI_FORMAT_R32G32B32_UINT,  1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 }, // advance every instance
		{ "COL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 4 }, // advance every 4th instance, i.e. every face
	};

	ID3D11InputLayout* inputLayout;
	device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), vertexShaderCSO->GetBufferPointer(), vertexShaderCSO->GetBufferSize(), &inputLayout);

	D3D11_RASTERIZER_DESC1 rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	ID3D11RasterizerState1* rasterizerState;
	device->CreateRasterizerState1(&rasterizerDesc, &rasterizerState);

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	ID3D11SamplerState* samplerState;
	device->CreateSamplerState(&samplerDesc, &samplerState);

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	ID3D11DepthStencilState* depthStencilState;
	device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);

	struct float4 { float x, y, z, w; };

	struct Constants
	{
		float4 Projection[4];
		float4 LightVector;
		float4 Rotate;
		float4 Scale;
		float4 Translate;
	};

	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.ByteWidth = sizeof(Constants) + 0xf & 0xfffffff0; // ensure constant buffer size is multiple of 16 bytes
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC; // will be updated from CPU every frame
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ID3D11Buffer* constantBuffer;
	device->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer);

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.ByteWidth = sizeof(VertexData);
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE; // will never be updated 
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexData = { VertexData };

	ID3D11Buffer* vertexBuffer;
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.ByteWidth = sizeof(IndexData);
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE; // will never be updated
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexData = { IndexData };

	ID3D11Buffer* indexBuffer;
	device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

	D3D11_BUFFER_DESC instanceBufferDesc = {};
	instanceBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.ByteWidth = sizeof(InstanceRotationData);

	D3D11_SUBRESOURCE_DATA instanceData = { InstanceRotationData };

	ID3D11Buffer* instanceRotationBuffer;
	device->CreateBuffer(&instanceBufferDesc, &instanceData, &instanceRotationBuffer);


	instanceBufferDesc.ByteWidth = sizeof(InstanceColorData);
	instanceData.pSysMem = { InstanceColorData };
	ID3D11Buffer* instanceColorBuffer;
	device->CreateBuffer(&instanceBufferDesc, &instanceData, &instanceColorBuffer);

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = 2;
	textureDesc.Height = 2;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA textureData = {};
	textureData.pSysMem = TextureData;
	textureData.SysMemPitch = 2 * sizeof(UINT); // texture is 2 pixels wide, 4 bytes per pixel

	ID3D11Texture2D* texture;
	device->CreateTexture2D(&textureDesc, &textureData, &texture);

	ID3D11ShaderResourceView* textureView;
	device->CreateShaderResourceView(texture, nullptr, &textureView);

	ID3D11Buffer* buffers[] = { vertexBuffer, instanceRotationBuffer, instanceColorBuffer };
	
	UINT strides[] = { 8 * sizeof(float), 3 * sizeof(UINT), 3 * sizeof(float) }; // vertex (float3 position, float3 normal, float2 texcoord), instance rotation (uint3 rotation), instance color (float3 color)
	UINT offsets[] = { 0, 0, 0 };


	float w = (float)window.GetWidth() / (float)window.GetHeight(); // width (aspect ratio)
	float h = 1.0f;                             // height
	float n = 1.0f;                             // near
	float f = 9.0f;                             // far

	Constants constants = { 2 * n / w, 0, 0, 0, 0, 2 * n / h, 0, 0, 0, 0, f / (f - n), 1, 0, 0, n * f / (n - f), 0 }; // projection matrix

	constants.LightVector = { 1.0f, -1.0f,  1.0f };
	constants.Rotate = { 0.0f,  0.0f,  0.0f };
	constants.Scale = { 1.0f,  1.0f,  1.0f };
	constants.Translate = { 0.0f,  0.0f,  4.0f };

	while (!window.IsShouldClose())
	{
		window.PollEvent();
		rhi.Resize(window.GetWidth(), window.GetHeight());

		// update
		{
			constants.Rotate.x += 0.005f;
			constants.Rotate.y += 0.009f;
			constants.Rotate.z += 0.001f;

			D3D11_MAPPED_SUBRESOURCE mappedSubresource;

			deviceContext->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource); // update constant buffer every frame
			
			*reinterpret_cast<Constants*>(mappedSubresource.pData) = constants;

			deviceContext->Unmap(constantBuffer, 0);
		}

		rhi.BeginFrame();

		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		deviceContext->IASetInputLayout(inputLayout);
		deviceContext->IASetVertexBuffers(0, 3, buffers, strides, offsets);
		deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		deviceContext->VSSetShader(vertexShader, nullptr, 0);
		deviceContext->VSSetConstantBuffers(0, 1, &constantBuffer);

		deviceContext->RSSetState(rasterizerState);

		deviceContext->PSSetShader(pixelShader, nullptr, 0);
		deviceContext->PSSetShaderResources(0, 1, &textureView);
		deviceContext->PSSetSamplers(0, 1, &samplerState);

		deviceContext->OMSetDepthStencilState(depthStencilState, 0);
		deviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);

		deviceContext->DrawIndexedInstanced(ARRAYSIZE(IndexData), 24, 0, 0, 0);


		rhi.EndFrame();
	}

	rhi.Destroy();
	window.Destroy();
	log.Destroy();
}
//=============================================================================S