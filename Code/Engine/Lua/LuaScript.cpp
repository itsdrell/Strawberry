#include "LuaScript.hpp"
#include "Engine/Core/Tools/ErrorWarningAssert.hpp"
#include "Engine/Lua/EngineLuaFunctionBindings.hpp"

//===============================================================================================
LuaScript::LuaScript(const String & path)
{
	m_filePath = path;

	m_state = luaL_newstate();
	if(m_state == NULL)
	{
		ERROR_AND_DIE("Lua script at: " + path + " STATE could not be created");
	}

	AddBindingsToScript();

	int resultOfLoad = luaL_loadfile(m_state, path.c_str());
	if(resultOfLoad != LUA_OK)
		m_errorFromLoad = resultOfLoad;

	m_errorFromPCall = lua_pcall(m_state, 0, 0, 0);
	if(m_errorFromPCall != LUA_OK)
	{
		LogError();
		m_state = nullptr;
	}

	AddLibrariesToLuaScript();
}

//-----------------------------------------------------------------------------------------------
LuaScript::~LuaScript()
{
	if(m_state != NULL)
		lua_close(m_state);
}

//-----------------------------------------------------------------------------------------------
void LuaScript::AddLibrariesToLuaScript()
{
	// this adds all libraries. You can f12 the function
	// and see the list if you wanna be more specific. 
	// for now its ok
	if(m_state) luaL_openlibs(m_state);
}

//-----------------------------------------------------------------------------------------------
void LuaScript::AddBindingsToScript()
{
	BindLuaFunctionsToScript(m_state);
}

//-----------------------------------------------------------------------------------------------
void LuaScript::LogError()
{
	DebuggerPrintf("//---------------------------------------------------------------------------- \n");
	
	String loadErrorMessage = LuaErrorCodeToString(m_errorFromLoad);
	String stack = std::to_string(lua_gettop(m_state));

	const char *msg = lua_tostring(m_state, -1);
	m_errorMessage = 
		loadErrorMessage + 
		"\nStack: " + stack +
		"\nReason: " + String(msg) + "\n";
	lua_pop(m_state, 1);  /* remove message */

	DebuggerPrintf(m_errorMessage.c_str());
	DebuggerPrintf("//---------------------------------------------------------------------------- \n");
}

//-----------------------------------------------------------------------------------------------
String LuaScript::LuaErrorCodeToString(int theCode)
{
	if(theCode == LUA_ERRSYNTAX) return "Syntax Error! :O ";

	return "idk the error";
}

//-----------------------------------------------------------------------------------------------
bool LuaScript::HasError()
{
	return (m_errorFromLoad != LUA_OK || m_errorFromPCall != LUA_OK); 
}

//-----------------------------------------------------------------------------------------------
String LuaScript::GetErrorMessage()
{
	return m_errorMessage;
}

//===============================================================================================
bool LuaStartUp(LuaScript& mainScript)
{
	lua_State* theState = mainScript.GetLuaState();
	if(theState == nullptr) return false;

	lua_getglobal(theState, "Test");

	lua_pcall(theState, 1, 0, 0);

	return true;
}

//-----------------------------------------------------------------------------------------------
bool LuaUpdate(LuaScript& mainScript, float ds)
{
	lua_State* theState = mainScript.GetLuaState();
	if(theState == nullptr) return false;

	lua_getglobal(theState, "Update");
	lua_pushnumber(theState, ds);

	lua_pcall(theState, 1, 0, 0);

	return true;
}

//-----------------------------------------------------------------------------------------------
bool LuaRender(LuaScript & mainScript)
{
	lua_State* theState = mainScript.GetLuaState();
	if(theState == nullptr) return false;

	lua_getglobal(theState, "Draw");

	lua_pcall(theState, 0, 0, 0);

	return true;
}
