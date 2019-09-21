#pragma once
#include <string>

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
class IntVector2
{
public:
	IntVector2();
	IntVector2(int theX, int theY);
	IntVector2(const std::string& theString);

public:
	const IntVector2 operator+(const IntVector2& vecToAdd) const;
	const IntVector2 operator-(const IntVector2& vecToSubtract) const;
	const bool operator==(const IntVector2& vecToCompare) const;
	const bool operator!=(const IntVector2& vecToCompare) const;

public:
	std::string ToString() const;

public:
	int x,y;
};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [1/31/2019]
//====================================================================================