#pragma once
#include "Engine/Lua/EngineLuaFunctionBindings.hpp"

//====================================================================================
// Forward Declare
//====================================================================================


//====================================================================================
// Standalone C Functions
//====================================================================================

void BindGameSideLuaFunctions(lua_State* theState);

static int LuaDrawMap(lua_State* theState);

// move camera is pivoted bottom left, look at is centered
static int LuaMoveCamera(lua_State* theState);
static int LuaCameraLookAt(lua_State* theState);

static int LuaSetTileSprite(lua_State* theState);
static int LuaGetTileSprite(lua_State* theState);

static int LuaDoesTileHaveThisCollision(lua_State* theState);

static int LuaScreenShake(lua_State* theState);

static int LuaGetMousePosition(lua_State* theState);

static int LuaSleep(lua_State* theState);
static int LuaTimeScale(lua_State* theState);

static int LuaEvaluateCurve(lua_State* theState);


//====================================================================================
// Written by Zachary Bracken : [10/3/2019]
//====================================================================================