#pragma once
#include "Engine/Math/Vectors/Vector2.hpp"

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
class AABB2
{
public:
	AABB2();
	AABB2(float minX, float minY, float maxX, float maxY);
	AABB2(const Vector2& theMins, const Vector2& theMaxs);

public:
	Vector2 mins, maxs;


public:
	const static AABB2 ZERO_TO_ONE;
};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [1/29/2019]
//====================================================================================