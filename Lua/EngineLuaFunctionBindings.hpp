#pragma once
#include "Engine/Core/General/EngineCommon.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
struct lua_State;

//====================================================================================
// Type Defs + Defines
//====================================================================================
typedef int (*FunctionForLua)(lua_State* theState);


//====================================================================================
// Standalone C Functions
//====================================================================================
void BindLuaFunctionsToScript( lua_State* theState);

// just a helper to make the bind script more readable
void BindFunctionToScript( lua_State* theState, FunctionForLua theFunction, const String& nameOfFunctionInLua);

//===============================================================================================
// EXPOSED TO LUA
//===============================================================================================
static int DrawTestTriangle( lua_State* theState );
static int PrintString( lua_State* theState );

//-----------------------------------------------------------------------------------------------
// Rendering
static int LuaClearScreen(lua_State* theState);
static int LuaSetDefaultDrawColor(lua_State* theState);

static int LuaDrawLine(lua_State* theState);

static int LuaDrawCircleFilled(lua_State* theState);
static int LuaDrawCircleOutline(lua_State* theState);

static int LuaDrawAABB2Filled(lua_State* theState);
static int LuaDrawAABB2WireFrame(lua_State* theState);

//-----------------------------------------------------------------------------------------------
// Input
static int LuaIsKeyPressed(lua_State* theState);
static int LuaWasKeyJustPressed(lua_State* theState);
static int LuaWasKeyJustReleased(lua_State* theState);

//-----------------------------------------------------------------------------------------------
// Audio
static int LuaPlayOneShot(lua_State* theState);
static int LuaPlayBackgroundMusic(lua_State* theState);
static int LuaStopMusic(lua_State* theState);

//-----------------------------------------------------------------------------------------------
// M A T H
static int LuaAbsoluteValue(lua_State* theState);
static int LuaATan2(lua_State* theState);
static int LuaCosDegrees(lua_State* theState);
static int LuaSinDegrees(lua_State* theState);
static int LuaFloor(lua_State* theState);
static int LuaCeil(lua_State* theState);  
static int LuaMin(lua_State* theState);
static int LuaMax(lua_State* theState);
static int LuaSquareRoot(lua_State* theState);
static int LuaClamp(lua_State* theState);
static int LuaRandomRange(lua_State* theState);
static int LuaDotProduct(lua_State* theState);
static int LuaInterpolate(lua_State* theState);
static int LuaChance(lua_State* theState);

//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [2/2/2019]
//====================================================================================