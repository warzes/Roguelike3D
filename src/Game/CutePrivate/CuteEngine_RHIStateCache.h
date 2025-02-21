#pragma once

//=============================================================================
namespace RHIStateCache
{
	void Clear();

	ID3D11Buffer* vertexBuffer[MaxVertexBuffers] = { nullptr };
	UINT          vertexBufferStride[MaxVertexBuffers] = { 0 };
	UINT          vertexBufferOffset[MaxVertexBuffers] = { 0 };
	uint32_t      currentFreeVertexBuffer = 0;
} // namespace RHIStateCache