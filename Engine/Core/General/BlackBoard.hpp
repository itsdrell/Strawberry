#pragma once
#include "Engine/Core/General/EngineCommon.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
class LuaScript;
class Rgba;
class Vector2;
class IntVector2;

//====================================================================================
// Type Defs + Defines
//====================================================================================


//====================================================================================
// ENUMS
//====================================================================================
enum eBlackboardTypes
{
	GAME_BLACKBOARD,
	ENGINE_BLACKBOARD,
	DATA_BLACKBOARD
};

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
	BlackBoard(const String& thePath, eBlackboardTypes type);

	~BlackBoard();

public:
	void CreateDataBlackboard();
	void CreateEngineBlackboard();
	void CreateGameBlackboard();

public:
	float GetValue(const String& name, float defaultValue) const;
	int GetValue(const String& name, int defaultValue) const;
	bool GetValue(const String& name, bool defaultValue) const;
	String GetValue(const String& name, const String& defaultValue) const;
	String GetValue(const String& name, const char* defaultValue) const;
	Rgba GetValue(const String& name, const Rgba& defaultValue) const;
	Vector2 GetValue(const String& name, const Vector2& defaultValue) const;
	IntVector2 GetValue(const String& name, const IntVector2& defaultValue) const;

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
extern BlackBoard* g_theEngineBlackboard;

//====================================================================================
// Written by Zachary Bracken : [3/27/2019]
//====================================================================================