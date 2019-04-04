#include "Engine/Math/Vectors/Vector2.hpp"

//===============================================================================================
Vector2::Vector2()
{
	x = 0;
	y = 0;
}

//-----------------------------------------------------------------------------------------------
Vector2::Vector2(float theX, float theY)
{
	x = theX;
	y = theY;
}

//-----------------------------------------------------------------------------------------------
const Vector2 Vector2::operator+(const Vector2& vecToAdd) const
{
	return Vector2(vecToAdd.x + x, vecToAdd.y + y);
}
