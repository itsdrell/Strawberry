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
		printf("Lua script at: %s  :STATE could not be created \n", m_filePath.c_str());
	}

	AddBindingsToScript();

	int resultOfLoad = luaL_loadfile(m_state, m_filePath.c_str());
	if(resultOfLoad != LUA_OK)
	{
		m_errorCode = resultOfLoad;
		LogError();
		m_state = nullptr;
	}
	else // only do this if the load succeeds
	{
		int resultFromPCall = lua_pcall(m_state, 0, 0, 0);
		if(resultFromPCall != LUA_OK)
		{
			m_errorCode = resultFromPCall;
			LogError();
			m_state = nullptr;
		}
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
	PrintLog("\n//---------------------------------------------------------------------------- \n");
	
	String loadErrorMessage = LuaErrorCodeToString(m_errorCode);
	String msg = lua_tostring(m_state, -1);

	size_t leftColonPos = msg.find( ':' );
	size_t rightColonPos = msg.find( ':', leftColonPos + 1 );
	String lineNumber = String( msg, leftColonPos+1, (rightColonPos - leftColonPos - 1));

	m_errorMessage = 
		loadErrorMessage + 
		" \n On Line: " + lineNumber +  
		" \n Reason: " + String(msg) + " \n";
	lua_pop(m_state, 1);  /* remove message */

	PrintLog(m_errorMessage.c_str());
	PrintLog("//---------------------------------------------------------------------------- \n");
}

//-----------------------------------------------------------------------------------------------
String LuaScript::LuaErrorCodeToString(int theCode)
{
	if( theCode == LUA_ERRSYNTAX)	return "Syntax Error! :O ";
	if( theCode == LUA_ERRRUN)		return "Runtime Error! :O ";
	if (theCode == LUA_YIELD)		return "Yield Error?! :O ";
	if (theCode == LUA_ERRMEM)		return "Memory Error! :O ";
	if (theCode == LUA_ERRGCMM)		return "GCM Error?! :O ";
	if (theCode == LUA_ERRERR)		return "Error Error! :O ";

	return "idk the error";
}

//-----------------------------------------------------------------------------------------------
bool LuaScript::HasError()
{
	return (m_errorCode != LUA_OK); 
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

	lua_getglobal(theState, "StartUp");

	mainScript.SetErrorCode( lua_pcall(theState, 0, 0, 0) );

 	if (mainScript.HasError())
	{
		mainScript.LogError();
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------------------------
bool LuaUpdate(LuaScript& mainScript, float ds)
{
	lua_State* theState = mainScript.GetLuaState();
	if(theState == nullptr) return false;

	lua_getglobal(theState, "Update");
	lua_pushnumber(theState, ds);

	mainScript.SetErrorCode( lua_pcall(theState, 1, 0, 0) );

	if (mainScript.HasError())
	{
		mainScript.LogError();
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------------------------
bool LuaRender(LuaScript & mainScript)
{
	lua_State* theState = mainScript.GetLuaState();
	if(theState == nullptr) return false;

	lua_getglobal(theState, "Draw");

	mainScript.SetErrorCode( lua_pcall(theState, 0, 0, 0) );

	if (mainScript.HasError())
	{
		mainScript.LogError();
		return false;
	}

	return true;
}
