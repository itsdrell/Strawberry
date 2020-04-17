#include "Engine/Math/Vectors/Vector2.hpp"
#include "Engine/Core/Utils/StringUtils.hpp"
#include "Engine/Math/Vectors/IntVector2.hpp"
#include <cmath>

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
Vector2::Vector2(const std::string& theString)
{
	Strings theValues = SplitString(theString, ",");
	x = ParseString(theValues.at(0), 0.f);
	y = ParseString(theValues.at(1), 0.f);
}

//-----------------------------------------------------------------------------------------------
const Vector2 Vector2::operator+(const Vector2& vecToAdd) const
{
	return Vector2(vecToAdd.x + x, vecToAdd.y + y);
}

//-----------------------------------------------------------------------------------------------
std::string Vector2::ToString() const
{
	return std::to_string(x) + "," + std::to_string(y);
}

//-----------------------------------------------------------------------------------------------
IntVector2 Vector2::GetAsIntVector2() const
{
	return IntVector2((int)x, (int)y);
}

//-----------------------------------------------------------------------------------------------
Vector2 Vector2::GetNormalized() const
{
	//First we need to find the distance
	float dis = GetLength();

	// todo check to see if length > 0
	if (dis == 0) { return Vector2(0.f, 0.f); }

	// Then we divide x,y by distance
	float xx = x / dis;
	float yy = y / dis;

	Vector2 answer = Vector2(xx, yy);
	return answer;
}

//-----------------------------------------------------------------------------------------------
float Vector2::GetLength() const
{
	float answer = std::sqrtf((x * x) + (y * y));
	return answer;
}

//-----------------------------------------------------------------------------------------------
float Vector2::GetLengthSquared() const
{
	float answer = (x * x) + (y * y);
	return answer;
}

//-----------------------------------------------------------------------------------------------
const Vector2 Vector2::operator-(const Vector2& vecToSubtract) const
{
	float newX = this->x - vecToSubtract.x;
	float newY = this->y - vecToSubtract.y;

	return Vector2(newX, newY);
}

//-----------------------------------------------------------------------------------------------
const Vector2 Vector2::operator*(float uniformScale) const
{
	float newX = this->x * uniformScale;
	float newY = this->y * uniformScale;

	return Vector2(newX, newY);
}

//-----------------------------------------------------------------------------------------------
const Vector2 Vector2::operator/(float inverseScale) const
{
	float scale = 1.0f / inverseScale;

	// 	float newX = this->x / inverseScale;
	// 	float newY = this->y / inverseScale;

	float newX = this->x * scale;
	float newY = this->y * scale;

	return Vector2(newX, newY);
}

//-----------------------------------------------------------------------------------------------
void Vector2::operator+=(const Vector2& vecToAdd)
{
	this->x = this->x + vecToAdd.x;
	this->y = this->y + vecToAdd.y;
}

//-----------------------------------------------------------------------------------------------
void Vector2::operator-=(const Vector2& vecToSubtract)
{
	this->x = this->x - vecToSubtract.x;
	this->y = this->y - vecToSubtract.y;
}

//-----------------------------------------------------------------------------------------------
void Vector2::operator*=(const float uniformScale)
{
	this->x = this->x * uniformScale;
	this->y = this->y * uniformScale;
}

//-----------------------------------------------------------------------------------------------
void Vector2::operator/=(const float uniformDivisor)
{
	// if its a constant its as cheap just to divide by it...?

	this->x = this->x / uniformDivisor;
	this->y = this->y / uniformDivisor;
}

//-----------------------------------------------------------------------------------------------
void Vector2::operator=(const Vector2& copyFrom)
{
	this->x = copyFrom.x;
	this->y = copyFrom.y;
}

//-----------------------------------------------------------------------------------------------
const Vector2 operator*(float uniformScale, const Vector2& vecToScale)
{
	float scale = uniformScale;

	float newX = vecToScale.x * scale;
	float newY = vecToScale.y * scale;

	return Vector2(newX, newY);
}

//-----------------------------------------------------------------------------------------------
bool Vector2::operator==(const Vector2& compare) const
{
	if (this->x == compare.x)
	{
		if (this->y == compare.y)
		{
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------------------------
bool Vector2::operator!=(const Vector2& compare) const
{
	// I could just call == and do the same thing but just so it's written out

	if (this->x == compare.x)
	{
		if (this->y == compare.y)
		{
			return false;
		}
	}

	return true;
}
