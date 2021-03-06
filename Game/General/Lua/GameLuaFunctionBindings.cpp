#include "GameLuaFunctionBindings.hpp"
#include "Game/Main/Game.hpp"
#include "Engine/Renderer/Images/SpriteSheet.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Game/General/Map/Map.hpp"
#include "Engine/Lua/LuaUtils.hpp"
#include "Game/General/Map/Tile.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Lua/LuaScript.hpp"
#include "Engine/Core/General/Camera.hpp"
#include <cmath>

//===============================================================================================
void BindGameSideLuaFunctions(lua_State* theState)
{
	BindFunctionToScript(theState, LuaDrawMap, "DrawMap");
	BindFunctionToScript(theState, LuaMoveCamera, "Camera");
	BindFunctionToScript(theState, LuaCameraLookAt, "CameraLookAt");
	BindFunctionToScript(theState, LuaSetTileSprite, "SetTileSprite");
	BindFunctionToScript(theState, LuaGetTileSprite, "GetTileSprite");
	BindFunctionToScript(theState, LuaDoesTileHaveThisCollision, "DoesTileHaveCollision");
	BindFunctionToScript(theState, LuaScreenShake, "Screenshake");
	BindFunctionToScript(theState, LuaGetMousePosition, "GetMousePosition");
	BindFunctionToScript(theState, LuaSleep, "Sleep");
	BindFunctionToScript(theState, LuaTimeScale, "TimeScale");
	BindFunctionToScript(theState, LuaEvaluateCurve, "EvaluateCurve");
}

//===============================================================================================
// DrawMap()
int LuaDrawMap(lua_State* theState)
{
	UNUSED(theState);
	
	g_theGame->m_drawMap = true;

	Renderer* r = Renderer::GetInstance();
	r->DrawMesh(g_theGame->m_map->m_tileMesh);

	return 0;
}

//-----------------------------------------------------------------------------------------------
// Camera(x,y)
int LuaMoveCamera(lua_State* theState)
{
	float xPos = LuaGetFloat(theState, 1, 0.f);
	float yPos = LuaGetFloat(theState, 2, 0.f);

	g_theGame->m_cameraPos = Vector2(xPos, yPos);

	return 0;
}

//-----------------------------------------------------------------------------------------------
// CameraLookAt(x, y, bool)
int LuaCameraLookAt(lua_State* theState)
{
	float xPos = LuaGetFloat(theState, 1, 0.f);
	float yPos = LuaGetFloat(theState, 2, 0.f);
	bool clampWithMap = LuaGetBool(theState, 3, false);

	float padding = 128;
	xPos = (xPos - padding);
	yPos = (yPos - padding);
 
	if (clampWithMap)
	{
		AABB2 bounds = g_theGame->m_map->GetBounds();
		xPos = ClampFloat(xPos, 0, bounds.maxs.x - (padding * 2));
		yPos = ClampFloat(yPos, 0, bounds.maxs.y - (padding * 2));
	}

	g_theGame->m_cameraPos = Vector2(xPos, yPos);

	return 0;
}

//-----------------------------------------------------------------------------------------------
// SetTileSprite(x, y, spriteIndex)
int LuaSetTileSprite(lua_State * theState)
{
	AABB2 mapBounds = g_theGame->m_map->GetBounds();
	
	float xPos = ClampFloat(LuaGetFloat(theState, 1, 0.f), 0.f, mapBounds.maxs.x);
	float yPos = ClampFloat(LuaGetFloat(theState, 2, 0.f), 0.f, mapBounds.maxs.y);
	int spriteIndex = LuaGetInt(theState, 3, 0);
	
	TileSpriteInfo newInfo;
	newInfo.SetSpriteIndex(spriteIndex);
	newInfo.SetSpriteSheet((int) std::floorf(((float)spriteIndex) / 256.f));

	g_theGame->m_map->ChangeTileAtMousePos(Vector2(xPos, yPos), newInfo);

	return 0;
}

//-----------------------------------------------------------------------------------------------
// GetTileSprite(x,y)
// Takes a world pos not tile pos
int LuaGetTileSprite(lua_State * theState)
{
	AABB2 mapBounds = g_theGame->m_map->GetBounds();

	float xPos = ClampFloat(LuaGetFloat(theState, 1, 0.f), 0.f, mapBounds.maxs.x);
	float yPos = ClampFloat(LuaGetFloat(theState, 2, 0.f), 0.f, mapBounds.maxs.y);

	int index = g_theGame->m_map->GetTileIndexFromWorldPos(Vector2(xPos, yPos));
	Tile theTile = g_theGame->m_map->GetTileByIndex(index);

	lua_pushinteger(theState, theTile.m_spriteInfo.GetSpriteIndex());
	return 1;
}

//-----------------------------------------------------------------------------------------------
// DoesTileHaveCollision(x,y, channelToCheck);
int LuaDoesTileHaveThisCollision(lua_State * theState)
{
	AABB2 mapBounds = g_theGame->m_map->GetBounds();
	
	float xPos = ClampFloat(LuaGetFloat(theState, 1, 0.f), 0.f, mapBounds.maxs.x);
	float yPos = ClampFloat(LuaGetFloat(theState, 2, 0.f), 0.f, mapBounds.maxs.y);
	int channel = LuaGetInt(theState, 3, 0);

	int index = g_theGame->m_map->GetTileIndexFromWorldPos(Vector2(xPos, yPos));
	Tile theTile = g_theGame->m_map->GetTileByIndex(index);
	
	bool result = (theTile.m_spriteInfo.GetCollisionChannelValue() == channel);

	lua_pushboolean(theState, result);
	return 1;
}

//-----------------------------------------------------------------------------------------------
// Screenshake(amountZeroToOne)
int LuaScreenShake(lua_State* theState)
{
	float amount = LuaGetFloat(theState, 1, 0.f);

	g_theGame->m_screenshakeAmount = ClampFloat(g_theGame->m_screenshakeAmount + amount, 0.f, 1.f);
	
	return 0;
}

//-----------------------------------------------------------------------------------------------
// MousePos(bool)
// This is in game not engine because we are using the game camera and map bounds
int LuaGetMousePosition(lua_State* theState)
{
	AABB2 cameraBounds = g_theGameCamera->GetBounds();
	Vector2 cameraPos = g_theGame->m_cameraPos;

	Vector2 pos = GetMousePosition(cameraBounds);

	// always stay inside the camera
	pos.x = ClampFloat(pos.x, cameraPos.x, cameraPos.x + CAMERA_SCREEN_SIZE);
	pos.y = ClampFloat(pos.y, cameraPos.y, cameraPos.y + CAMERA_SCREEN_SIZE);

	lua_pushnumber(theState, pos.x);
	lua_pushnumber(theState, pos.y);

	return 2;
}

//-----------------------------------------------------------------------------------------------
// Sleep(seconds?)
int LuaSleep(lua_State* theState)
{
	float number = LuaGetFloat(theState, 1, 1.0f);

	g_theGame->m_sleepTimer = number;

	return 0;
}

//-----------------------------------------------------------------------------------------------
// TimeScale(scale)
int LuaTimeScale(lua_State* theState)
{
	float scale = LuaGetFloat(theState, 1, 1.0f);

	g_theGame->m_timeScale = scale;

	return 0;
}

//-----------------------------------------------------------------------------------------------
// EvaluateCurve(curveIndex, t)
int LuaEvaluateCurve(lua_State* theState)
{
	int curveNumber = LuaGetFloat(theState, 1, 1.0f);
	float t = LuaGetFloat(theState, 2, .5f);

	CurveData theCurve = g_theGame->m_curveData[curveNumber];
	float result = theCurve.Evaluate(t);

	lua_pushnumber(theState, result);

	return 1;
}
