#pragma once
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Math/Vectors/Vector4.hpp"
#include <map>

//====================================================================================
// Forward Declare
//====================================================================================


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
class Rgba
{
public:
	Rgba();
	Rgba(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);

public:
	bool operator==(const Rgba& compare) const;

public:
	Vector4 GetAsNormalizedVector4() const;
	
public:
	static void AddColorToMap(const String& name, const Rgba& theColor);
	static Rgba GetColorByName(const String& name);
	static Rgba GetRandomColor();

public:
	static std::map<String, Rgba> s_defaultColors;

public:
	// 0 - 255
	unsigned char r,g,b,a = 255;


public:
	static Rgba WHITE;
	static Rgba STRAWBERRY_RED;

	static const Rgba BLACK;
	static const Rgba CYAN;
	static const Rgba MAGENTA;
	static const Rgba RED;
	static const Rgba ORANGE;
	static const Rgba YELLOW;
	static const Rgba GREEN;
	static const Rgba BLUE;
	static const Rgba INDIGO;
	static const Rgba VIOLET;

	// rainbow ;D
	static const Rgba RAINBOW_RED;
	static const Rgba RAINBOW_ORANGE;
	static const Rgba RAINBOW_YELLOW;
	static const Rgba RAINBOW_GREEN;
	static const Rgba RAINBOW_BLUE;
	static const Rgba RAINBOW_INDIGO;
	static const Rgba RAINBOW_VIOLET;
};


//====================================================================================
// Standalone C Functions
//====================================================================================
Rgba Interpolate(const Rgba& start, const Rgba& end, float t);

Rgba GetRainbowColor(int idx, int total = 7);
Rgba GetNextColorInRainbow(Rgba& currentColor);
Rgba GetRandomColorInRainbow();
Rgba InterpolateRainbow(Rgba& currentColor, float percent);

//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [1/29/2019]
//====================================================================================