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
	float		GetWidth() const;
	float		GetHeight() const;
	Vector2		GetDimensions() const;
	Vector2		GetPositionWithinBox(const Vector2& percentWithin);

public:
	bool		IsPointInBox(const Vector2& thePoint) const;

public:
	void Translate(const Vector2& pos);
	void Translate(float translationX, float translationY);

public:
	void GrowToSquare();
	void ShrinkToSquare();

public:
	// does .5 * size to mins, .5 to size to maxs
	void GrowBy(float size);
	void GrowBy(float xGrowth, float yGrowth);
	void ShrinkBy(float size);
	void ShrinkBy(float xShrink, float yShrink);

public:
	Vector2 mins, maxs;

public:
	const static AABB2 ZERO_TO_ONE;
};

//====================================================================================
// Standalone C Functions
//====================================================================================
AABB2 GetAABB2FromAABB2(const Vector2& minPercentage, const Vector2& maxPercentage, const AABB2& theBounds);
AABB2 GetBiggerAABB2FromAABB2(const AABB2& original, float xGrowth, float yGrowth);
AABB2 GetSmallerAABB2FromAABB2(const AABB2& original, float xAmount, float yAmount);

//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [1/29/2019]
//====================================================================================