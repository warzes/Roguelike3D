#pragma once

#include "CoreD3D11.h"

struct TextureSamplerCreateInfo final
{
	TextureSamplerFilter filter;
	TextureSamplerWrap   wrapMode;
};

struct Sampler final
{
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
};

struct Texture2DCreateInfo final
{
	uint32_t      width{ 0 };
	uint32_t      height{ 0 };
	uint32_t      mipCount{ 1 };
	TexelsFormat  format{ TexelsFormat::RGBA_U8 };
	void*         data{ nullptr };
};

struct Texture2D final
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D>           texture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  shaderResourceView;
	//Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> unorderedAccessView;
	uint32_t                                          width{ 0 };
	uint32_t                                          height{ 0 };
	uint32_t                                          mipCount{ 0 };
	TexelsFormat                                      format{ TexelsFormat::RGBA_U8 };
};

class GPUBuffer final
{

};

class Pipeline final
{

};