#pragma once

#include "ContextD3D11.h"

struct RHISystemCreateInfo final
{
	ContextD3D11CreateInfo context;
};

class RHISystem final
{
public:
	~RHISystem();

	[[nodiscard]]
	bool Create(const RHISystemCreateInfo& createInfo);
	void Destroy();

	bool Resize(uint32_t width, uint32_t height);

	void BeginFrame();
	void EndFrame();

private:
	ContextD3D11 m_context;
};