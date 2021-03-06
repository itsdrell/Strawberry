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
class Vector3
{
public:
	Vector3();
	Vector3( float theX, float theY, float theZ );
	Vector3(const Vector2& xy);


public:
	float GetLength() const;

public:
	const Vector3 operator+(const Vector3& vecToAdd) const;		// vec3 + vec3
	const Vector3 operator-(const Vector3& vecToSubtract) const;	// vec3 - vec3
	const Vector3 operator*(float uniformScale) const;			// vec3 * float
	void operator*=(const float uniformScale);					// vec3 *= float

	Vector3 operator-() const { return Vector3(-x, -y, -z); }
public:
	float x,y,z;


public:
	const static Vector3 ZERO;
	const static Vector3 ONE;
	const static Vector3 UP;
	const static Vector3 DOWN;
	const static Vector3 RIGHT;
	const static Vector3 LEFT;
	const static Vector3 FORWARD;
	const static Vector3 BACK;
};

//====================================================================================
// Standalone C Functions
//====================================================================================
Vector3		Normalize(const Vector3& vectorToNormalize);
Vector3		Cross(const Vector3& a, const Vector3& b);
float		DotProduct(const Vector3& a, const Vector3& b);

//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [1/29/2019]
//====================================================================================