#pragma once

struct Image
{
	static Image LoadFromFile(const char* pFilePath);
	static Image CreateEmptyImage(size_t bytes);

	static Image CreateResizedImage(const Image& img, unsigned TargetWidth, unsigned TargetHeight);
	inline static Image CreateHalfResolutionFromImage(const Image& img) { return CreateResizedImage(img, img.x >> 1, img.y >> 1); }

	bool SaveToDisk(const char* pStrPath) const;
	void Destroy();  // Destroy must be called following a LoadFromFile() to prevent memory leak

	inline bool IsValid() const { return pData != nullptr && x != 0 && y != 0; }
	inline bool IsHDR() const { return BytesPerPixel > 4; }
	inline size_t GetSizeInBytes() const { return BytesPerPixel * x * y; }

	static unsigned short CalculateMipLevelCount(unsigned __int64 w, unsigned __int64 h);
	inline unsigned short CalculateMipLevelCount() const { return CalculateMipLevelCount(this->Width, this->Height); };

	union { int x; int Width; };
	union { int y; int Height; };
	int BytesPerPixel = 0;
	void* pData = nullptr;
	float MaxLuminance = 0.0f;
};