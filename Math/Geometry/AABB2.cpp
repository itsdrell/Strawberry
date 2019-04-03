#include "AABB2.hpp"

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
AABB2::AABB2(float minX, float minY, float maxX, float maxY)
{
	mins = Vector2(minX, minY);
	maxs = Vector2(maxX, maxY);
}
