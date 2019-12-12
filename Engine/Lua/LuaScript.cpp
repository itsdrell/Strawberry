#include "LuaScript.hpp"
#include "Engine/Core/Tools/ErrorWarningAssert.hpp"
#include "Engine/Lua/EngineLuaFunctionBindings.hpp"
#include "Engine/Core/Utils/StringUtils.hpp"
#include "Engine/Core/Platform/File.hpp"
#include "Engine/Lua/NativeLuaFunctions.hpp"

#pragma warning( disable : 4310 ) // cast truncate constant value (when we cast npos to int)

//===============================================================================================
LuaScript::LuaScript(const String & path, const String& includeDir, GameSideLuaFunctionBinding gameSideBinding)
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

	std::string theString = GetFileContentAsString(m_filePath.c_str()) + "\n"; // need padding for appends
	m_includes.push_back(IncludeFileData("Main.lua", (int) CountHowManyLinesAreInAString(theString)));

	ModifyLoadedLuaFileString(&theString, includeDir);

#ifndef EMSCRIPTEN_PORT
	if(gameSideBinding != nullptr)
		LogStringToFile("Data/fullScript.lua", theString.c_str(), true);
#endif

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
void LuaScript::ModifyLoadedLuaFileString(String* stringToModify, const String& includeDir)
{
	// we can do some includes
	GatherIncludeFilePaths(stringToModify, includeDir);
	
	// swap all operators we used to lua friendly versions
	// ex var += 2 becomes var = var + 2
	ChangeOperator(stringToModify, "+=");
	ChangeOperator(stringToModify, "-=");
	ChangeOperator(stringToModify, "*=");
	ChangeOperator(stringToModify, "/=");

	// var++ becomes var = var + 1
	ChangeOperator(stringToModify, "--");
	ChangeOperator(stringToModify, "++");

	// Add our native lua helpers 
	*stringToModify += g_NativeLuaLibrary;
}

//-----------------------------------------------------------------------------------------------
void LuaScript::ChangeOperator(String* stringToModify, const String& operatorToLookFor)
{
	String& theString = *stringToModify;

	int foundPosition = (int)theString.find(operatorToLookFor, 2);

	while (foundPosition != (int)std::string::npos)
	{
		int stepBackPos = foundPosition - 2;
		while ((stepBackPos >= 0) && (theString.at(stepBackPos) != ' ') && (theString.at(stepBackPos) != '\n'))
		{
			stepBackPos -= 1;
		}

		if (operatorToLookFor[0] == operatorToLookFor[1]) // ++ --
		{
			// since lua comments use -- we have to do some checks :l  
			if ((foundPosition < 3) || (theString[foundPosition - 1] == ' ') || (theString[foundPosition - 1] == '\n'))
			{
				int endOfLine = (int)theString.find("\n", foundPosition);
				foundPosition = (int)theString.find(operatorToLookFor, endOfLine);
				continue;
			}
			
			String variableName = theString.substr(stepBackPos + 1, foundPosition - stepBackPos - 1);
			theString.erase(theString.begin() + (foundPosition - variableName.size()) ,theString.begin() + foundPosition +2);

			String replacement = variableName + " = " + variableName + " " + operatorToLookFor.at(0) + " 1";
			
			theString.insert(foundPosition - variableName.size(), replacement);
			foundPosition = (int)theString.find(operatorToLookFor, foundPosition + (variableName.size() + 10));
		}
		else
		{
			String variableName = theString.substr(stepBackPos + 1, foundPosition - stepBackPos - 2);
			theString.erase(theString.begin() + foundPosition);
			
			String replacement = variableName + " " + operatorToLookFor.at(0) + " ";
			
			theString.insert(foundPosition + 2, replacement);
			foundPosition = (int)theString.find(operatorToLookFor, foundPosition);
		}
	}
}

//-----------------------------------------------------------------------------------------------
void LuaScript::GatherIncludeFilePaths(String* stringToModify, const String& includeDir)
{
	String& theString = *stringToModify;
	String includeString = "#include ";

	int foundPosition = (int)theString.find(includeString);
	while (foundPosition != (int)std::string::npos)
	{
		int endOfInclude = (int) theString.find('\n', foundPosition);
		String path = theString.substr(foundPosition + 10, endOfInclude - foundPosition - 11);

		String content;
		String fullPath = includeDir + "/Scripts/" + path;
		int lineCount = GetIncludeFileContent(fullPath, &content);

		theString.append(content);
		m_includes.push_back(IncludeFileData(path, lineCount));

		theString.erase(foundPosition, endOfInclude + 1);
		m_includes.at(0).m_lineCount -= 1;
		foundPosition = (int)theString.find(includeString);
	}
}

//-----------------------------------------------------------------------------------------------
int LuaScript::GetIncludeFileContent(const String& path, String* outContent)
{
	String fullPath = path;
	String content = GetFileContentAsString(fullPath.c_str());
	content += " \n";
	
	*outContent = content;
	return CountHowManyLinesAreInAString(content);
}

//-----------------------------------------------------------------------------------------------
void LuaScript::GetLineNumberAndIncludedFileName(const String& lineNumber, String* newLineNumber, String* fileName)
{
	// early out
	int line = ParseString(lineNumber, 0);
	if (m_includes.size() == 1 || line <= m_includes.at(0).m_lineCount)
	{
		*newLineNumber = std::to_string(line);
		*fileName = "Main.lua";
		return;
	}
	else
	{
		int previousTotal = 0;
		for (int i = 1; i < m_includes.size(); i++)
		{
			previousTotal += m_includes.at(i - 1).m_lineCount;
			int totalLine = (previousTotal + m_includes.at(i).m_lineCount);

			if (line <= totalLine)
			{
				// the plus 2 is weird cause it makes our syntax errors exact
				// but our runtime are off by 1 (needs a + 3). I think its
				// on luas end for not being 100% accurate :l 
				int realLineNumber = line - previousTotal; 
				*newLineNumber = std::to_string(realLineNumber);
				*fileName = m_includes.at(i).m_path;
				return;
			}
		}
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

	String modifiedLineNumber;
	String fileName;
	GetLineNumberAndIncludedFileName(lineNumber, &modifiedLineNumber, &fileName);

	m_errorMessage = 
		loadErrorMessage + 
		" \n In file: " + fileName +
		" \n Around Line: " + modifiedLineNumber +
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
