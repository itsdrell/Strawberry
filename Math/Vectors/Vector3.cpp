#include "Vector3.hpp"

Vector3::Vector3()
{
}

//-----------------------------------------------------------------------------------------------
Vector3::Vector3(float theX, float theY, float theZ)
{
	x = theX; 
	y = theY;
	z = theZ;
}

//-----------------------------------------------------------------------------------------------
Vector3::Vector3(const Vector2& xy)
{
	x = xy.x;
	y = xy.y;
	z = 0.f;
}
