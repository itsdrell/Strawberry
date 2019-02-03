#pragma once
#include "Engine/Core/General/EngineCommon.hpp"

extern "C" {
# include "Engine/ThirdParty/Lua/lua.h"
# include "Engine/ThirdParty/Lua/lauxlib.h" 
# include "Engine/ThirdParty/Lua/lualib.h"
}

//====================================================================================
// Forward Declare
//====================================================================================


//====================================================================================
// Type Defs + Defines
//====================================================================================


//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================


//====================================================================================
// Classes
//====================================================================================
class LuaScript
{
public:
	LuaScript( const String& path );
	~LuaScript();

private:
	void AddLibrariesToLuaScript();
	void AddBindingsToScript();

public:
	lua_State* GetLuaState() { return m_state; }

public:
	void LogError();
	String LuaErrorCodeToString(int theCode);
	bool HasError();
	String GetErrorMessage();

private:
	lua_State*		m_state;
	String			m_filePath;
	
	int				m_errorFromLoad; // uses LUA defines
	int				m_errorFromPCall;
	String			m_errorMessage;
};

//====================================================================================
// Standalone C Functions
//====================================================================================
// These are the three functions I wanna use in my game like Love2D does
// We could "offer more" like unity does and then move these to another location
bool LuaStartUp( LuaScript& mainScript );
bool LuaUpdate( LuaScript& mainScript, float ds );
bool LuaRender( LuaScript& mainScript );

//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [2/2/2019]
//====================================================================================