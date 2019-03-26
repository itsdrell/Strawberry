#include "EngineLuaFunctionBindings.hpp"
#include "Engine/Lua/LuaScript.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/General/Camera.hpp"
#include "Engine/Core/Tools/ErrorWarningAssert.hpp"
#include "../Math/Geometry/AABB2.hpp"

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
	String value = lua_tostring(theState, -1);
	lua_pop(theState, 1);//pop();
	//const char* str = lua_tostring(theState, 1); // get function argument
	//String text = String(str) + "\n";
	PrintLog(value.c_str()); // calling C++ function with this argument...
	return 0; // nothing to return!
}

//-----------------------------------------------------------------------------------------------
// Cls(r, g, b, a)
int LuaClearScreen(lua_State* theState)
{
	unsigned char r = (unsigned char)lua_tointeger(theState, 1);
	unsigned char g = (unsigned char)lua_tointeger(theState, 2);
	unsigned char b = (unsigned char)lua_tointeger(theState, 3);
	unsigned char a = (unsigned char)lua_tointeger(theState, 4);

	Renderer::GetInstance()->ClearScreen( Rgba(r, g, b, a));
	
	return 0;
}

//-----------------------------------------------------------------------------------------------
//  SetColor(r,g,b,a)
int LuaSetDefaultDrawColor(lua_State* theState)
{
	unsigned char r = (unsigned char)lua_tointeger(theState, 1);
	unsigned char g = (unsigned char)lua_tointeger(theState, 2);
	unsigned char b = (unsigned char)lua_tointeger(theState, 3);
	unsigned char a = (unsigned char)lua_tointeger(theState, 4);

	Renderer::GetInstance()->SetDefaultDrawColor(Rgba(r, g, b, a));

	return 0;
}

//-----------------------------------------------------------------------------------------------
// DrawLine(startX, startY, endX, endY, r, g, b, a)
int LuaDrawLine(lua_State * theState)
{
	float startX = lua_tonumber(theState, 1);
	float startY = lua_tonumber(theState, 2);
	float endX = lua_tonumber(theState, 3);
	float endY = lua_tonumber(theState, 4);

	unsigned char r = (unsigned char)lua_tointeger(theState, 5);
	unsigned char g = (unsigned char)lua_tointeger(theState, 6);
	unsigned char b = (unsigned char)lua_tointeger(theState, 7);
	unsigned char a = (unsigned char)lua_tointeger(theState, 8);

	Renderer::GetInstance()->DrawLine2D(Vector2(startX, startY), Vector2( endX, endY), Rgba(r, g, b, a));
	
	return 0;
}

//-----------------------------------------------------------------------------------------------
// DrawCircleFilled(centerX, centerY, radius, r, g, b, a)
int LuaDrawCircleFilled(lua_State * theState)
{
	float centerX = lua_tonumber(theState, 1);
	float centerY = lua_tonumber(theState, 2);
	float radius = lua_tonumber(theState, 3);

	unsigned char r = (unsigned char)lua_tointeger(theState, 4);
	unsigned char g = (unsigned char)lua_tointeger(theState, 5);
	unsigned char b = (unsigned char)lua_tointeger(theState, 6);
	unsigned char a = (unsigned char)lua_tointeger(theState, 7);

	Renderer::GetInstance()->DrawCircleFilled2D(Vector2(centerX, centerY), radius, Rgba(r, g, b, a));
	
	return 0;
}

//-----------------------------------------------------------------------------------------------
// DrawCircleFilled(centerX, centerY, radius, r, g, b, a)
int LuaDrawCircleOutline(lua_State * theState)
{
	float centerX = lua_tonumber(theState, 1);
	float centerY = lua_tonumber(theState, 2);
	float radius = lua_tonumber(theState, 3);

	unsigned char r = (unsigned char)lua_tointeger(theState, 4);
	unsigned char g = (unsigned char)lua_tointeger(theState, 5);
	unsigned char b = (unsigned char)lua_tointeger(theState, 6);
	unsigned char a = (unsigned char)lua_tointeger(theState, 7);

	Renderer::GetInstance()->DrawCircleOutline2D(Vector2(centerX, centerY), radius, Rgba(r, g, b, a));

	return 0;
}

//-----------------------------------------------------------------------------------------------
// DrawAABB2(minX, minY, maxX, maxY, r, g, b, a)
int LuaDrawAABB2Filled(lua_State* theState)
{
	float minX = lua_tonumber(theState, 1);
	float minY = lua_tonumber(theState, 2);
	float maxX = lua_tonumber(theState, 3);
	float maxY = lua_tonumber(theState, 4);

	unsigned char r = (unsigned char)lua_tointeger(theState, 5);
	unsigned char g = (unsigned char)lua_tointeger(theState, 6);
	unsigned char b = (unsigned char)lua_tointeger(theState, 7);
	unsigned char a = (unsigned char)lua_tointeger(theState, 8);

	Renderer::GetInstance()->DrawAABB2Filled(AABB2(minX, minY, maxX, maxY), Rgba(r, g, b, a));

	return 0;
}

//-----------------------------------------------------------------------------------------------
//  DrawAABB2(minX, minY, maxX, maxY, r, g, b, a)
int LuaDrawAABB2WireFrame(lua_State* theState)
{
	float minX = lua_tonumber(theState, 1);
	float minY = lua_tonumber(theState, 2);
	float maxX = lua_tonumber(theState, 3);
	float maxY = lua_tonumber(theState, 4);

	unsigned char r = (unsigned char)lua_tointeger(theState, 5);
	unsigned char g = (unsigned char)lua_tointeger(theState, 6);
	unsigned char b = (unsigned char)lua_tointeger(theState, 7);
	unsigned char a = (unsigned char)lua_tointeger(theState, 8);


	Renderer::GetInstance()->DrawAABB2Outline(AABB2(minX, minY, maxX, maxY), Rgba(r, g, b, a));

	return 0;
}

//-----------------------------------------------------------------------------------------------
// IsKeyPressed( stringCode )
int LuaIsKeyPressed(lua_State * theState)
{
	return 0;
}

//-----------------------------------------------------------------------------------------------
// WasKeyJustPressed( stringCode )
int LuaWasKeyJustPressed(lua_State * theState)
{
	return 0;
}

//-----------------------------------------------------------------------------------------------
// WasKeyJustReleased( stringCode )
int LuaWasKeyJustReleased(lua_State * theState)
{
	return 0;
}
