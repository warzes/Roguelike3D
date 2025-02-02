#include "stdafx.h"
#include "RHISystem.h"
//=============================================================================
RHISystem::~RHISystem()
{
}
//=============================================================================
bool RHISystem::Create(const RHISystemCreateInfo& createInfo)
{
	if (!m_context.Create(createInfo.context)) return false;

	return true;
}
//=============================================================================
void RHISystem::Destroy()
{
	m_context.Destroy();
}
//=============================================================================
bool RHISystem::Resize(uint32_t width, uint32_t height)
{
	return m_context.Resize(width, height);
}
//=============================================================================
void RHISystem::BeginFrame()
{
	m_context.BeginFrame();
}
//=============================================================================
void RHISystem::EndFrame()
{
	m_context.EndFrame();
}
//=============================================================================