#pragma once
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Core/General/Rgba.hpp"
#include "Engine/Input/InputSystem.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
struct lua_State;

//====================================================================================
// Type Defs + Defines
//====================================================================================

//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Standalone C Functions
//====================================================================================

int LuaExecute(const String& command);
void LuaThrowError(lua_State* theState, const String& message);
void LuaGuaranteeOrDie(lua_State* theState, bool conditional, const String& message);
#define LUA_GUARANTEE_OR_DIE (state, conditional, message) (LuaGuaranteeOrDie(state, conditional, message))

//-----------------------------------------------------------------------------------------------
// Lua stack variables helpers
float				LuaGetFloat(lua_State* theState, int stackIndex, float defaultValue);
int					LuaGetInt(lua_State* theState, int stackIndex, int defaultValue);
unsigned char		LuaGetUnsignedChar(lua_State* theState, int stackIndex, unsigned char defaultValue);
bool				LuaGetBool(lua_State* theState, int stackIndex, bool defaultValue);
String				LuaGetString(lua_State* theState, int stackIndex, const String& defaultValue);
Rgba				LuaGetRgba(lua_State* theState, int startingStackIndex, const Rgba& defaultValue);

KeyCode				LuaGetKeyCode(lua_State* theState, int stackIndex, KeyCode defaultValue);


//-----------------------------------------------------------------------------------------------
// Globals
int					LuaGetGlobalInt(lua_State* theState, const String& nameOfGlobal, int defaultValue);
bool				LuaGetGlobalBool(lua_State* theState, const String& nameOfGlobal, bool defaultValue);
float				LuaGetGlobalFloat(lua_State* theState, const String& nameOfGlobal, float defaultValue);
String				LuaGetGlobalString(lua_State* theState, const String& nameOfGlobal, const String& defaultValue);

//-----------------------------------------------------------------------------------------------
// Tables
// these assume the table is at the top of the stack https://www.lua.org/pil/25.1.html
int GetValueFromTable(lua_State* theState, const String& nameOfTableKey, int defaultValue);

Rgba LuaGetRgbaFromTable(lua_State* theState, const String& tableName, const Rgba& defaultValue);



//====================================================================================
// Written by Zachary Bracken : [3/26/2019]
//====================================================================================