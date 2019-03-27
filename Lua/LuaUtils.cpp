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
unsigned char LuaGetUnsignedChar(lua_State* theState, int stackIndex, unsigned char defaultValue)
{
	return (unsigned char) (lua_isnumber(theState, stackIndex) ? lua_tointeger(theState, stackIndex) : defaultValue);
}

//----------------------------------------------------------------------------------------------- 
Rgba LuaGetRgba(lua_State* theState, int startingStackIndex, const Rgba& defaultValue)
{
	unsigned char r = LuaGetUnsignedChar(theState, startingStackIndex, defaultValue.r);
	unsigned char g = LuaGetUnsignedChar(theState, startingStackIndex + 1, defaultValue.g);
	unsigned char b = LuaGetUnsignedChar(theState, startingStackIndex + 2, defaultValue.b);
	unsigned char a = LuaGetUnsignedChar(theState, startingStackIndex + 3, defaultValue.a);

	return Rgba(r, g, b, a);
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
