#pragma once
#include "Engine/Core/General/EngineCommon.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
class LuaScript;
class Rgba;

//====================================================================================
// Type Defs + Defines
//====================================================================================


//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================


//====================================================================================
// Classes
//====================================================================================
class BlackBoard
{
public:
	BlackBoard();

public:
	float GetValue(const String& name, float defaultValue) const;
	int GetValue(const String& name, int defaultValue) const;
	bool GetValue(const String& name, bool defaultValue) const;
	String GetValue(const String& name, const String& defaultValue) const;
	String GetValue(const String& name, const char* defaultValue) const;
	Rgba GetValue(const String& name, const Rgba& defaultValue) const;

private:
	void GetAllColorsFromGameConfigColorsTable();

private:
	LuaScript*		m_blackboardLuaScript;

};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================
extern BlackBoard* g_theGameBlackboard;

//====================================================================================
// Written by Zachary Bracken : [3/27/2019]
//====================================================================================