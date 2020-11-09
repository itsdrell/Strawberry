#include "Vector3.hpp"
#include "Engine/Math/MathUtils.hpp"

//===============================================================================================
const Vector3 Vector3::ZERO = Vector3(0.f, 0.f, 0.f);
const Vector3 Vector3::ONE = Vector3(1.f, 1.f, 1.f);
const Vector3 Vector3::UP = Vector3(0.f, 1.f, 0.f); // Y is up in my engine (unity)
const Vector3 Vector3::DOWN = Vector3(0.f, -1.f, 0.f);
const Vector3 Vector3::RIGHT = Vector3(1.f, 0.f, 0.f);
const Vector3 Vector3::LEFT = Vector3(-1.f, 0.f, 0.f);
const Vector3 Vector3::FORWARD = Vector3(0.f, 0.f, 1.f);
const Vector3 Vector3::BACK = Vector3(0.f, 0.f, -1.f);


//===============================================================================================
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

//-----------------------------------------------------------------------------------------------
float Vector3::GetLength() const
{
	return SquareRoot((x*x) + y * y + z * z);
}

//-----------------------------------------------------------------------------------------------
const Vector3 Vector3::operator+(const Vector3& vecToAdd) const
{
	float xx = x + vecToAdd.x;
	float yy = y + vecToAdd.y;
	float zz = z + vecToAdd.z;

	return Vector3(xx, yy, zz);
}

//-----------------------------------------------------------------------------------------------
const Vector3 Vector3::operator-(const Vector3& vecToSubtract) const
{
	float xx = x - vecToSubtract.x;
	float yy = y - vecToSubtract.y;
	float zz = z - vecToSubtract.z;

	return Vector3(xx, yy, zz);
}

//-----------------------------------------------------------------------------------------------
const Vector3 Vector3::operator*(float uniformScale) const
{
	float xx = x * uniformScale;
	float yy = y * uniformScale;
	float zz = z * uniformScale;

	return Vector3(xx, yy, zz);
}

//-----------------------------------------------------------------------------------------------
void Vector3::operator*=(const float uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
	z *= uniformScale;
}

//===============================================================================================
Vector3 Normalize(const Vector3 & vectorToNormalize)
{
	//First we need to find the distance
	float dis = vectorToNormalize.GetLength();

	// todo check to see if length > 0
	if (dis == 0) { return Vector3::ZERO; }

	// Then we divide x,y by distance
	float xx = vectorToNormalize.x / dis;
	float yy = vectorToNormalize.y / dis;
	float zz = vectorToNormalize.z / dis;

	return Vector3(xx, yy, zz);
}

//-----------------------------------------------------------------------------------------------
Vector3 Cross(const Vector3 & a, const Vector3 & b)
{
	float xx = (a.y*b.z) - (a.z * b.y);
	float yy = (a.z * b.x) - (a.x * b.z);
	float zz = (a.x * b.y) - (a.y * b.x);

	return Vector3(xx, yy, zz);
}

//-----------------------------------------------------------------------------------------------
float DotProduct(const Vector3 & a, const Vector3 & b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}
