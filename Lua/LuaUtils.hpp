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


//-----------------------------------------------------------------------------------------------
// Function parameter helpers
float				LuaGetFloat(lua_State* theState, int stackIndex, float defaultValue);
unsigned char		LuaGetUnsignedChar(lua_State* theState, int stackIndex, unsigned char defaultValue);
String				LuaGetString(lua_State* theState, int stackIndex, const String& defaultValue);
Rgba				LuaGetRgba(lua_State* theState, int startingStackIndex, const Rgba& defaultValue);

KeyCode				LuaGetKeyCode(lua_State* theState, int stackIndex, KeyCode defaultValue);


//====================================================================================
// Written by Zachary Bracken : [3/26/2019]
//====================================================================================