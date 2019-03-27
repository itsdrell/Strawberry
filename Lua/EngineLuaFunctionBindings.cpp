#include "EngineLuaFunctionBindings.hpp"
#include "Engine/Lua/LuaScript.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/General/Camera.hpp"
#include "Engine/Core/Tools/ErrorWarningAssert.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Engine/Lua/LuaUtils.hpp"

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

// input
	BindFunctionToScript(theState, LuaIsKeyPressed, "IsKeyPressed");
	BindFunctionToScript(theState, LuaWasKeyJustPressed, "WasKeyJustPressed");
	BindFunctionToScript(theState, LuaWasKeyJustReleased, "WasKeyJustReleased");


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
	String value = LuaGetString(theState, 1, "idk man");
 	PrintLog(value.c_str()); // calling C++ function with this argument...
	return 0; // nothing to return!
}

//-----------------------------------------------------------------------------------------------
// Cls(r, g, b, a)
int LuaClearScreen(lua_State* theState)
{
	Renderer* r = Renderer::GetInstance();
	Rgba color = LuaGetRgba(theState, 1, r->m_defaultDrawColor);
	
	r->ClearScreen( color );
	
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
// DrawCircleFilled(centerX, centerY, radius, r, g, b, a)
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
// DrawCircleFilled(centerX, centerY, radius, r, g, b, a)
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

	Rgba color = LuaGetRgba(theState, 4, r->m_defaultDrawColor);

	Renderer::GetInstance()->DrawAABB2Outline(AABB2(minX, minY, maxX, maxY), color);

	return 0;
}

//-----------------------------------------------------------------------------------------------
// IsKeyPressed( stringCode )
int LuaIsKeyPressed(lua_State * theState)
{
	InputSystem* is = InputSystem::GetInstance();
	KeyCode keyToCheck = LuaGetKeyCode(theState, 1, 'z');

	lua_pushboolean(theState, is->IsKeyPressed(keyToCheck));
	
	return 1; // we are returning one variable
}

//-----------------------------------------------------------------------------------------------
// WasKeyJustPressed( stringCode )
int LuaWasKeyJustPressed(lua_State * theState)
{
	InputSystem* is = InputSystem::GetInstance();
	KeyCode keyToCheck = LuaGetKeyCode(theState, 1, 'z');

	lua_pushboolean(theState, is->WasKeyJustPressed(keyToCheck));

	return 1; // we are returning one variable
}

//-----------------------------------------------------------------------------------------------
// WasKeyJustReleased( stringCode )
int LuaWasKeyJustReleased(lua_State * theState)
{
	InputSystem* is = InputSystem::GetInstance();
	KeyCode keyToCheck = LuaGetKeyCode(theState, 1, 'z');

	lua_pushboolean(theState, is->WasKeyJustReleased(keyToCheck));

	return 1; // we are returning one variable
}
