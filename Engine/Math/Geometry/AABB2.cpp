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
bool AABB2::IsPointInBox(const Vector2& thePoint) const
{
	return (thePoint.x >= mins.x) && (thePoint.x <= maxs.x)
		&& (thePoint.y >= mins.y) && (thePoint.y <= maxs.y);
}

//-----------------------------------------------------------------------------------------------
void AABB2::GrowToSquare()
{
	float width = GetWidth();
	float height = GetHeight();

	if (width == height)
		return;

	if (width < height)
	{
		float amountToUse = (height - width) *.5f;
		maxs.x += amountToUse;
		mins.x -= amountToUse;
	}
	else
	{
		float amountToUse = (width - height) *.5f;
		maxs.y += amountToUse;
		mins.y -= amountToUse;
	}
}

//-----------------------------------------------------------------------------------------------
void AABB2::ShrinkToSquare()
{
	float width = GetWidth();
	float height = GetHeight();

	if (width == height)
		return;

	if (width < height)
	{
		float amountToUse = (height - width) *.5f;
		maxs.y -= amountToUse;
		mins.y += amountToUse;
	}
	else
	{
		float amountToUse = (width - height) *.5f;
		maxs.x -= amountToUse;
		mins.x += amountToUse;
	}
}

//-----------------------------------------------------------------------------------------------
void AABB2::GrowBy(float size)
{
	GrowBy(size, size);
}

//-----------------------------------------------------------------------------------------------
void AABB2::GrowBy(float xGrowth, float yGrowth)
{
	xGrowth *= .5f;
	yGrowth *= .5f;

	mins.x -= xGrowth;
	mins.y -= yGrowth;

	maxs.x += xGrowth;
	maxs.y += yGrowth;
}

//-----------------------------------------------------------------------------------------------
void AABB2::ShrinkBy(float size)
{
	ShrinkBy(size, size);
}

//-----------------------------------------------------------------------------------------------
void AABB2::ShrinkBy(float xShrink, float yShrink)
{
	xShrink *= .5f;
	yShrink *= .5f;

	mins.x += xShrink;
	mins.y += yShrink;

	maxs.x -= xShrink;
	maxs.y -= yShrink;
}

//-----------------------------------------------------------------------------------------------
AABB2::AABB2(float minX, float minY, float maxX, float maxY)
{
	mins = Vector2(minX, minY);
	maxs = Vector2(maxX, maxY);
}

//-----------------------------------------------------------------------------------------------
void AABB2::Translate(float translationX, float translationY)
{

	if (translationX < 0)
	{
		maxs.x -= -translationX; // thats the same thing as adding... but it worked?
		mins.x -= -translationX;
	}
	else
	{
		maxs.x += translationX;
		mins.x += translationX;
	}

	if (translationY < 0)
	{
		maxs.y -= -translationY;
		mins.y -= -translationY;

	}
	else
	{
		maxs.y += translationY;
		mins.y += translationY;
	}


}

//-----------------------------------------------------------------------------------------------
void AABB2::Translate(const Vector2& pos)
{
	Translate(pos.x, pos.y);
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

//-----------------------------------------------------------------------------------------------
AABB2 GetBiggerAABB2FromAABB2(const AABB2& original, float xGrowth, float yGrowth)
{
	AABB2 bounds = original;
	bounds.GrowBy(xGrowth, yGrowth);
	return bounds;
}

//-----------------------------------------------------------------------------------------------
AABB2 GetSmallerAABB2FromAABB2(const AABB2& original, float xAmount, float yAmount)
{
	AABB2 bounds = original;
	bounds.ShrinkBy(xAmount, yAmount);
	return bounds;
}
