#include "AABB2.hpp"
#include "Engine/Math/MathUtils.hpp"

//===============================================================================================
const AABB2 AABB2::ZERO_TO_ONE = AABB2(0.f, 0.f, 1.f, 1.f);

//===============================================================================================
AABB2::AABB2()
{
	mins = Vector2(0.f, 0.f);
	maxs = Vector2(1.f, 1.f);
}

//-----------------------------------------------------------------------------------------------
AABB2::AABB2(const Vector2& theMins, const Vector2& theMaxs)
{
	mins = theMins;
	maxs = theMaxs;
}

//-----------------------------------------------------------------------------------------------
float AABB2::GetWidth() const
{
	return maxs.x - mins.x;
}

//-----------------------------------------------------------------------------------------------
float AABB2::GetHeight() const
{
	return maxs.y - mins.y;
}

//-----------------------------------------------------------------------------------------------
Vector2 AABB2::GetDimensions() const
{
	return Vector2(GetWidth(), GetHeight());
}

//-----------------------------------------------------------------------------------------------
Vector2 AABB2::GetPositionWithinBox(const Vector2 & percentWithin)
{
	Vector2 result;

	float x = Interpolate(mins.x, maxs.x, percentWithin.x);
	float y = Interpolate(mins.y, maxs.y, percentWithin.y);

	return Vector2(x,y);
}

//-----------------------------------------------------------------------------------------------
bool AABB2::IsPointInBox(const Vector2& thePoint)
{
	return (thePoint.x >= mins.x) && (thePoint.x <= maxs.x)
		&& (thePoint.y >= mins.y) && (thePoint.y <= maxs.y);
}

//-----------------------------------------------------------------------------------------------
AABB2::AABB2(float minX, float minY, float maxX, float maxY)
{
	mins = Vector2(minX, minY);
	maxs = Vector2(maxX, maxY);
}

//===============================================================================================
//C functions
//===============================================================================================
AABB2 GetAABB2FromAABB2(const Vector2& minPercentage, const Vector2& maxPercentage, const AABB2& theBounds)
{
	float width = theBounds.GetWidth();
	float height = theBounds.GetHeight();

	Vector2 newMins = Vector2(minPercentage.x * width, minPercentage.y * height);
	Vector2 newMaxs = Vector2(maxPercentage.x * width, maxPercentage.y * height);

	return AABB2(theBounds.mins + newMins, theBounds.mins + newMaxs);
}
