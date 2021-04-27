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
static int LuaDebugPrintLog(lua_State* theState);

//-----------------------------------------------------------------------------------------------
// Rendering
static int LuaClearScreen(lua_State* theState);
static int LuaSetDefaultDrawColor(lua_State* theState);

static int LuaDrawLine(lua_State* theState);

static int LuaDrawCircleFilled(lua_State* theState);
static int LuaDrawCircleOutline(lua_State* theState);

static int LuaDrawAABB2Filled(lua_State* theState);
static int LuaDrawAABB2WireFrame(lua_State* theState);

static int LuaDrawSprite(lua_State* theState);


static int LuaDrawText(lua_State* theState);
static int LuaDrawTextWrapped(lua_State* theState);
static int LuaDrawTextOverFlow(lua_State* theState);
static int LuaDrawTextShrinkToFit(lua_State* theState);

static int LuaSetCameraPosition(lua_State* theState);

//-----------------------------------------------------------------------------------------------
// Input
static int LuaIsKeyPressed(lua_State* theState);
static int LuaWasKeyJustPressed(lua_State* theState);
static int LuaWasKeyJustReleased(lua_State* theState);

// mouse
static int LuaIsMouseButtonPressed(lua_State* theState);
static int LuaWasMouseButtonJustPressed(lua_State* theState);
static int LuaWasMouseButtonJustReleased(lua_State* theState);

static int LuaDidMouseWheelScrollDown(lua_State* theState);
static int LuaDidMouseWheelScrollUp(lua_State* theState);

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
static int LuaInterpolate(lua_State* theState);
static int LuaChance(lua_State* theState);
static int LuaGetFractionOf(lua_State* theState);
static int LuaRangeMap(lua_State* theState);

// vec2
static int LuaGetDistance(lua_State* theState);
static int LuaGetDistanceSquared(lua_State* theState);
static int LuaNormalizeVector2D(lua_State* theState);
static int LuaDotProduct(lua_State* theState);
static int LuaGetLength(lua_State* theState);
static int LuaGetLengthSquared(lua_State* theState);

// Disc
static int LuaIsPointInsideDisc(lua_State* theState);

// AABB2
static int LuaGetIsPointInBox(lua_State* theState);
static int LuaAABB2GrowToSquare(lua_State* theState);
static int LuaAABB2ShrinkToSquare(lua_State* theState);
static int LuaGetAABB2FromAABB2(lua_State* theState);
static int LuaGetPositionInsideBox(lua_State* theState);

// Noise
static int Lua1DRawNoise(lua_State* theState);
static int Lua2DRawNoise(lua_State* theState);
static int Lua1DPerlin(lua_State* theState);
static int Lua2DPerlin(lua_State* theState);

//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [2/2/2019]
//====================================================================================