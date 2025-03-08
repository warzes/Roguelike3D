#pragma once

#include "CuteMath.h"

// TODO: переписть под нормальный offset

struct VertexPos final
{
	VertexPos() = default;
	VertexPos(Vec3 inPos) : pos(inPos) {}
	VertexPos(float x, float y, float z) : pos(x, y, z) {}

	bool operator==(const VertexPos& other) const
	{
		return pos == other.pos;
	}

	Vec3 pos;

	static inline const std::vector<rhi::InputLayoutDescriptor> Layout = {
		{ "POSITION", 0, rhi::DataFormat::RGB32F, 0, 0 }
	};	
	static inline const uint32_t NumElements = Layout.size();
};
MAKE_HASHABLE(VertexPos, t.pos);

struct VertexPosUV final
{
	bool operator==(const VertexPosUV& other) const
	{
		return pos == other.pos && uv == other.uv;
	}

	Vec3 pos;
	Vec2 uv;

	static inline const std::vector<rhi::InputLayoutDescriptor> Layout = {
		{ "POSITION", 0, rhi::DataFormat::RGB32F, 0, 0            },
		{ "UV",       0, rhi::DataFormat::RG32F,  0, sizeof(Vec3) },
	};

	static inline const uint32_t NumElements = Layout.size();
};
MAKE_HASHABLE(VertexPosUV, t.pos, t.uv);

struct VertexPosUVNormals final
{
	bool operator==(const VertexPosUVNormals& other) const
	{
		return pos == other.pos && uv == other.uv && normal == other.normal;
	}

	Vec3 pos;
	Vec3 normal;
	Vec2 uv;

	static inline const std::vector<rhi::InputLayoutDescriptor> Layout = {
		{ "POSITION", 0, rhi::DataFormat::RGB32F, 0, 0                           },
		{ "NORMAL",   0, rhi::DataFormat::RGB32F, 0, sizeof(Vec3)                },
		{ "UV",       0, rhi::DataFormat::RG32F,  0, sizeof(Vec3) + sizeof(Vec3) },
	};

	static inline const uint32_t NumElements = Layout.size();
};
MAKE_HASHABLE(VertexPosUVNormals, t.pos, t.normal, t.uv);

struct VertexPosUVNormalsTangents final
{
	bool operator==(const VertexPosUVNormalsTangents& other) const
	{
		return pos == other.pos && uv == other.uv && normal == other.normal;
	}

	Vec3 pos;
	Vec3 normal;
	Vec2 uv;
	Vec3 tangent;

	static inline const std::vector<rhi::InputLayoutDescriptor> Layout = {
		{ "POSITION", 0, rhi::DataFormat::RGB32F, 0, 0                                          },
		{ "NORMAL",   0, rhi::DataFormat::RGB32F, 0, sizeof(Vec3)                               },
		{ "UV",       0, rhi::DataFormat::RG32F,  0, sizeof(Vec3) + sizeof(Vec3)                },
		{ "TANGENT",  0, rhi::DataFormat::RGB32F, 0, sizeof(Vec3) + sizeof(Vec3) + sizeof(Vec2) },
	};

	static inline const uint32_t NumElements = Layout.size();
};
MAKE_HASHABLE(VertexPosUVNormalsTangents, t.pos, t.normal, t.uv, t.tangent);

struct VertexPosColor final
{
	Vec3 pos;
	Vec3 color;

	bool operator==(const VertexPosColor& other) const
	{
		return pos == other.pos && color == other.color;
	}

	static inline const std::vector<rhi::InputLayoutDescriptor> Layout = {
		{ "POSITION", 0, rhi::DataFormat::RGB32F, 0, 0   },
		{ "COLOR",    0, rhi::DataFormat::RGB32F, 0, sizeof(Vec3) },
	};

	static inline const uint32_t NumElements = Layout.size();
};
MAKE_HASHABLE(VertexPosColor, t.pos, t.color);
