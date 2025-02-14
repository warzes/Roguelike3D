#include "stdafx.h"
#include "Log.h"
//=============================================================================
void Warning(const std::string& msg)
{
	Print("[WARNING]: " + msg);
}
//=============================================================================
void Debug([[maybe_unused]] const std::string& msg)
{
#if defined(_DEBUG)
	Print("[DEBUG]: " + msg);
#endif
}
//=============================================================================
void Error(const std::string& msg)
{
	Print("[ERROR]: " + msg);
}
//=============================================================================