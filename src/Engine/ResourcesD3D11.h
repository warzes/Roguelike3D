#pragma once

#include "CoreD3D11.h"

class Texture2D final
{
public:
	auto GetD3DTexture2D() const { return m_texture2D; }
	auto GetD3DTexture2DPtr() const { return m_texture2D.Get(); }
	auto GetD3DShaderResourceView() const { return m_shaderResourceView; }
	auto GetD3DShaderResourceViewPtr() const { return m_shaderResourceView.Get(); }
	auto GetD3DUnorderedAccessView() const { return m_unorderedAccessView; }
	auto GetD3DUnorderedAccessViewPtr() const { return m_unorderedAccessView.Get(); }

	auto GetWidth() const { return m_width; }
	auto GetHeight() const { return m_height; }

	auto GetFormat() const { return m_format; }

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>           m_texture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  m_shaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_unorderedAccessView;
	uint32_t                                          m_width{ 0 };
	uint32_t                                          m_height{ 0 };
	uint32_t                                          m_mipCount{ 0 };
	TexelsFormat                                      m_format{ TexelsFormat::RGBA_U8 };
};

class GPUBuffer final
{

};

class Pipeline final
{

};

class Framebuffer final
{

};