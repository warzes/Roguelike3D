﻿#pragma once
//=============================================================================
struct ShaderProgram final
{
	Microsoft::WRL::ComPtr<ID3D11VertexShader>   vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>    inputLayout;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>    pixelShader;

	Microsoft::WRL::ComPtr<ID3D11HullShader>     hullShader;
	Microsoft::WRL::ComPtr<ID3D11DomainShader>   domainShader;

	Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometryShader;

	Microsoft::WRL::ComPtr<ID3D11ComputeShader>  computeShader;
};

struct PipelineState final
{
	Microsoft::WRL::ComPtr<ID3D11RasterizerState2>  rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11BlendState1>       blendState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	uint32_t                                        stencilRef;
};

struct SamplerState final
{
	Microsoft::WRL::ComPtr<ID3D11SamplerState> state;
};

struct Buffer final
{
	Microsoft::WRL::ComPtr<ID3D11Buffer>              buffer;
	UINT                                              bindFlags;
	UINT                                              dataBufferStride;
};

struct Texture1D final
{
	Microsoft::WRL::ComPtr<ID3D11Texture1D>            texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   view;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView1> uav;
};

struct Texture2D final
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D1>           texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   view;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView1> uav;
};

struct Texture3D final
{
	Microsoft::WRL::ComPtr<ID3D11Texture3D1>           texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   view;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView1> uav;
};
//=============================================================================
std::expected<Microsoft::WRL::ComPtr<ID3DBlob>, std::string> CompileShaderFromFile(const ShaderLoadInfo& loadInfo, const std::string& target)
{
	if (loadInfo.entryPoint.empty()) return std::unexpected("Entry point in ShaderLoadInfo is empty.");

	D3D_SHADER_MACRO* d3dmacros = nullptr;
	if (!loadInfo.macros.empty())
	{
		d3dmacros = new D3D_SHADER_MACRO[loadInfo.macros.size()];
		for (size_t i = 0; i < loadInfo.macros.size(); ++i)
		{
			d3dmacros[i].Name       = loadInfo.macros[i].name;
			d3dmacros[i].Definition = loadInfo.macros[i].value;
		}
	}

	UINT d3dFlags = 0;
	if (loadInfo.flags & static_cast<UINT>(ShaderCompileFlags::Debug))     d3dFlags |= D3DCOMPILE_DEBUG;
	if (loadInfo.flags & static_cast<UINT>(ShaderCompileFlags::Strict))    d3dFlags |= D3DCOMPILE_ENABLE_STRICTNESS;
	if (loadInfo.flags & static_cast<UINT>(ShaderCompileFlags::IEEStrict)) d3dFlags |= D3DCOMPILE_IEEE_STRICTNESS;
	if (loadInfo.flags & static_cast<UINT>(ShaderCompileFlags::Optimize0)) d3dFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL0;
	if (loadInfo.flags & static_cast<UINT>(ShaderCompileFlags::Optimize1)) d3dFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL1;
	if (loadInfo.flags & static_cast<UINT>(ShaderCompileFlags::Optimize2)) d3dFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL2;
	if (loadInfo.flags & static_cast<UINT>(ShaderCompileFlags::Optimize3)) d3dFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;

	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
	HRESULT result = D3DCompileFromFile(loadInfo.file.c_str(), d3dmacros, 0, loadInfo.entryPoint.c_str(), target.c_str(), d3dFlags, 0, &blob, &errorBlob);
	delete[] d3dmacros;
	if (FAILED(result))
	{
		std::string errorText = DX_ERR_STR("D3DCompileFromFile() failed: ", result);
		if (errorBlob != nullptr)
		{
			std::string error;
			error.resize(errorBlob->GetBufferSize());
			std::memcpy(error.data(), errorBlob->GetBufferPointer(), errorBlob->GetBufferSize());
			errorText += "\n";
			errorText += error;
		}

		return std::unexpected(errorText);
	}
	return blob;
}
//=============================================================================
std::expected<ShaderProgramPtr, std::string> CuteEngineApp::LoadShaderProgram(const ShaderProgramLoadInfo& loadInfo)
{
	ShaderProgramPtr program = std::make_shared<ShaderProgram>();

	// Create Vertex Shader
	{
		if (!loadInfo.vertexShader.file.empty())
		{
			Print(L"Load Vertex Shader in File: " + loadInfo.vertexShader.file);
			auto blobRet = CompileShaderFromFile(loadInfo.vertexShader, "vs_5_0");
			if (!blobRet.has_value()) return std::unexpected(blobRet.error());
			auto blob = blobRet.value();
			HRESULT result = rhiData::d3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &program->vertexShader);
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
		if (!loadInfo.pixelShader.file.empty())
		{
			Print(L"Load Pixel Shader in File: " + loadInfo.pixelShader.file);
			auto blobRet = CompileShaderFromFile(loadInfo.pixelShader, "ps_5_0");
			if (!blobRet.has_value()) return std::unexpected(blobRet.error());
			auto blob = blobRet.value();
			HRESULT result = rhiData::d3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &program->pixelShader);
			if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreatePixelShader() failed: ", result));
		}
	}

	// Create Hull Shader
	{
		if (!loadInfo.hullShader.file.empty())
		{
			Print(L"Load Hull Shader in File: " + loadInfo.hullShader.file);
			auto blobRet = CompileShaderFromFile(loadInfo.hullShader, "hs_5_0");
			if (!blobRet.has_value()) return std::unexpected(blobRet.error());
			auto blob = blobRet.value();
			HRESULT result = rhiData::d3dDevice->CreateHullShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &program->hullShader);
			if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateHullShader() failed: ", result));
		}
	}

	// Create Domain Shader
	{
		if (!loadInfo.domainShader.file.empty())
		{
			Print(L"Load Domain Shader in File: " + loadInfo.domainShader.file);
			auto blobRet = CompileShaderFromFile(loadInfo.domainShader, "ds_5_0");
			if (!blobRet.has_value()) return std::unexpected(blobRet.error());
			auto blob = blobRet.value();
			HRESULT result = rhiData::d3dDevice->CreateDomainShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &program->domainShader);
			if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateDomainShader() failed: ", result));
		}
	}

	// Create Geometry Shader
	{
		if (!loadInfo.geometryShader.file.empty())
		{
			Print(L"Load Geometry Shader in File: " + loadInfo.geometryShader.file);
			auto blobRet = CompileShaderFromFile(loadInfo.geometryShader, "gs_5_0");
			if (!blobRet.has_value()) return std::unexpected(blobRet.error());
			auto blob = blobRet.value();
			HRESULT result = rhiData::d3dDevice->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &program->geometryShader);
			if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateGeometryShader() failed: ", result));
		}
	}

	// Create Compute Shader
	{
		if (!loadInfo.computeShader.file.empty())
		{
			Print(L"Load Compute Shader in File: " + loadInfo.computeShader.file);
			auto blobRet = CompileShaderFromFile(loadInfo.geometryShader, "cs_5_0");
			if (!blobRet.has_value()) return std::unexpected(blobRet.error());
			auto blob = blobRet.value();
			HRESULT result = rhiData::d3dDevice->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &program->computeShader);
			if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateComputeShader() failed: ", result));
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
		const auto& state = createInfo.blendState;

		D3D11_BLEND_DESC1 blendDesc = { 0 };
		blendDesc.AlphaToCoverageEnable = state.alphaToCoverageEnabled;
		blendDesc.IndependentBlendEnable = state.separateBlendEnabled;

		//if (state.blendDesc.blendEnabled) {
		blendDesc.RenderTarget[0].BlendEnable           = state.blendDesc.blendEnabled;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = static_cast<UINT8>(state.renderTargetBlendDesc[0].writeMask);
		blendDesc.RenderTarget[0].SrcBlend              = ConvertToD3D11(state.blendDesc.srcBlend);
		blendDesc.RenderTarget[0].DestBlend             = ConvertToD3D11(state.blendDesc.dstBlend);
		blendDesc.RenderTarget[0].BlendOp               = ConvertToD3D11(state.blendDesc.blendOp);
		blendDesc.RenderTarget[0].SrcBlendAlpha         = ConvertToD3D11(state.blendDesc.srcBlendAlpha);
		blendDesc.RenderTarget[0].DestBlendAlpha        = ConvertToD3D11(state.blendDesc.dstBlendAlpha);
		blendDesc.RenderTarget[0].BlendOpAlpha          = ConvertToD3D11(state.blendDesc.blendOpAlpha);
		//}
		// TODO: D3D11_RENDER_TARGET_BLEND_DESC1::LogicOp and D3D11_RENDER_TARGET_BLEND_DESC1::LogicOpEnable

		if (state.separateBlendEnabled)
		{
			for (size_t i = 0; i < 8; ++i)
			{
				blendDesc.RenderTarget[i].BlendEnable           = state.renderTargetBlendDesc[i].blendEnabled;
				blendDesc.RenderTarget[i].RenderTargetWriteMask = static_cast<UINT8>(state.renderTargetBlendDesc[i].writeMask);
				blendDesc.RenderTarget[i].SrcBlend              = ConvertToD3D11(state.renderTargetBlendDesc[i].srcBlend);
				blendDesc.RenderTarget[i].DestBlend             = ConvertToD3D11(state.renderTargetBlendDesc[i].dstBlend);
				blendDesc.RenderTarget[i].BlendOp               = ConvertToD3D11(state.renderTargetBlendDesc[i].blendOp);
				blendDesc.RenderTarget[i].SrcBlendAlpha         = ConvertToD3D11(state.renderTargetBlendDesc[i].srcBlendAlpha);
				blendDesc.RenderTarget[i].DestBlendAlpha        = ConvertToD3D11(state.renderTargetBlendDesc[i].dstBlendAlpha);
				blendDesc.RenderTarget[i].BlendOpAlpha          = ConvertToD3D11(state.renderTargetBlendDesc[i].blendOpAlpha);
				// TODO: D3D11_RENDER_TARGET_BLEND_DESC1::LogicOp and D3D11_RENDER_TARGET_BLEND_DESC1::LogicOpEnable
			}
		}

		HRESULT result = rhiData::d3dDevice->CreateBlendState1(&blendDesc, &pipelineState->blendState);
		if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateBlendState1() failed: ", result));
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
std::expected<BufferPtr, std::string> CreateBuffer(const BufferCreateInfo& createInfo, UINT bindFlags)
{
	BufferPtr buffer = std::make_shared<Buffer>();
	buffer->bindFlags = bindFlags;
	buffer->dataBufferStride = static_cast<UINT>(createInfo.stride);

	D3D11_BUFFER_DESC bufferDesc   = { 0 };
	bufferDesc.ByteWidth           = static_cast<UINT>(createInfo.size);
	bufferDesc.Usage               = ConvertToD3D11(createInfo.usage);
	bufferDesc.BindFlags           = buffer->bindFlags;
	bufferDesc.CPUAccessFlags      = ConvertToD3D11(createInfo.cpuAccessFlags);
	bufferDesc.MiscFlags           = 0;
	bufferDesc.StructureByteStride = buffer->dataBufferStride;

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = createInfo.memoryData;

	HRESULT result = rhiData::d3dDevice->CreateBuffer(&bufferDesc, (createInfo.memoryData == nullptr) ? nullptr : &data, &buffer->buffer);
	if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateBuffer() failed: ", result));

	return buffer;
}
//=============================================================================
std::expected<BufferPtr, std::string> CuteEngineApp::CreateConstantBuffer(const BufferCreateInfo& createInfo)
{
	return CreateBuffer(createInfo, D3D11_BIND_CONSTANT_BUFFER);
}
//=============================================================================
std::expected<BufferPtr, std::string> CuteEngineApp::CreateVertexBuffer(const BufferCreateInfo& createInfo)
{
	return CreateBuffer(createInfo, D3D11_BIND_VERTEX_BUFFER);
}
//=============================================================================
std::expected<BufferPtr, std::string> CuteEngineApp::CreateIndexBuffer(const BufferCreateInfo& createInfo)
{
	return CreateBuffer(createInfo, D3D11_BIND_INDEX_BUFFER);
}
//=============================================================================
std::expected<Texture1DPtr, std::string> CuteEngineApp::CreateTexture1D(const Texture1DCreateInfo& createInfo)
{
	Texture1DPtr texture = std::make_shared<Texture1D>();

	UINT        bindFlags = D3D11_BIND_SHADER_RESOURCE;
	D3D11_USAGE usageFlags = D3D11_USAGE_DEFAULT; // TODO: а еще есть D3D11_USAGE_IMMUTABLE, сделать
	UINT        cpuAccess = 0;
	bool        isStaging = false;
	bool        isUAV = false;

	if (createInfo.flags & TextureFlags::RenderTarget)
		bindFlags |= D3D11_BIND_RENDER_TARGET;
	if (createInfo.flags & TextureFlags::DepthStencil)
		bindFlags |= D3D11_BIND_DEPTH_STENCIL;

	if (createInfo.flags & TextureFlags::CPURead)
	{
		bindFlags = 0;
		usageFlags = D3D11_USAGE_STAGING;
		cpuAccess = D3D11_CPU_ACCESS_READ;
		isStaging = true;
	}

	if (createInfo.flags & TextureFlags::GPUWrite)
	{
		bindFlags |= D3D11_BIND_UNORDERED_ACCESS;
		isUAV = true;
	}

	D3D11_TEXTURE1D_DESC textureDesc = { 0 };
	textureDesc.Width          = createInfo.width;
	textureDesc.MipLevels      = createInfo.mipCount;
	textureDesc.ArraySize      = 1;
	textureDesc.Format         = ConvertToD3D11(createInfo.format);
	textureDesc.Usage          = usageFlags;
	textureDesc.BindFlags      = bindFlags;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags      = 0;

	// TODO: доделать загрузку данных при инициализации

	HRESULT result = rhiData::d3dDevice->CreateTexture1D(&textureDesc, nullptr, &texture->texture);
	if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateTexture1D() failed: ", result));

	if (!isStaging)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
		viewDesc.Format              = ConvertToD3D11(createInfo.format);
		viewDesc.ViewDimension       = D3D11_SRV_DIMENSION_TEXTURE1D;
		viewDesc.Texture1D.MipLevels = createInfo.mipCount;

		result = rhiData::d3dDevice->CreateShaderResourceView(texture->texture.Get(), nullptr, &texture->view);
		if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateShaderResourceView() failed: ", result));
	}
	
	if (isUAV)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC1 uavDesc = {};
		uavDesc.Format                            = ConvertToD3D11(createInfo.format);
		uavDesc.ViewDimension                     = D3D11_UAV_DIMENSION_TEXTURE1D;
		uavDesc.Texture1D.MipSlice                = 0;

		result = rhiData::d3dDevice->CreateUnorderedAccessView1(texture->texture.Get(), &uavDesc, &texture->uav);
		if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateUnorderedAccessView1() failed: ", result));
	}

	return texture;
}
//=============================================================================
std::expected<Texture2DPtr, std::string> CuteEngineApp::CreateTexture2D(const Texture2DCreateInfo& createInfo)
{
	Texture2DPtr texture = std::make_shared<Texture2D>();

	UINT        bindFlags = D3D11_BIND_SHADER_RESOURCE;
	D3D11_USAGE usageFlags = D3D11_USAGE_DEFAULT; // TODO: а еще есть D3D11_USAGE_IMMUTABLE, сделать
	UINT        cpuAccess = 0;
	bool        isStaging = false;
	bool        isUAV = false;

	if (createInfo.flags & TextureFlags::RenderTarget)
		bindFlags |= D3D11_BIND_RENDER_TARGET;
	if (createInfo.flags & TextureFlags::DepthStencil)
		bindFlags |= D3D11_BIND_DEPTH_STENCIL;

	if (createInfo.flags & TextureFlags::CPURead)
	{
		bindFlags = 0;
		usageFlags = D3D11_USAGE_STAGING;
		cpuAccess = D3D11_CPU_ACCESS_READ;
		isStaging = true;
	}

	if (createInfo.flags & TextureFlags::GPUWrite)
	{
		bindFlags |= D3D11_BIND_UNORDERED_ACCESS;
		isUAV = true;
	}

	DXGI_FORMAT dataFormat = ConvertToD3D11(createInfo.format);

	DXGI_FORMAT textureFormat = dataFormat;
	DXGI_FORMAT viewFormat = dataFormat;

	if (createInfo.format == TexelsFormat::D16)
	{
		textureFormat = DXGI_FORMAT_R16_TYPELESS;
		viewFormat = DXGI_FORMAT_R16_UNORM;
	}

	if (createInfo.format == TexelsFormat::D24S8)
	{
		// TODO: X8 part of the texture is not accessible ATM, try to handle this properly
		textureFormat = DXGI_FORMAT_R24G8_TYPELESS;
		viewFormat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	}

	if (createInfo.format == TexelsFormat::D32F)
	{
		textureFormat = DXGI_FORMAT_R32_TYPELESS;
		viewFormat = DXGI_FORMAT_R32_FLOAT;
	}

	D3D11_TEXTURE2D_DESC1 textureDesc = { 0 };
	textureDesc.Width          = createInfo.width;
	textureDesc.Height         = createInfo.height;
	textureDesc.MipLevels      = createInfo.mipCount;
	textureDesc.ArraySize      = 1;
	textureDesc.Format         = textureFormat;
	textureDesc.SampleDesc     = { 1, 0 };
	textureDesc.Usage          = usageFlags;
	textureDesc.BindFlags      = bindFlags;
	textureDesc.CPUAccessFlags = cpuAccess;
	textureDesc.MiscFlags = 0;

	//D3D11_SUBRESOURCE_DATA textureData{};
	//textureData.pSysMem = createInfo.memoryData;
	//textureData.SysMemPitch = 2 * sizeof(UINT); // texture is 2 pixels wide, 4 bytes per pixel // TODO: доделать
	// TODO: доделать загрузку данных при инициализации

	HRESULT result = rhiData::d3dDevice->CreateTexture2D1(&textureDesc, nullptr, &texture->texture);
	if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateTexture2D1() failed: ", result));

	if (!isStaging)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
		viewDesc.Format              = viewFormat;
		viewDesc.ViewDimension       = D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MipLevels = createInfo.mipCount;

		result = rhiData::d3dDevice->CreateShaderResourceView(texture->texture.Get(), nullptr, &texture->view);
		if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateShaderResourceView() failed: ", result));
	}

	if (isUAV)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC1 uavDesc = {};
		uavDesc.Format             = ConvertToD3D11(createInfo.format);
		uavDesc.ViewDimension      = D3D11_UAV_DIMENSION_TEXTURE2D;

		result = rhiData::d3dDevice->CreateUnorderedAccessView1(texture->texture.Get(), &uavDesc, &texture->uav);
		if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateUnorderedAccessView1() failed: ", result));
	}

	return texture;
}
//=============================================================================
std::expected<Texture3DPtr, std::string> CuteEngineApp::CreateTexture3D(const Texture3DCreateInfo& createInfo)
{
	Texture3DPtr texture = std::make_shared<Texture3D>();

	UINT        bindFlags = D3D11_BIND_SHADER_RESOURCE;
	D3D11_USAGE usageFlags = D3D11_USAGE_DEFAULT;
	UINT        cpuAccess = 0;
	bool        isStaging = false;
	bool        isUAV = false;

	if (createInfo.flags & TextureFlags::RenderTarget)
		bindFlags |= D3D11_BIND_RENDER_TARGET;
	if (createInfo.flags & TextureFlags::DepthStencil)
		bindFlags |= D3D11_BIND_DEPTH_STENCIL;

	if (createInfo.flags & TextureFlags::CPURead)
	{
		bindFlags = 0;
		usageFlags = D3D11_USAGE_STAGING;
		cpuAccess = D3D11_CPU_ACCESS_READ;
		isStaging = true;
	}

	if (createInfo.flags & TextureFlags::GPUWrite)
	{
		bindFlags |= D3D11_BIND_UNORDERED_ACCESS;
		isUAV = true;
	}

	D3D11_TEXTURE3D_DESC1 textureDesc = { 0 };
	textureDesc.Width          = createInfo.width;
	textureDesc.Height         = createInfo.height;
	textureDesc.Depth          = createInfo.depth;
	textureDesc.MipLevels      = createInfo.mipCount;
	textureDesc.Format         = ConvertToD3D11(createInfo.format);
	textureDesc.Usage          = usageFlags;
	textureDesc.BindFlags      = bindFlags;
	textureDesc.CPUAccessFlags = cpuAccess;
	textureDesc.MiscFlags      = 0;

	// TODO: доделать загрузку данных при инициализации

	HRESULT result = rhiData::d3dDevice->CreateTexture3D1(&textureDesc, nullptr, &texture->texture);
	if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateTexture3D1() failed: ", result));

	if (!isStaging)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
		viewDesc.Format              = ConvertToD3D11(createInfo.format);
		viewDesc.ViewDimension       = D3D11_SRV_DIMENSION_TEXTURE3D;
		viewDesc.Texture3D.MipLevels = createInfo.mipCount;

		result = rhiData::d3dDevice->CreateShaderResourceView(texture->texture.Get(), nullptr, &texture->view);
		if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateShaderResourceView() failed: ", result));
	}

	if (isUAV)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC1 uavDesc = {};
		uavDesc.Format        = ConvertToD3D11(createInfo.format);
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;

		result = rhiData::d3dDevice->CreateUnorderedAccessView1(texture->texture.Get(), &uavDesc, &texture->uav);
		if (FAILED(result)) return std::unexpected(DX_ERR_STR("ID3D11Device5::CreateUnorderedAccessView1() failed: ", result));
	}

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
void CuteEngineApp::DeleteRHIResource(BufferPtr& resource)
{
	resource.reset();
}
//=============================================================================
void CuteEngineApp::DeleteRHIResource(Texture1DPtr& resource)
{
	resource.reset();
}
//=============================================================================
void CuteEngineApp::DeleteRHIResource(Texture2DPtr& resource)
{
	resource.reset();
}
//=============================================================================
void CuteEngineApp::DeleteRHIResource(Texture3DPtr& resource)
{
	resource.reset();
}
//=============================================================================
void* CuteEngineApp::Map(BufferPtr buffer, MapType type)
{
	D3D11_MAPPED_SUBRESOURCE mappedSubresource = { 0 };
	HRESULT result = rhiData::d3dContext->Map(buffer->buffer.Get(), 0, ConvertToD3D11(type), 0, &mappedSubresource);
	if (FAILED(result))
	{
		DX_ERR("ID3D11Device5::Map() failed: ", result);
		return nullptr;
	}
	return mappedSubresource.pData;
}
//=============================================================================
void CuteEngineApp::Unmap(BufferPtr buffer)
{
	rhiData::d3dContext->Unmap(buffer->buffer.Get(), 0);
}
//=============================================================================
void CuteEngineApp::UpdateBuffer(BufferPtr buffer, const void* mem)
{
	rhiData::d3dContext->UpdateSubresource(buffer->buffer.Get(), 0, nullptr, mem, 0, 0);
}
//=============================================================================
void CuteEngineApp::CopyBufferData(BufferPtr buffer, size_t offset, size_t size, const void* mem)
{
	const D3D11_BOX box = {
		.left   = static_cast<UINT>(offset),
		.top    = 0, .front = 0,
		.right  = static_cast<UINT>(offset + size),
		.bottom = 1, .back = 1,
	};
	rhiData::d3dContext->UpdateSubresource(buffer->buffer.Get(), 0, &box, mem, 0, 0);
}
//=============================================================================
inline void ClearTextureRW(Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView1> uav, uint32_t value)
{
	uint32_t d3dValues[4] = { value, 0, 0, 0 };
	if (uav) rhiData::d3dContext->ClearUnorderedAccessViewUint(uav.Get(), d3dValues);
}
//=============================================================================
inline void ClearTextureRW(Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView1> uav, float value)
{
	float d3dValues[4] = { value, 0.F, 0.0F, 0.0F };
	if (uav) rhiData::d3dContext->ClearUnorderedAccessViewFloat(uav.Get(), d3dValues);
}
//=============================================================================
void CuteEngineApp::ClearTextureRW(Texture1DPtr texture, uint32_t value)
{
	::ClearTextureRW(texture->uav, value);
}
//=============================================================================
void CuteEngineApp::ClearTextureRW(Texture1DPtr texture, float value)
{
	::ClearTextureRW(texture->uav, value);
}
//=============================================================================
void CuteEngineApp::ClearTextureRW(Texture2DPtr texture, uint32_t value)
{
	::ClearTextureRW(texture->uav, value);
}
//=============================================================================
void CuteEngineApp::ClearTextureRW(Texture2DPtr texture, float value)
{
	::ClearTextureRW(texture->uav, value);
}
//=============================================================================
void CuteEngineApp::ClearTextureRW(Texture3DPtr texture, uint32_t value)
{
	::ClearTextureRW(texture->uav, value);
}
//=============================================================================
void CuteEngineApp::ClearTextureRW(Texture3DPtr texture, float value)
{
	::ClearTextureRW(texture->uav, value);
}
//=============================================================================
inline void* Map(Microsoft::WRL::ComPtr<ID3D11Resource> texture, MapType type)
{
	D3D11_MAPPED_SUBRESOURCE mappedSubresource = { 0 };
	HRESULT result = rhiData::d3dContext->Map(texture.Get(), 0, ConvertToD3D11(type), 0, &mappedSubresource);
	if (FAILED(result))
	{
		DX_ERR("ID3D11Device5::Map() failed: ", result);
		return nullptr;
	}
	return mappedSubresource.pData;
}
//=============================================================================
void* CuteEngineApp::Map(Texture1DPtr handle, MapType type)
{
	return ::Map(handle->texture, type);
}
//=============================================================================
void* CuteEngineApp::Map(Texture2DPtr handle, MapType type)
{
	return ::Map(handle->texture, type);
}
//=============================================================================
void* CuteEngineApp::Map(Texture3DPtr handle, MapType type)
{
	return ::Map(handle->texture, type);
}
//=============================================================================
void CuteEngineApp::Unmap(Texture1DPtr handle)
{
	rhiData::d3dContext->Unmap(handle->texture.Get(), 0);
}
//=============================================================================
void CuteEngineApp::Unmap(Texture2DPtr handle)
{
	rhiData::d3dContext->Unmap(handle->texture.Get(), 0);
}
//=============================================================================
void CuteEngineApp::Unmap(Texture3DPtr handle)
{
	rhiData::d3dContext->Unmap(handle->texture.Get(), 0);
}
//=============================================================================
void CuteEngineApp::UpdateTexture(Texture1DPtr handle, const void* mem, uint32_t mip, size_t offsetX, size_t sizeX, size_t offsetY, size_t sizeY, size_t offsetZ, size_t sizeZ, size_t rowPitch, size_t depthPitch)
{
	D3D11_BOX box;
	box.left = static_cast<UINT>(offsetX);
	box.right = static_cast<UINT>(offsetX + sizeX);
	box.top = static_cast<UINT>(offsetY);
	box.bottom = static_cast<UINT>(offsetY + sizeY);
	box.front = static_cast<UINT>(offsetZ);
	box.back = static_cast<UINT>(offsetZ + sizeZ);

	rhiData::d3dContext->UpdateSubresource(handle->texture.Get(), mip, &box, mem, static_cast<UINT>(rowPitch), static_cast<UINT>(depthPitch));
}
//=============================================================================
void CuteEngineApp::UpdateTexture(Texture2DPtr handle, const void* mem, uint32_t mip, size_t offsetX, size_t sizeX, size_t offsetY, size_t sizeY, size_t offsetZ, size_t sizeZ, size_t rowPitch, size_t depthPitch)
{
	D3D11_BOX box;
	box.left = static_cast<UINT>(offsetX);
	box.right = static_cast<UINT>(offsetX + sizeX);
	box.top = static_cast<UINT>(offsetY);
	box.bottom = static_cast<UINT>(offsetY + sizeY);
	box.front = static_cast<UINT>(offsetZ);
	box.back = static_cast<UINT>(offsetZ + sizeZ);

	rhiData::d3dContext->UpdateSubresource(handle->texture.Get(), mip, &box, mem, static_cast<UINT>(rowPitch), static_cast<UINT>(depthPitch));
}
//=============================================================================
void CuteEngineApp::UpdateTexture(Texture3DPtr handle, const void* mem, uint32_t mip, size_t offsetX, size_t sizeX, size_t offsetY, size_t sizeY, size_t offsetZ, size_t sizeZ, size_t rowPitch, size_t depthPitch)
{
	D3D11_BOX box;
	box.left = static_cast<UINT>(offsetX);
	box.right = static_cast<UINT>(offsetX + sizeX);
	box.top = static_cast<UINT>(offsetY);
	box.bottom = static_cast<UINT>(offsetY + sizeY);
	box.front = static_cast<UINT>(offsetZ);
	box.back = static_cast<UINT>(offsetZ + sizeZ);

	rhiData::d3dContext->UpdateSubresource(handle->texture.Get(), mip, &box, mem, static_cast<UINT>(rowPitch), static_cast<UINT>(depthPitch));
}
//=============================================================================
void CuteEngineApp::BindShaderProgram(ShaderProgramPtr resource)
{
	if (!resource) [[unlikely]] return;

	rhiData::d3dContext->VSSetShader(resource->vertexShader.Get(), 0, 0);
	rhiData::d3dContext->PSSetShader(resource->pixelShader.Get(), 0, 0);

	rhiData::d3dContext->IASetInputLayout(resource->inputLayout.Get());

	rhiData::d3dContext->HSSetShader(resource->hullShader.Get(), 0, 0);
	rhiData::d3dContext->DSSetShader(resource->domainShader.Get(), 0, 0);

	rhiData::d3dContext->GSSetShader(resource->geometryShader.Get(), 0, 0);
}
//=============================================================================
void CuteEngineApp::BindPipelineState(PipelineStatePtr resource)
{
	rhiData::d3dContext->RSSetState(resource->rasterizerState.Get());
	rhiData::d3dContext->OMSetBlendState(resource->blendState.Get(), nullptr, 0xffffffff);
	rhiData::d3dContext->OMSetDepthStencilState(resource->depthStencilState.Get(), resource->stencilRef);

}
//=============================================================================
void CuteEngineApp::BindSamplerState(SamplerStatePtr resource, uint32_t slot)
{
	rhiData::d3dContext->PSSetSamplers(slot, 1, resource->state.GetAddressOf());
}
//=============================================================================
void CuteEngineApp::BindConstantBuffer(BufferPtr resource, uint32_t slot)
{
	rhiData::d3dContext->VSSetConstantBuffers(slot, 1, resource->buffer.GetAddressOf());
}
//=============================================================================
void CuteEngineApp::BindVertexBuffer(BufferPtr resource)
{
	// TODO:
}
//=============================================================================
void CuteEngineApp::BindVertexBuffers(const std::vector<BufferPtr>& resources, const std::vector<uint32_t>& strides, const std::vector<uint32_t>& offsets)
{
	std::vector<ID3D11Buffer*> buffers(resources.size());
	for (size_t i = 0; i < resources.size(); i++)
		buffers[i] = resources[i]->buffer.Get();

	rhiData::d3dContext->IASetVertexBuffers(0, resources.size(), buffers.data(), strides.data(), offsets.data());
}
//=============================================================================
void CuteEngineApp::BindIndexBuffer(BufferPtr resource)
{
	rhiData::d3dContext->IASetIndexBuffer(resource->buffer.Get(), DXGI_FORMAT_R32_UINT, 0); // TODO: DXGI_FORMAT_R32_UINT должно передаваться
}
//=============================================================================
void CuteEngineApp::BindTexture2D(Texture2DPtr resource, uint32_t slot)
{
	rhiData::d3dContext->PSSetShaderResources(slot, 1, resource->view.GetAddressOf());
}
//=============================================================================