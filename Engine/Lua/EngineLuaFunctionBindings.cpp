#include "EngineLuaFunctionBindings.hpp"
#include "Engine/Lua/LuaScript.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/General/Camera.hpp"
#include "Engine/Core/Tools/ErrorWarningAssert.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Engine/Lua/LuaUtils.hpp"
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/General/Camera.hpp"
#include "Engine/Renderer/Images/Sprite.hpp"
#include "Engine/Core/Tools/Console.hpp"
#include "Engine/Renderer/Images/SpriteSheet.hpp"
#include <cstdlib>
#include <cmath>


//===============================================================================================
void BindLuaFunctionsToScript(lua_State * theState)
{
	BindFunctionToScript(theState, DrawTestTriangle, "DrawTestTriangle");
	BindFunctionToScript(theState, PrintString, "Print");

// rendering
	BindFunctionToScript(theState, LuaClearScreen, "Cls");
	BindFunctionToScript(theState, LuaSetDefaultDrawColor, "SetColor");
	BindFunctionToScript(theState, LuaDrawLine, "DrawLine");
	BindFunctionToScript(theState, LuaDrawCircleOutline, "DrawCircle");
	BindFunctionToScript(theState, LuaDrawCircleFilled, "DrawCircleFill");
	BindFunctionToScript(theState, LuaDrawAABB2Filled, "DrawAABB2Fill");
	BindFunctionToScript(theState, LuaDrawAABB2WireFrame, "DrawAABB2");
	BindFunctionToScript(theState, LuaDrawSprite, "DrawSprite");
	BindFunctionToScript(theState, LuaDrawText, "DrawText");

	BindFunctionToScript(theState, LuaSetCameraPosition, "Camera");

// input
	BindFunctionToScript(theState, LuaIsKeyPressed, "IsKeyPressed");
	BindFunctionToScript(theState, LuaWasKeyJustPressed, "WasKeyJustPressed");
	BindFunctionToScript(theState, LuaWasKeyJustReleased, "WasKeyJustReleased");

// Audio
	BindFunctionToScript(theState, LuaPlayOneShot, "PlayOneShot");
	BindFunctionToScript(theState, LuaPlayBackgroundMusic, "PlayMusic");
	BindFunctionToScript(theState, LuaStopMusic, "StopMusic");

// Math
	BindFunctionToScript(theState, LuaAbsoluteValue, "Abs");
	BindFunctionToScript(theState, LuaATan2, "ATan2");
	BindFunctionToScript(theState, LuaCosDegrees, "CosDegrees");
	BindFunctionToScript(theState, LuaSinDegrees, "SinDegrees");
	BindFunctionToScript(theState, LuaFloor, "Floor");
	BindFunctionToScript(theState, LuaCeil, "Ceil");
	BindFunctionToScript(theState, LuaMin, "Min");
	BindFunctionToScript(theState, LuaMax, "Max");
	BindFunctionToScript(theState, LuaSquareRoot, "Sqrt");
	BindFunctionToScript(theState, LuaClamp, "Clamp");
	BindFunctionToScript(theState, LuaRandomRange, "RandomRange");
	BindFunctionToScript(theState, LuaDotProduct, "Dot");
	BindFunctionToScript(theState, LuaInterpolate, "Lerp");
	BindFunctionToScript(theState, LuaChance, "Chance");
	BindFunctionToScript(theState, LuaGetFractionOf, "Fract");


// goes at the end
	lua_pcall(theState, 0, 0, 0);
}

//-----------------------------------------------------------------------------------------------
void BindFunctionToScript(lua_State* theState, FunctionForLua theFunction, const String& nameOfFunctionInLua)
{
	lua_pushcfunction(theState, theFunction);
	lua_setglobal(theState, nameOfFunctionInLua.c_str());
}

//===============================================================================================
int DrawTestTriangle(lua_State* theState)
{
	UNUSED(theState);
	
	Renderer* r = Renderer::GetInstance();

	// The code we want
	r->m_defaultCamera->SetProjectionOrthoByAspect(10.f);

	r->SetCamera(r->m_defaultCamera);
	r->SetShader(r->m_defaultShader);
	r->SetCurrentTexture();

	Vertex3D_PCU triangle[3] = 
	{
		Vertex3D_PCU(Vector3(1.f, 0.f, 1.f), Rgba(255,0,0), Vector2(0.f, 1.f)),
		Vertex3D_PCU(Vector3(0.f, 1.f, 1.f), Rgba(255,0,0), Vector2(0.f, 1.f)),
		Vertex3D_PCU(Vector3(-1.f, 0.f, 1.f), Rgba(255,0,0), Vector2(0.f, 1.f)),
	};

	r->DrawMeshImmediate(PRIMITIVE_TRIANGLES, triangle, 3); //DrawVertexArray

	return 0;
}

//-----------------------------------------------------------------------------------------------
// Print(string)
int PrintString(lua_State* theState)
{
	String value = "\n" + LuaGetString(theState, 1, "idk man");
 	PrintLog(value.c_str()); // calling C++ function with this argument...
	AddConsoleDialogue(value, Rgba::WHITE);
	return 0; // nothing to return!
}

//-----------------------------------------------------------------------------------------------
// Cls(r, g, b, a)
int LuaClearScreen(lua_State* theState)
{
	Renderer* r = Renderer::GetInstance();

	Rgba color = LuaGetRgba(theState, 1, r->m_clearScreenColor);

	// we clear the screen next time? 
	// Had a bug where it didn't like me clearing the screen late in the pipeline so doing it like this
	r->m_clearScreenColor = color;
	r->m_clearScreen = true;
	
	return 0;
}

//-----------------------------------------------------------------------------------------------
//  SetColor(r,g,b,a)
int LuaSetDefaultDrawColor(lua_State* theState)
{
	Renderer* r = Renderer::GetInstance();
	Rgba color = LuaGetRgba(theState, 1, r->m_defaultDrawColor);

	r->SetDefaultDrawColor(color);

	return 0;
}

//-----------------------------------------------------------------------------------------------
// DrawLine(startX, startY, endX, endY, r, g, b, a)
int LuaDrawLine(lua_State * theState)
{
	Renderer* r = Renderer::GetInstance();
	
	float startX = LuaGetFloat(theState, 1, 1000.f);
	float startY = LuaGetFloat(theState, 2, 1000.f);
	float endX = LuaGetFloat(theState, 3, 10000.f);
	float endY = LuaGetFloat(theState, 4, 1000.f);

	Rgba color = LuaGetRgba(theState, 5, r->m_defaultDrawColor);

	r->DrawLine2D(Vector2(startX, startY), Vector2( endX, endY), color);
	
	return 0;
}

//-----------------------------------------------------------------------------------------------
// DrawCircleFill(centerX, centerY, radius, r, g, b, a)
int LuaDrawCircleFilled(lua_State * theState)
{
	Renderer* r = Renderer::GetInstance();
	
	float centerX = LuaGetFloat(theState, 1, 0.f);
	float centerY = LuaGetFloat(theState, 2, 0.f);
	float radius = LuaGetFloat(theState, 3, 0.f);

	Rgba color = LuaGetRgba(theState, 4, r->m_defaultDrawColor);

	r->DrawCircleFilled2D(Vector2(centerX, centerY), radius, color);
	
	return 0;
}

//-----------------------------------------------------------------------------------------------
// DrawCircle(centerX, centerY, radius, r, g, b, a)
int LuaDrawCircleOutline(lua_State * theState)
{
	Renderer* r = Renderer::GetInstance();
	
	float centerX = LuaGetFloat(theState, 1, 0.f);
	float centerY = LuaGetFloat(theState, 2, 0.f);
	float radius = LuaGetFloat(theState, 3, 0.f);

	Rgba color = LuaGetRgba(theState, 4, r->m_defaultDrawColor);

	r->DrawCircleOutline2D(Vector2(centerX, centerY), radius, color);

	return 0;
}

//-----------------------------------------------------------------------------------------------
// DrawAABB2(minX, minY, maxX, maxY, r, g, b, a)
int LuaDrawAABB2Filled(lua_State* theState)
{
	Renderer* r = Renderer::GetInstance();
	
	float minX = LuaGetFloat(theState, 1, 0.f);
	float minY = LuaGetFloat(theState, 2, 0.f);
	float maxX = LuaGetFloat(theState, 3, 0.f);
	float maxY = LuaGetFloat(theState, 4, 0.f);

	Rgba color = LuaGetRgba(theState, 5, r->m_defaultDrawColor);

	r->DrawAABB2Filled(AABB2(minX, minY, maxX, maxY), color);

	return 0;
}

//-----------------------------------------------------------------------------------------------
//  DrawAABB2(minX, minY, maxX, maxY, r, g, b, a)
int LuaDrawAABB2WireFrame(lua_State* theState)
{
	Renderer* r = Renderer::GetInstance();
	
	float minX = LuaGetFloat(theState, 1, 0.f);
	float minY = LuaGetFloat(theState, 2, 0.f);
	float maxX = LuaGetFloat(theState, 3, 0.f);
	float maxY = LuaGetFloat(theState, 4, 0.f);

	Rgba color = LuaGetRgba(theState, 5, r->m_defaultDrawColor);

	Renderer::GetInstance()->DrawAABB2Outline(AABB2(minX, minY, maxX, maxY), color);

	return 0;
}

//-----------------------------------------------------------------------------------------------
// DrawSprite(index, x, y, rotation, width, height, flip X, flipY, pixelsPerUnit)
int LuaDrawSprite(lua_State* theState)
{
	int spriteIndex = LuaGetInt(theState, 1, 0);
	
	float x = LuaGetFloat(theState, 2, 1);
	float y = LuaGetFloat(theState, 3, 1);
	
	float rotation = LuaGetFloat(theState, 4, 0.f);
	
	float width = LuaGetFloat(theState, 5, 1.f);
	float height = LuaGetFloat(theState, 6, 1.f);

	bool flipX = LuaGetBool(theState, 7, false);
	bool flipY = LuaGetBool(theState, 8, false);
	
	float ppu = LuaGetFloat(theState, 9, 1.f);

	Vector2 dimensions = Vector2(width * SPRITE_DIMENSION * .5f, height * SPRITE_DIMENSION * .5f);
	AABB2 uvs = g_theSpriteSheet->GetTexCoordsForSpriteIndexAndDimensions(spriteIndex, IntVector2((int) width, (int) height));
	Sprite spriteToDraw = Sprite(g_theSpriteSheet->m_texture, dimensions, uvs, ppu);

	Renderer* r = Renderer::GetInstance();
	
	r->DrawSpriteRotated2D(Vector3(x,y,0.f), spriteToDraw, rotation, flipX, flipY);

	return 0;
}

//-----------------------------------------------------------------------------------------------
// DrawText string, x, y, height, color
// may get rid of height cause pico fixes it
int LuaDrawText(lua_State* theState)
{
	String text =	LuaGetString(theState, 1, "idk man");
	float x =		LuaGetFloat(theState, 2, 0);
	float y =		LuaGetFloat(theState, 3, 0);
	float height =	LuaGetFloat(theState, 4, 1.f);
	Rgba color =	LuaGetRgba(theState, 5, Rgba::WHITE);

	Renderer* r = Renderer::GetInstance();
	r->DrawText2D(Vector2(x, y), text, height, color);

	return 0;
}

//-----------------------------------------------------------------------------------------------
// Camera ( x, y )
int LuaSetCameraPosition(lua_State* theState)
{
	float x = LuaGetFloat(theState, 1, 0.f);
	float y = LuaGetFloat(theState, 2, 0.f);

	// do - because the view matrix is the inverse
	g_theGameCamera->m_viewMatrix.SetPosition2D(Vector2(-x, -y));

	return 0;
}

//-----------------------------------------------------------------------------------------------
// IsKeyPressed( stringCode )
int LuaIsKeyPressed(lua_State * theState)
{
	if (Console::GetInstance()->m_isOpen == true)
	{
		lua_pushboolean(theState, false);
		return 1;
	}
	
	InputSystem* is = InputSystem::GetInstance();
	KeyCode keyToCheck = LuaGetKeyCode(theState, 1, 'z');

	lua_pushboolean(theState, is->IsKeyPressed(keyToCheck));
	
	return 1; // we are returning one variable
}

//-----------------------------------------------------------------------------------------------
// WasKeyJustPressed( stringCode )
int LuaWasKeyJustPressed(lua_State * theState)
{
	if (Console::GetInstance()->m_isOpen == true)
	{
		lua_pushboolean(theState, false);
		return 1;
	}
	
	InputSystem* is = InputSystem::GetInstance();
	KeyCode keyToCheck = LuaGetKeyCode(theState, 1, 'z');

	lua_pushboolean(theState, is->WasKeyJustPressed(keyToCheck));

	return 1; // we are returning one variable
}

//-----------------------------------------------------------------------------------------------
// WasKeyJustReleased( stringCode )
int LuaWasKeyJustReleased(lua_State * theState)
{
	if (Console::GetInstance()->m_isOpen == true)
	{
		lua_pushboolean(theState, false);
		return 1;
	}
	
	InputSystem* is = InputSystem::GetInstance();
	KeyCode keyToCheck = LuaGetKeyCode(theState, 1, 'z');

	lua_pushboolean(theState, is->WasKeyJustReleased(keyToCheck));

	return 1; // we are returning one variable
}

//===============================================================================================
// Audio
//===============================================================================================
// PlayOneShot( stringPath )
int LuaPlayOneShot(lua_State * theState)
{
	String path = LuaGetString(theState, 1, "idk");
	String fileLocation = "Projects/" + g_currentProjectName + "/" + path;

	AudioSystem::GetInstance()->CreateOrGetSound(fileLocation);
	PlayOneShot(fileLocation);
	
	return 0;
}

//-----------------------------------------------------------------------------------------------
// PlayMusic( stringPath, bool loops )
int LuaPlayBackgroundMusic(lua_State * theState)
{
	String path = LuaGetString(theState, 1, "idk");
	String fileLocation = "Projects/" + g_currentProjectName + "/" + path;

	AudioSystem::GetInstance()->CreateOrGetSound(fileLocation);
	PlayLoopingSound(fileLocation);

	return 0;
}

//-----------------------------------------------------------------------------------------------
// StopMusic( stringPath )
int LuaStopMusic(lua_State * theState)
{
	String path = LuaGetString(theState, 1, "idk");
	String fileLocation = "Projects/" + g_currentProjectName + "/" + path;

	StopSound(fileLocation);

	return 0;
}

//===============================================================================================
// Math
//===============================================================================================
// Abs( number )
int LuaAbsoluteValue(lua_State * theState)
{
	float number = LuaGetFloat(theState, 1, 1.0f);
	float result = (float) std::abs((double) number);

	lua_pushnumber(theState, result);
	
	return 1;
}

//-----------------------------------------------------------------------------------------------
// ATan2( x, y )
int LuaATan2(lua_State * theState)
{
	float x = LuaGetFloat(theState, 1, 0.f);
	float y = LuaGetFloat(theState, 2, 1.f);
	
	// swaps the x and y in the function
	float result = ATan2fDegrees(x, y);

	lua_pushnumber(theState, result);

	return 1;
}

//-----------------------------------------------------------------------------------------------
// Cos( x )
int LuaCosDegrees(lua_State * theState)
{
	float degrees = LuaGetFloat(theState, 1, 0.f);
	float result = CosDegrees(degrees);

	lua_pushnumber(theState, result);
	
	return 1;
}

//-----------------------------------------------------------------------------------------------
// Sin( x )
int LuaSinDegrees(lua_State * theState)
{
	float degrees = LuaGetFloat(theState, 1, 0.f);
	float result = SinDegrees(degrees);

	lua_pushnumber(theState, result);

	return 1;
}

//-----------------------------------------------------------------------------------------------
// Floor ( x )
int LuaFloor(lua_State * theState)
{
	float number = LuaGetFloat(theState, 1, 0.f);
	float result = floorf(number);

	lua_pushnumber(theState, result);

	return 1;
}

//-----------------------------------------------------------------------------------------------
// Ceil( x )
int LuaCeil(lua_State * theState)
{
	float number = LuaGetFloat(theState, 1, 0.f);
	float result = ceilf(number);

	lua_pushnumber(theState, result);

	return 1;
}

//-----------------------------------------------------------------------------------------------
// Min (x,y)
int LuaMin(lua_State * theState)
{
	float a = LuaGetFloat(theState, 1, 0.f);
	float b = LuaGetFloat(theState, 2, 1.f);

	float result = Min(a, b);

	lua_pushnumber(theState, result);
	
	return 1;
}

//-----------------------------------------------------------------------------------------------
// Max (x , y)
int LuaMax(lua_State * theState)
{
	float a = LuaGetFloat(theState, 1, 0.f);
	float b = LuaGetFloat(theState, 2, 1.f);

	float result = Max(a, b);

	lua_pushnumber(theState, result);

	return 1;
}

//-----------------------------------------------------------------------------------------------
// Sqrt( x )
int LuaSquareRoot(lua_State * theState)
{
	float number = LuaGetFloat(theState, 1, 0.f);
	float result = sqrtf(number);

	lua_pushnumber(theState, result);

	return 1;
}

//-----------------------------------------------------------------------------------------------
// Clamp( number, min, max)
int LuaClamp(lua_State * theState)
{
	float currentValue = LuaGetFloat(theState, 1, 0.f);
	float min = LuaGetFloat(theState, 2, 0.f);
	float max = LuaGetFloat(theState, 3, 1.f);

	float result = ClampFloat(currentValue, min, max);

	lua_pushnumber(theState, result);

	return 1;
}

//-----------------------------------------------------------------------------------------------
// Random( min, max )
int LuaRandomRange(lua_State * theState)
{
	float min = LuaGetFloat(theState, 1, 0.f);
	float max = LuaGetFloat(theState, 2, 1.f);
	float result = GetRandomFloat(min, max);

	lua_pushnumber(theState, result);

	return 1;
}

//-----------------------------------------------------------------------------------------------
// DotProduct (x1, y1, x2, y2 )
int LuaDotProduct(lua_State * theState)
{
	float x1 = LuaGetFloat(theState, 1, 0.f);
	float y1 = LuaGetFloat(theState, 2, 0.f);
	float x2 = LuaGetFloat(theState, 3, 1.f);
	float y2 = LuaGetFloat(theState, 4, 1.f);

	float result = DotProduct(Vector2(x1, y1), Vector2(x2, y2));

	lua_pushnumber(theState, result);

	return 1;
}

//-----------------------------------------------------------------------------------------------
// Lerp ( start, end, t )
int LuaInterpolate(lua_State * theState)
{
	float start = LuaGetFloat(theState, 1, 0.f);
	float end = LuaGetFloat(theState, 2, 1.f);
	float t = LuaGetFloat(theState, 3, 1.f);

	float result = Interpolate(start, end, t);

	lua_pushnumber(theState, result);
	
	return 1;
}

//-----------------------------------------------------------------------------------------------
// Chance( percentChance )
int LuaChance(lua_State* theState)
{
	float chance = LuaGetFloat(theState, 1, 50.f);
	float result = Chance(chance);

	lua_pushnumber(theState, result);

	return 1;
}

//-----------------------------------------------------------------------------------------------
int LuaGetFractionOf(lua_State* theState)
{
	float value = LuaGetFloat(theState, 1, 50.f);
	float result = GetFractionOf(value);

	lua_pushnumber(theState, result);

	return 1;
}
