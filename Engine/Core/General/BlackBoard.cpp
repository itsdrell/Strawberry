#include "BlackBoard.hpp"
#include "Engine/Lua/LuaScript.hpp"
#include "Engine/Lua/LuaUtils.hpp"

//===============================================================================================
BlackBoard* g_theGameBlackboard = nullptr;
BlackBoard* g_theEngineBlackboard = nullptr;

//===============================================================================================
BlackBoard::BlackBoard()
{
#ifndef EMSCRIPTEN_PORT
	m_blackboardLuaScript = new LuaScript("Scripts/GameConfig.lua");
#else
	m_blackboardLuaScript = new LuaScript("Scripts/GameConfig.lua");
#endif

	GetAllColorsFromGameConfigColorsTable();

	g_theGameBlackboard = this;
}

//-----------------------------------------------------------------------------------------------
BlackBoard::BlackBoard(const String& thePath, eBlackboardTypes type)
{
	m_blackboardLuaScript = new LuaScript(thePath);

	switch (type)
	{
	case GAME_BLACKBOARD:
		CreateGameBlackboard();
		break;
	case ENGINE_BLACKBOARD:
		CreateEngineBlackboard();
		break;
	case DATA_BLACKBOARD:
		CreateDataBlackboard();
		break;
	default:
		break;
	}
		
}

//-----------------------------------------------------------------------------------------------
BlackBoard::~BlackBoard()
{
	delete m_blackboardLuaScript;
	m_blackboardLuaScript = nullptr;
}

//-----------------------------------------------------------------------------------------------
void BlackBoard::CreateDataBlackboard()
{
	// ta do
}

//-----------------------------------------------------------------------------------------------
void BlackBoard::CreateEngineBlackboard()
{
	g_theEngineBlackboard = this;
}

//-----------------------------------------------------------------------------------------------
void BlackBoard::CreateGameBlackboard()
{
	GetAllColorsFromGameConfigColorsTable();
	g_theGameBlackboard = this;
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

//-----------------------------------------------------------------------------------------------
void BlackBoard::GetAllColorsFromGameConfigColorsTable()
{
	Rgba::s_defaultColors.clear();
	
	lua_State* L = m_blackboardLuaScript->GetLuaState();
	
	// us finding the global table and putting it on the stack
	lua_getglobal(L, "colors");
	lua_gettable(L, -1);
	lua_pushnil(L);

	// note: this does read them in reverse order of the table cause stack?
	while (lua_next(L, -2))
	{
		// default values
		String nameOfColor = LuaGetString(L, -2, "missing");
		Rgba color = Rgba(255, 255, 255, 255);
		int index = 0;
	
		lua_pushnil(L); // put sub/value table on stack
		while (lua_next(L, -2))
		{
			// note: -2 index is the key of the table at this point in the stack

			unsigned char currentValue = LuaGetUnsignedChar(L, -1, 69);

			if (index == 0) { color.r = currentValue; }
			if (index == 1) { color.g = currentValue; }
			if (index == 2) { color.b = currentValue; }
			if (index == 3) { color.a = currentValue; }

			index++;
			lua_pop(L, 1);
		}

		Rgba::AddColorToMap(nameOfColor, color);
		lua_pop(L, 1);
	}

	lua_pop(L, 1);
}

