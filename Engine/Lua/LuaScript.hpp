 #pragma once
#include "Engine/Core/General/EngineCommon.hpp"

extern "C" {
# include "Engine/ThirdParty/Lua/lua.h"
# include "Engine/ThirdParty/Lua/lauxlib.h" 
# include "Engine/ThirdParty/Lua/lualib.h"
}

//====================================================================================
// Forward Declare
//====================================================================================


//====================================================================================
// Type Defs + Defines
//====================================================================================
typedef void(*GameSideLuaFunctionBinding)(lua_State* theState);

//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================
struct IncludeFileData 
{
	IncludeFileData(const String& path, int lineCount)
		: m_path(path)
		, m_lineCount(lineCount) {}

	String	m_path;
	int		m_lineCount;
};

//====================================================================================
// Classes
//====================================================================================
class LuaScript
{
public:
	// null for data
	LuaScript( const String& path , const String& includeDir = "", GameSideLuaFunctionBinding gameSideBinding = nullptr);
	~LuaScript();

private:
	void AddLibrariesToLuaScript();
	void AddBindingsToScript();
	void ModifyLoadedLuaFileString( String* stringToModify, const String& includeDir );
	void ChangeOperator(String* stringToModify, const String& operatorToLookFor);
	void GatherIncludeFilePaths(String* stringToModify, const String& includeDir);
	int GetIncludeFileContent(const String& path, String* outContent);
	void GetLineNumberAndIncludedFileName(const String& lineNumber, String* newLineNumber, String* fileName);

public:
	lua_State* GetLuaState() { return m_state; }

public:
	void LogError();
	void SetErrorCode(int theCode) { m_errorCode = theCode; }
	bool HasError();
	String LuaErrorCodeToString(int theCode);
	String GetErrorMessage();

private:
	lua_State*		m_state;
	String			m_filePath;
	
	int				m_errorCode = 0; // uses LUA defines
	String			m_errorMessage = "no errors";

	std::vector<IncludeFileData>	m_includes;
};

//====================================================================================
// Standalone C Functions
//====================================================================================
// These are the three functions I wanna use in my game like Love2D does
// We could "offer more" like unity does and then move these to another location
bool LuaStartUp( LuaScript& mainScript );
bool LuaUpdate( LuaScript& mainScript, float ds );
bool LuaRender( LuaScript& mainScript );

//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [2/2/2019]
//====================================================================================