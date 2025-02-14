#pragma once
//=============================================================================
struct ShaderProgram final
{
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>  inputLayout;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>  pixelShader;
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
void CuteEngineApp::DeleteRHIResource(ShaderProgramPtr& resource)
{
	resource.reset();
}
//=============================================================================
void CuteEngineApp::BindShaderProgram(ShaderProgramPtr resource)
{
	if (!resource) [[unlikely]] return;

	if (resource->vertexShader) rhiData::d3dContext->VSSetShader(resource->vertexShader.Get(), 0, 0);
	if (resource->pixelShader)  rhiData::d3dContext->PSSetShader(resource->pixelShader.Get(), 0, 0);
}
//=============================================================================