#pragma once
#include <string>

//====================================================================================
// Forward Declare
//====================================================================================
class IntVector2;

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
class Vector2
{
public:
	Vector2();
	Vector2( float theX, float theY );
	Vector2(const std::string& theString);

public:
	Vector2 operator-() const { return Vector2(-x, -y); }
	const Vector2 operator+(const Vector2& vecToAdd) const;
	const Vector2 operator-(const Vector2& vecToSubtract) const;	// vec2 - vec2
	const Vector2 operator*(float uniformScale) const;			// vec2 * float
	const Vector2 operator/(float inverseScale) const;			// vec2 / float
	void operator+=(const Vector2& vecToAdd);						// vec2 += vec2
	void operator-=(const Vector2& vecToSubtract);				// vec2 -= vec2
	void operator*=(const float uniformScale);					// vec2 *= float
	void operator/=(const float uniformDivisor);					// vec2 /= float
	void operator=(const Vector2& copyFrom);						// vec2 = vec2
	bool operator==(const Vector2& compare) const;				// vec2 == vec2
	bool operator!=(const Vector2& compare) const;				// vec2 != vec2

	friend const Vector2 operator*(float uniformScale, const Vector2& vecToScale);	// float * vec2

public:
	std::string ToString() const;
	IntVector2 GetAsIntVector2() const;
	Vector2 GetNormalized() const;
	float GetLength() const;
	float GetLengthSquared() const;

public:
	static Vector2 Interpolate(const Vector2& a, const Vector2& b, float percentIn) { return Interpolate(a, b, percentIn); }

public:
	float x,y;

public:
	const static Vector2 ZERO;
	const static Vector2 ONE;
};

//====================================================================================
// Standalone C Functions
//====================================================================================
Vector2 Interpolate(const Vector2& a, const Vector2& b, float percentIn);

//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [1/29/2019]
//====================================================================================