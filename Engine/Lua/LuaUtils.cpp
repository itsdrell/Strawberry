#include "LuaUtils.hpp"
#include "Engine/Lua/LuaScript.hpp"


//===============================================================================================
String LuaGetString(lua_State * theState, int stackIndex, const String & defaultValue)
{
	return lua_isstring(theState, stackIndex) ? lua_tostring(theState, stackIndex) : defaultValue;
}

//-----------------------------------------------------------------------------------------------
float LuaGetFloat(lua_State* theState, int stackIndex, float defaultValue)
{
	return (float) (lua_isnumber(theState, stackIndex) ? lua_tonumber(theState, stackIndex) : defaultValue);
}

//-----------------------------------------------------------------------------------------------
int LuaGetInt(lua_State* theState, int stackIndex, int defaultValue)
{
	return (int) (lua_isinteger(theState, stackIndex) ? lua_tointeger(theState, stackIndex) : defaultValue);
}

//-----------------------------------------------------------------------------------------------
unsigned char LuaGetUnsignedChar(lua_State* theState, int stackIndex, unsigned char defaultValue)
{
	return (unsigned char) (lua_isnumber(theState, stackIndex) ? lua_tointeger(theState, stackIndex) : defaultValue);
}

//-----------------------------------------------------------------------------------------------
bool LuaGetBool(lua_State * theState, int stackIndex, bool defaultValue)
{
	return lua_isboolean(theState, stackIndex) ? lua_toboolean(theState, stackIndex) : defaultValue;
}

//----------------------------------------------------------------------------------------------- 
Rgba LuaGetRgba(lua_State* theState, int startingStackIndex, const Rgba& defaultValue)
{
	Rgba color;
	
	// isstring didn't work so had to do !isNumber
	if (!lua_isnumber(theState, startingStackIndex))
	{
		String nameOfColor = LuaGetString(theState, startingStackIndex, "idkzac");

		if (nameOfColor == "idkzac")
			color = defaultValue;
		else if (nameOfColor == "random")
			color = Rgba::GetRandomColor();
		else
			color = Rgba::GetColorByName(nameOfColor);
	}
	else
	{
		unsigned char r = LuaGetUnsignedChar(theState, startingStackIndex, defaultValue.r);
		unsigned char g = LuaGetUnsignedChar(theState, startingStackIndex + 1, defaultValue.g);
		unsigned char b = LuaGetUnsignedChar(theState, startingStackIndex + 2, defaultValue.b);
		unsigned char a = LuaGetUnsignedChar(theState, startingStackIndex + 3, defaultValue.a);

		color = Rgba(r, g, b, a);
	}
	
	return color;
}

//-----------------------------------------------------------------------------------------------
KeyCode LuaGetKeyCode(lua_State* theState, int stackIndex, KeyCode defaultValue)
{
	// could force the string to always be lowercase but eh atm
	String keyCodeAsString = LuaGetString(theState, stackIndex, "error");

	// if its one then its a letter ex. 'a'
	if (keyCodeAsString.length() == 1)
	{
		return (unsigned char) keyCodeAsString.front();
	}
	else // special input like "space"
	{
		if (keyCodeAsString == "space") { return KEYBOARD_SPACE; }
		if (keyCodeAsString == "lshift") { return KEYBOARD_LSHIFT; }
		if (keyCodeAsString == "enter") { return KEYBOARD_ENTER; }
		if (keyCodeAsString == "backspace") { return KEYBOARD_BACKSPACE; }
		if (keyCodeAsString == "left") { return KEYBOARD_LEFT_ARROW; }
		if (keyCodeAsString == "right") { return KEYBOARD_RIGHT_ARROW; }
		if (keyCodeAsString == "up") { return KEYBOARD_UP_ARROW; }
		if (keyCodeAsString == "down") { return KEYBOARD_DOWN_ARROW; }
	}

	// if no cases matched, return default
	return defaultValue;
}

//-----------------------------------------------------------------------------------------------
String LuaGetGlobalString(lua_State* theState, const String& nameOfGlobal, const String& defaultValue)
{
	lua_getglobal(theState, nameOfGlobal.c_str());
	return LuaGetString(theState, -1, defaultValue);
}

//-----------------------------------------------------------------------------------------------
int LuaGetGlobalInt(lua_State * theState, const String & nameOfGlobal, int defaultValue)
{
	lua_getglobal(theState, nameOfGlobal.c_str());
	return LuaGetInt(theState, -1, defaultValue);
}

//-----------------------------------------------------------------------------------------------
float LuaGetGlobalFloat(lua_State * theState, const String & nameOfGlobal, float defaultValue)
{
	lua_getglobal(theState, nameOfGlobal.c_str());
	return LuaGetFloat(theState, -1, defaultValue);
}

//-----------------------------------------------------------------------------------------------
bool LuaGetGlobalBool(lua_State * theState, const String & nameOfGlobal, bool defaultValue)
{
	lua_getglobal(theState, nameOfGlobal.c_str());
	return LuaGetBool(theState, -1, defaultValue);
}

//-----------------------------------------------------------------------------------------------
int GetValueFromTable(lua_State * theState, const String & nameOfTableKey, int defaultValue)
{
	lua_pushstring(theState, nameOfTableKey.c_str());
	lua_gettable(theState, -2);

	int value = LuaGetInt(theState, -1, defaultValue);
	lua_pop(theState, 1);

	return value;
}

//-----------------------------------------------------------------------------------------------
Rgba LuaGetRgbaFromTable(lua_State * theState, const String & tableName, const Rgba & defaultValue)
{
	lua_getglobal(theState, tableName.c_str());

	if (!lua_istable(theState, -1))
		return defaultValue;
	
	unsigned char r = (unsigned char) GetValueFromTable(theState, "r", defaultValue.r);
	unsigned char g = (unsigned char) GetValueFromTable(theState, "g", defaultValue.g);
	unsigned char b = (unsigned char) GetValueFromTable(theState, "b", defaultValue.b);
	unsigned char a = (unsigned char) GetValueFromTable(theState, "a", defaultValue.a);

	return Rgba(r, g, b, a);
}

