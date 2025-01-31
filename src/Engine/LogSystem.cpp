#include "stdafx.h"
#include "LogSystem.h"
//=============================================================================
LogSystem* thisLogSystem = nullptr;
void RequestExit();
//=============================================================================
LogSystem::~LogSystem()
{
	assert(!thisLogSystem);
}
//=============================================================================
bool LogSystem::Create(const LogSystemCreateInfo& createInfo)
{
	assert(!thisLogSystem);

	thisLogSystem = this;
	return true;
}
//=============================================================================
void LogSystem::Destroy()
{
	thisLogSystem = nullptr;
}
//=============================================================================
void LogSystem::Print(const std::string& msg)
{
	assert(thisLogSystem);
	puts(msg.data());
}
//=============================================================================
void LogSystem::Debug([[maybe_unused]] const std::string& msg)
{
#if defined(_DEBUG)
	Print("[DEBUG] " + msg);
#endif
}
//=============================================================================
void LogSystem::Warning(const std::string& msg)
{
	Print("[WARNING] " + msg);
}
//=============================================================================
void LogSystem::Error(const std::string& msg)
{
	Print("[ERROR] " + msg);
}
//=============================================================================
void LogSystem::Fatal(const std::string& msg)
{
	Print("[FATAL] " + msg);
	RequestExit();
}
//=============================================================================