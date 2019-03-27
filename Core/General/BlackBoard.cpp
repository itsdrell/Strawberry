#include "BlackBoard.hpp"
#include "Engine/Lua/LuaScript.hpp"
#include "Engine/Lua/LuaUtils.hpp"

//===============================================================================================
BlackBoard* g_theGameBlackboard = nullptr;

//===============================================================================================
BlackBoard::BlackBoard()
{
#ifndef EMSCRIPTEN_PORT
	m_blackboardLuaScript = new LuaScript("GameConfig.lua");
#else
	m_blackboardLuaScript = new LuaScript("Run_Win32/GameConfig.lua");
#endif
	
	g_theGameBlackboard = this;

	//lua_getglobal(m_blackboardLuaScript->GetLuaState(), "color");
	//int r = lua_tointeger(m_blackboardLuaScript->GetLuaState(), -3);
	Rgba color = LuaGetRgbaFromTable(m_blackboardLuaScript->GetLuaState(), "color", Rgba(255, 0, 0, 255));
}

//-----------------------------------------------------------------------------------------------
float BlackBoard::GetValue(const String & name, float defaultValue) const
{
	return LuaGetGlobalFloat(m_blackboardLuaScript->GetLuaState(), name.c_str(), defaultValue);
}

//-----------------------------------------------------------------------------------------------
int BlackBoard::GetValue(const String & name, int defaultValue) const
{
	return LuaGetGlobalInt(m_blackboardLuaScript->GetLuaState(), name.c_str(), defaultValue);
}

//-----------------------------------------------------------------------------------------------
bool BlackBoard::GetValue(const String & name, bool defaultValue) const
{
	return LuaGetGlobalBool(m_blackboardLuaScript->GetLuaState(), name.c_str(), defaultValue);
}

//-----------------------------------------------------------------------------------------------
String BlackBoard::GetValue(const String & name, const String & defaultValue) const
{
	return LuaGetGlobalString(m_blackboardLuaScript->GetLuaState(), name.c_str(), defaultValue);
}

//-----------------------------------------------------------------------------------------------
String BlackBoard::GetValue(const String& name, const char* defaultValue) const
{
	return GetValue(name, String(defaultValue));
}

//-----------------------------------------------------------------------------------------------
Rgba BlackBoard::GetValue(const String& name, const Rgba& defaultValue) const
{
	// we know this is a global table so we can use these helper functions
	return LuaGetRgbaFromTable(m_blackboardLuaScript->GetLuaState(), name, defaultValue);
}

