#include "LuaScript.hpp"
#include "Engine/Core/Tools/ErrorWarningAssert.hpp"
#include "Engine/Lua/EngineLuaFunctionBindings.hpp"

//===============================================================================================
LuaScript::LuaScript(const String & path, GameSideLuaFunctionBinding gameSideBinding)
{
	m_filePath = path;

	m_state = luaL_newstate();
	if(m_state == NULL)
	{
		printf("Lua script at: %s  :STATE could not be created \n", m_filePath.c_str());
	}

	if(gameSideBinding)
		gameSideBinding(m_state);
	AddBindingsToScript();

	FILE* theFile;
	fopen_s(&theFile, m_filePath.c_str(), "r");

	std::string theString;
	int c;
	while ((c = std::fgetc(theFile)) != EOF) 
	{ 
		std::putchar(c);
		theString.push_back((char)c);
	}

	ModifyLoadedLuaFileString(&theString);

	int resultOfLoad = luaL_loadstring(m_state, theString.c_str());

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
void LuaScript::ModifyLoadedLuaFileString(String* stringToModify)
{
	// we can do some includes
	
	// swap all operators we used to lua friendly versions
	// ex var += 1 becomes var = var + 1
	ChangeOperator(stringToModify, "+=");
	ChangeOperator(stringToModify, "-=");
	ChangeOperator(stringToModify, "*=");
	ChangeOperator(stringToModify, "/=");
}

//-----------------------------------------------------------------------------------------------
void LuaScript::ChangeOperator(String* stringToModify, const String& operatorToLookFor)
{
	String& theString = *stringToModify;

	int foundPosition = (int)theString.find(operatorToLookFor);

	while (foundPosition != (int)std::string::npos)
	{
		int stepBackPos = foundPosition - 2;
		while ((stepBackPos >= 0) && (theString.at(stepBackPos) != ' ') && (theString.at(stepBackPos) != '\n'))
		{
			stepBackPos -= 1;
		}

		String variableName = theString.substr(stepBackPos + 1, foundPosition - stepBackPos - 2);

		theString.erase(theString.begin() + foundPosition);

		String replacement = variableName + " " + operatorToLookFor.at(0) + " ";
		theString.insert(foundPosition + 2, replacement);

		foundPosition = (int)theString.find(operatorToLookFor, foundPosition);
	}
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
