﻿#include "stdafx.h"
#include "Image.h"
#include "CuteUtility.h"
#include "Log.h"
#include <set>
#include <cmath>
#include <stb/stb_image.h>
#include <stb/stb_image_resize.h>
#include <stb/stb_image_write.h>
#include <tinyexr.h>
//=============================================================================
static const std::set<std::string> S_HDR_FORMATS = { "hdr", "exr" };
static bool IsHDRFileExtension(const std::string& ext) { return S_HDR_FORMATS.find(ext) != S_HDR_FORMATS.end(); }
//=============================================================================
static float CalculateMaxLuminance(const float* pData, int width, int height, int numComponents)
{
	float MaxLuminance = 0.0f;
	float MaxLuminance2 = 0.0f;
	float MaxLuminance3 = 0.0f;
	float MaxLightLevel = 0.0f;
	float LuminanceSum = 0.0f;

	for (int h = 0; h < height; ++h) // row
		for (int w = 0; w < width; ++w) // col
		{
			const int pxIndex = (w + width * h) * numComponents;
			const float& r = pData[pxIndex + 0];
			const float& g = pData[pxIndex + 1];
			const float& b = pData[pxIndex + 2];

			// https://en.wikipedia.org/wiki/Relative_luminance
			const float lum = 0.2126f * r + 0.7152f * g + 0.0722f * b;
			if (lum > MaxLuminance)
				MaxLuminance = lum;

			const float lum2 = 0.299f * r + 0.587f * g + 0.114f * b;
			if (lum2 > MaxLuminance2)
				MaxLuminance2 = lum2;

			const float lum3 = std::sqrtf(0.299f * r * r + 0.587f * g * g + 0.114f * b * b);
			if (lum3 > MaxLuminance3)
				MaxLuminance3 = lum3;



			if (MaxLightLevel <= r)
				MaxLightLevel = r;
			if (MaxLightLevel <= g)
				MaxLightLevel = g;
			if (MaxLightLevel <= b)
				MaxLightLevel = b;


			LuminanceSum += lum;;
		}

	// calc avg lum
	LuminanceSum /= width * height; // TODO:

	return MaxLuminance;
}
//=============================================================================
Image Image::LoadFromFile(const char* pFilePath)
{
	constexpr int reqComp = 0;

	const std::string Extension = directory::GetFileExtension(pFilePath);


	const bool bEXR = Extension == "exr";
	const bool bHDR = IsHDRFileExtension(Extension);

	Image img;

	int NumImageComponents = 0;
	if (bEXR)
	{
		int width;
		int height;
		const char* err = nullptr;
		int ret = LoadEXR((float**)&img.pData, &width, &height, pFilePath, &err);
		if (ret != TINYEXR_SUCCESS)
		{
			if (err)
			{
				Error("Couldn't load EXR file: " + std::string(err));
				FreeEXRErrorMessage(err); // Free error message.
			}
		}
	}
	else
	{
		img.pData = bHDR
			? (void*)stbi_loadf(pFilePath, &img.x, &img.y, &NumImageComponents, 4)
			: (void*)stbi_load(pFilePath, &img.x, &img.y, &NumImageComponents, 4);
	}

	if (img.pData == nullptr)
	{
		Error("Error loading file: " + std::string(pFilePath));
	}
	img.BytesPerPixel = bHDR
		? NumImageComponents * 4 // HDR=RGBA32F -> 16 Bytes/Pixel = 4 Bytes / component
		: NumImageComponents;    // SDR=RGBA8   -> 4  Bytes/Pixel = 1 Byte  / component

	if (img.pData && bHDR)
	{
		img.MaxLuminance = CalculateMaxLuminance(static_cast<const float*>(img.pData), img.Width, img.Height, 4);
	}

	return img;
}
//=============================================================================
Image Image::CreateEmptyImage(size_t bytes)
{
	Image img;
	img.pData = (void*)malloc(bytes);
	return img;
}
//=============================================================================
Image Image::CreateResizedImage(const Image& img, unsigned TargetWidth, unsigned TargetHeight)
{
	assert(TargetWidth > 0 && TargetHeight > 0);
	const int TargetResolution = TargetHeight * TargetWidth;
	const int TargetImageSizeInBytes = TargetResolution * (img.IsHDR() ? 16 : 4); // HDR is 16bytes/px (RGBA32F), SDR is 4bytes/px (RGBA8)
	assert(TargetImageSizeInBytes > 0);

	// create downsample image
	Image NewImage = CreateEmptyImage(TargetImageSizeInBytes);

	// 'resize' input image
	int rc = 0;
	if (img.IsHDR())
	{
		const int NUM_CHANNELS = 4; // RGBA
		const int STRIDE_BYTES_INPUT = 0;
		const int STRIDE_BYTES_OUTPUT = 0;
		rc = stbir_resize_float(reinterpret_cast<const float*>(img.pData), img.Width, img.Height, STRIDE_BYTES_INPUT,
			reinterpret_cast<float*>(NewImage.pData), TargetWidth, TargetHeight, STRIDE_BYTES_OUTPUT,
			NUM_CHANNELS
		);
	}
	else
	{
		rc = -1;
		assert(false); // TODO: impl
	}

	if (rc <= 0)
	{
		Error("Error resizing image ");
	}
	else
	{
		NewImage.Width = TargetWidth;
		NewImage.Height = TargetHeight;
		NewImage.MaxLuminance = img.MaxLuminance;
		NewImage.BytesPerPixel = img.BytesPerPixel;
	}

	return NewImage;
}
//=============================================================================
bool Image::SaveToDisk(const char* pStrPath) const
{
	if (this->GetSizeInBytes() == 0)
	{
		Warning("Image::SaveToDisk() failed: ImageSize=0 : " + std::string(pStrPath));
		return false;
	}

	const std::string Folder = directory::GetFolderPath(pStrPath);
	directory::CreateFolderIfItDoesntExist(Folder);

	const std::string Extension = directory::GetFileExtension(pStrPath);
	const bool bHDR = IsHDRFileExtension(Extension);

	int rc = 0;
	if (bHDR)
	{
		const int comp = 4; // HDR is 4 bytes / component (RGBA32F)
		rc = stbi_write_hdr(pStrPath, this->x, this->y, comp, reinterpret_cast<const float*>(this->pData));
	}
	else
	{
		assert(false); // TODO: test
		const int comp = this->BytesPerPixel;
		rc = stbi_write_png(pStrPath, this->x, this->y, comp, this->pData, 0);
	}

	return rc != 0; // 0 is err?
}
//=============================================================================
void Image::Destroy()
{
	if (pData)
	{
		stbi_image_free(pData);
		pData = nullptr;
	}
}
//=============================================================================
unsigned short Image::CalculateMipLevelCount(unsigned __int64 w, unsigned __int64 h)
{
	int mips = 0;
	while (w >= 1 && h >= 1)
	{
		++mips;
		w >>= 1;
		h >>= 1;
	}
	return mips;
}
//=============================================================================