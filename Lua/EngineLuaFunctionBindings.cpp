#include "EngineLuaFunctionBindings.hpp"
#include "Engine/Lua/LuaScript.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/General/Camera.hpp"
#include "Engine/Core/Tools/ErrorWarningAssert.hpp"

//===============================================================================================
void BindLuaFunctionsToScript(lua_State * theState)
{
	BindFunctionToScript(theState, DrawTestTriangle, "DrawTestTriangle");
	BindFunctionToScript(theState, PrintString, "Print");

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
int PrintString(lua_State* theState)
{
	const char* str = lua_tostring(theState, 1); // get function argument
	String text = String(str) + "\n";
	DebuggerPrintf(text.c_str()); // calling C++ function with this argument...
	return 0; // nothing to return!
}
