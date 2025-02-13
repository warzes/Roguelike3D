#pragma once

//=============================================================================
#define DX_ERR(str,hr) Fatal( str + std::string(HRToString(hr)) + ".\n\tFile: " + std::string(__FILE__) + ", Line: " + std::to_string(__LINE__))
//=============================================================================
enum CreateRHIFlag
{
	rhi_vsync = 1 << 0,
};
typedef uint32_t CreateRHIFlags;

// Color space:
//  - BT.709 - LDR https://en.wikipedia.org/wiki/Rec._709
//  - BT.2020 - HDR https://en.wikipedia.org/wiki/Rec._2020
// Transfer function:
//  - G10 - linear (gamma 1.0)
//  - G22 - sRGB (gamma ~2.2)
//  - G2084 - SMPTE ST.2084 (Perceptual Quantization)
// Bits per channel:
//  - 8, 10, 16 (float)
enum class SwapChainFormat : uint8_t
{
	BT709_G10_16BIT,
	BT709_G22_8BIT,
	BT709_G22_10BIT,
	BT2020_G2084_10BIT
};
//=============================================================================