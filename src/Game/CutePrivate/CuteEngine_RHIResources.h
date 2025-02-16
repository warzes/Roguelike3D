#pragma once
//=============================================================================
struct ShaderProgram final
{
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>  inputLayout;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>  pixelShader;
};

struct PipelineState final
{
	Microsoft::WRL::ComPtr<ID3D11RasterizerState2>  rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	uint32_t                                        stencilRef;
};

struct SamplerState final
{
	Microsoft::WRL::ComPtr<ID3D11SamplerState> state;
};

struct ConstantBuffer final
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
};
struct VertexBuffer final
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
};
struct IndexBuffer final
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
};

struct Texture2D final
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D>          texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> view;
};
//=============================================================================
std::expected<ShaderProgramPtr, std::string> CuteEngineApp::LoadShaderProgram(const ShaderProgramLoadInfo& loadInfo)
{
	ShaderProgramPtr program = std::make_shared<ShaderProgram>();

	// Create Vertex Shader
	{
		const auto& vsli = loadInfo.vertexShader;
		if (!vsli.file.empty())
		{
			Print(L"Load Vertex Shader in File: " + vsli.file);
			if (vsli.entryPoint.empty()) return std::unexpected("Entry point in ShaderProgramLoadInfo::vertexShader is empty.");
			if (vsli.target.empty()) return std::unexpected("Target in ShaderProgramLoadInfo::vertexShader is empty.");

			Microsoft::WRL::ComPtr<ID3DBlob> blob;
			HRESULT result = D3DCompileFromFile(vsli.file.c_str(), 0, 0, vsli.entryPoint.c_str(), vsli.target.c_str(), 0, 0, &blob, 0);
			if (FAILED(result)) return std::unexpected(DX_ERR_STR("D3DCompileFromFile() failed: ", result));

			result = rhiData::d3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &program->vertexShader);
			if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateVertexShader() failed: ", result));

			// Create Input Layout
			{
				if (loadInfo.inputLayout.empty())
				{
					// TODO: сделать получение из шейдера если не введены данные
				}
				else
				{
					std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc(loadInfo.inputLayout.size());

					for (size_t i = 0; i < loadInfo.inputLayout.size(); i++)
					{
						const auto& elem = loadInfo.inputLayout[i];

						inputElementDesc[i].SemanticName = elem.semanticName.c_str();
						inputElementDesc[i].SemanticIndex = elem.semanticIndex;
						inputElementDesc[i].Format = ConvertToD3D11(elem.format);
						inputElementDesc[i].InputSlot = elem.slot;
						inputElementDesc[i].AlignedByteOffset = static_cast<UINT>(elem.offset);
						inputElementDesc[i].InputSlotClass = elem.perInstanceData ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;
						inputElementDesc[i].InstanceDataStepRate = elem.perInstanceData ? elem.instanceDataStepRate : 0;
					}
					HRESULT result = rhiData::d3dDevice->CreateInputLayout(inputElementDesc.data(), inputElementDesc.size(), blob->GetBufferPointer(), blob->GetBufferSize(), &program->inputLayout);
					if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateInputLayout() failed: ", result));
				}
			}
		}
	}

	// Create Pixel Shader
	{
		const auto& psli = loadInfo.pixelShader;
		if (!psli.file.empty())
		{
			Print(L"Load Pixel Shader in File: " + psli.file);
			if (psli.entryPoint.empty()) return std::unexpected("Entry point in ShaderProgramLoadInfo::pixelShader is empty.");
			if (psli.target.empty()) return std::unexpected("Target in ShaderProgramLoadInfo::pixelShader is empty.");

			Microsoft::WRL::ComPtr<ID3DBlob> blob;
			HRESULT result = D3DCompileFromFile(psli.file.c_str(), 0, 0, psli.entryPoint.c_str(), psli.target.c_str(), 0, 0, &blob, 0);
			if (FAILED(result)) return std::unexpected(DX_ERR_STR("D3DCompileFromFile() failed: ", result));

			result = rhiData::d3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &program->pixelShader);
			if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreatePixelShader() failed: ", result));
		}
	}

	return program;
}
//=============================================================================
std::expected<PipelineStatePtr, std::string> CuteEngineApp::CreatePipelineState(const PipelineStateCreateInfo& createInfo)
{
	PipelineStatePtr pipelineState = std::make_shared<PipelineState>();

	// create rasterizer state
	{
		const auto& state = createInfo.rasterizerState;

		D3D11_RASTERIZER_DESC2 rasterizerDesc{};
		rasterizerDesc.FillMode              = ConvertToD3D11(state.fillMode);
		rasterizerDesc.CullMode              = ConvertToD3D11(state.cullMode);
		rasterizerDesc.FrontCounterClockwise = state.counterDirection == CounterDirection::CW ? TRUE : FALSE;
		rasterizerDesc.DepthBias             = 0;
		rasterizerDesc.DepthBiasClamp        = 0.0f;
		rasterizerDesc.SlopeScaledDepthBias  = 0.0f;
		rasterizerDesc.DepthClipEnable       = TRUE;
		rasterizerDesc.ScissorEnable         = FALSE;
		rasterizerDesc.MultisampleEnable     = FALSE;
		rasterizerDesc.AntialiasedLineEnable = FALSE;
		rasterizerDesc.ForcedSampleCount     = 0;
		rasterizerDesc.ConservativeRaster    = D3D11_CONSERVATIVE_RASTERIZATION_MODE_OFF;

		HRESULT result = rhiData::d3dDevice->CreateRasterizerState2(&rasterizerDesc, &pipelineState->rasterizerState);
		if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateRasterizerState2() failed: ", result));
	}

	// create blend state
	{

	}

	// create depth stencil state
	{
		const auto& state = createInfo.depthStencilState;

		D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
		depthStencilDesc.DepthEnable                  = state.depthEnabled;
		depthStencilDesc.DepthWriteMask               = ConvertToD3D11(state.writeMask);
		depthStencilDesc.DepthFunc                    = ConvertToD3D11(state.depthFunc);
		depthStencilDesc.StencilEnable                = state.stencilEnabled;
		depthStencilDesc.StencilReadMask              = state.stencilReadMask;
		depthStencilDesc.StencilWriteMask             = state.stencilWriteMask;
		depthStencilDesc.FrontFace.StencilFunc        = ConvertToD3D11(state.frontFaceStencilDesc.stencilFunc);
		depthStencilDesc.FrontFace.StencilFailOp      = ConvertToD3D11(state.frontFaceStencilDesc.failOp);
		depthStencilDesc.FrontFace.StencilDepthFailOp = ConvertToD3D11(state.frontFaceStencilDesc.depthFailOp);
		depthStencilDesc.FrontFace.StencilPassOp      = ConvertToD3D11(state.frontFaceStencilDesc.passOp);
		depthStencilDesc.BackFace.StencilFunc         = ConvertToD3D11(state.backFaceStencilDesc.stencilFunc);
		depthStencilDesc.BackFace.StencilFailOp       = ConvertToD3D11(state.backFaceStencilDesc.failOp);
		depthStencilDesc.BackFace.StencilDepthFailOp  = ConvertToD3D11(state.backFaceStencilDesc.depthFailOp);
		depthStencilDesc.BackFace.StencilPassOp       = ConvertToD3D11(state.backFaceStencilDesc.passOp);

		HRESULT result = rhiData::d3dDevice->CreateDepthStencilState(&depthStencilDesc, &pipelineState->depthStencilState);
		if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateDepthStencilState() failed: ", result));

		pipelineState->stencilRef = state.stencilRef;
	}

	return pipelineState;
}
//=============================================================================
std::expected<SamplerStatePtr, std::string> CuteEngineApp::CreateSamplerState(const SamplerStateCreateInfo& createInfo)
{
	SamplerStatePtr samplerState = std::make_shared<SamplerState>();

	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter         = ConvertToD3D11(createInfo.filter);
	samplerDesc.AddressU       = ConvertToD3D11(createInfo.addressU);
	samplerDesc.AddressV       = ConvertToD3D11(createInfo.addressV);
	samplerDesc.AddressW       = ConvertToD3D11(createInfo.addressW);
	samplerDesc.MipLODBias     = createInfo.lodBias;
	samplerDesc.MaxAnisotropy  = createInfo.maxAnisotropy;
	samplerDesc.ComparisonFunc = ConvertToD3D11(createInfo.comparisonFunc);
	samplerDesc.BorderColor[0] = static_cast<float>((createInfo.borderColor >> 0) & 0xFF) / 255.0F;
	samplerDesc.BorderColor[1] = static_cast<float>((createInfo.borderColor >> 8) & 0xFF) / 255.0F;
	samplerDesc.BorderColor[2] = static_cast<float>((createInfo.borderColor >> 16) & 0xFF) / 255.0F;
	samplerDesc.BorderColor[3] = static_cast<float>((createInfo.borderColor >> 24) & 0xFF) / 255.0F;
	samplerDesc.MinLOD         = createInfo.minLod;
	samplerDesc.MaxLOD         = createInfo.maxLod;

	HRESULT result = rhiData::d3dDevice->CreateSamplerState(&samplerDesc, &samplerState->state);
	if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateSamplerState() failed: ", result));

	return samplerState;
}
//=============================================================================
std::expected<ConstantBufferPtr, std::string> CuteEngineApp::CreateConstantBuffer(const ConstantBufferCreateInfo& createInfo)
{
	ConstantBufferPtr buffer = std::make_shared<ConstantBuffer>();

	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth           = static_cast<UINT>(createInfo.size);
	bufferDesc.Usage               = ConvertToD3D11(createInfo.usage);
	bufferDesc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags      = ConvertToD3D11(createInfo.cpuAccessFlags);
	bufferDesc.MiscFlags           = 0;
	bufferDesc.StructureByteStride = static_cast<UINT>(createInfo.size);

	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = createInfo.memoryData;

	HRESULT result = rhiData::d3dDevice->CreateBuffer(&bufferDesc, (createInfo.memoryData == nullptr) ? nullptr : &data, &buffer->buffer);
	if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateBuffer() failed: ", result));

	return buffer;
}
//=============================================================================
std::expected<VertexBufferPtr, std::string> CuteEngineApp::CreateVertexBuffer(const VertexBufferCreateInfo& createInfo)
{
	VertexBufferPtr buffer = std::make_shared<VertexBuffer>();

	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth           = static_cast<UINT>(createInfo.size);
	bufferDesc.Usage               = ConvertToD3D11(createInfo.usage);
	bufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags      = ConvertToD3D11(createInfo.cpuAccessFlags);
	bufferDesc.MiscFlags           = 0;
	bufferDesc.StructureByteStride = static_cast<UINT>(createInfo.size);

	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = createInfo.memoryData;

	HRESULT result = rhiData::d3dDevice->CreateBuffer(&bufferDesc, (createInfo.memoryData == nullptr) ? nullptr : &data, &buffer->buffer);
	if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateBuffer() failed: ", result));

	return buffer;
}
//=============================================================================
std::expected<IndexBufferPtr, std::string> CuteEngineApp::CreateIndexBuffer(const IndexBufferCreateInfo& createInfo)
{
	IndexBufferPtr buffer = std::make_shared<IndexBuffer>();

	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth           = static_cast<UINT>(createInfo.size);
	bufferDesc.Usage               = ConvertToD3D11(createInfo.usage);
	bufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags      = ConvertToD3D11(createInfo.cpuAccessFlags);
	bufferDesc.MiscFlags           = 0;
	bufferDesc.StructureByteStride = static_cast<UINT>(createInfo.size);

	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = createInfo.memoryData;

	HRESULT result = rhiData::d3dDevice->CreateBuffer(&bufferDesc, (createInfo.memoryData == nullptr) ? nullptr : &data, &buffer->buffer);
	if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateBuffer() failed: ", result));

	return buffer;
}
//=============================================================================
std::expected<Texture2DPtr, std::string> CuteEngineApp::CreateTexture2D(const Texture2DCreateInfo& createInfo)
{
	Texture2DPtr texture = std::make_shared<Texture2D>();

	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width      = createInfo.width;
	textureDesc.Height     = createInfo.height;
	textureDesc.MipLevels  = createInfo.mipCount;
	textureDesc.ArraySize  = 1;
	textureDesc.Format     = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	textureDesc.SampleDesc = { 1, 0 };
	textureDesc.Usage      = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags  = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA textureData{};
	textureData.pSysMem = createInfo.memoryData;
	textureData.SysMemPitch = 2 * sizeof(UINT); // texture is 2 pixels wide, 4 bytes per pixel // TODO: доделать

	HRESULT result = rhiData::d3dDevice->CreateTexture2D(&textureDesc, &textureData, &texture->texture);
	if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateTexture2D() failed: ", result));

	result = rhiData::d3dDevice->CreateShaderResourceView(texture->texture.Get(), nullptr, &texture->view);
	if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateShaderResourceView() failed: ", result));

	return texture;
}
//=============================================================================
void CuteEngineApp::DeleteRHIResource(ShaderProgramPtr& resource)
{
	resource.reset();
}
//=============================================================================
void CuteEngineApp::DeleteRHIResource(PipelineStatePtr& resource)
{
	resource.reset();
}
//=============================================================================
void CuteEngineApp::DeleteRHIResource(SamplerStatePtr& resource)
{
	resource.reset();
}
//=============================================================================
void CuteEngineApp::DeleteRHIResource(ConstantBufferPtr& resource)
{
	resource.reset();
}
//=============================================================================
void CuteEngineApp::DeleteRHIResource(VertexBufferPtr& resource)
{
	resource.reset();
}
//=============================================================================
void CuteEngineApp::DeleteRHIResource(IndexBufferPtr& resource)
{
	resource.reset();
}
//=============================================================================
void CuteEngineApp::DeleteRHIResource(Texture2DPtr& resource)
{
	resource.reset();
}
//=============================================================================
void* CuteEngineApp::Map(ConstantBufferPtr buffer, MapType type)
{
	D3D11_MAPPED_SUBRESOURCE mappedSubresource{};
	HRESULT result = rhiData::d3dContext->Map(buffer->buffer.Get(), 0, ConvertToD3D11(type), 0, &mappedSubresource);
	if (FAILED(result))
	{
		DX_ERR("ID3D11Device5::Map() failed: ", result);
		return nullptr;
	}
	return mappedSubresource.pData;
}
//=============================================================================
void CuteEngineApp::UnMap(ConstantBufferPtr buffer)
{
	rhiData::d3dContext->Unmap(buffer->buffer.Get(), 0);
}
//=============================================================================
void CuteEngineApp::BindShaderProgram(ShaderProgramPtr resource)
{
	if (!resource) [[unlikely]] return;

	if (resource->vertexShader) rhiData::d3dContext->VSSetShader(resource->vertexShader.Get(), 0, 0);
	if (resource->pixelShader)  rhiData::d3dContext->PSSetShader(resource->pixelShader.Get(), 0, 0);
	if (resource->inputLayout)  rhiData::d3dContext->IASetInputLayout(resource->inputLayout.Get());
}
//=============================================================================
void CuteEngineApp::BindPipelineState(PipelineStatePtr resource)
{
	rhiData::d3dContext->RSSetState(resource->rasterizerState.Get());
	rhiData::d3dContext->OMSetDepthStencilState(resource->depthStencilState.Get(), resource->stencilRef);
	rhiData::d3dContext->OMSetBlendState(nullptr, nullptr, 0xffffffff); // TODO: доделать бленд сейт
}
//=============================================================================
void CuteEngineApp::BindSamplerState(SamplerStatePtr resource, uint32_t slot)
{
	rhiData::d3dContext->PSSetSamplers(slot, 1, resource->state.GetAddressOf());
}
//=============================================================================
void CuteEngineApp::BindConstantBuffer(ConstantBufferPtr resource, uint32_t slot)
{
	rhiData::d3dContext->VSSetConstantBuffers(slot, 1, resource->buffer.GetAddressOf());
}
//=============================================================================
void CuteEngineApp::BindVertexBuffer(VertexBufferPtr resource)
{
	// TODO:
}
//=============================================================================
void CuteEngineApp::BindVertexBuffers(const std::vector<VertexBufferPtr>& resources, const std::vector<uint32_t>& strides, const std::vector<uint32_t>& offsets)
{
	std::vector<ID3D11Buffer*> buffers(resources.size());
	for (size_t i = 0; i < resources.size(); i++)
		buffers[i] = resources[i]->buffer.Get();

	rhiData::d3dContext->IASetVertexBuffers(0, resources.size(), buffers.data(), strides.data(), offsets.data());
}
//=============================================================================
void CuteEngineApp::BindIndexBuffer(IndexBufferPtr resource)
{
	rhiData::d3dContext->IASetIndexBuffer(resource->buffer.Get(), DXGI_FORMAT_R32_UINT, 0); // TODO: DXGI_FORMAT_R32_UINT должно передаваться
}
//=============================================================================
void CuteEngineApp::BindTexture2D(Texture2DPtr resource, uint32_t slot)
{
	rhiData::d3dContext->PSSetShaderResources(slot, 1, resource->view.GetAddressOf());
}
//=============================================================================