#pragma once
#include "Engine/Math/Vectors/Vector2.hpp"
#include "Engine/Math/Vectors/Vector3.hpp"

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
class Matrix44
{
public:
	Matrix44();
	explicit Matrix44( const float* sixteenValuesBasisMajor ); // float[16] array in order Ix, Iy...

public:
	void SetIdentity();
	void Append( const Matrix44& matrixToAppend ); // a.k.a. Concatenate (right-multiply)
	void Translate2D( const Vector2& translation );

public:
	void GetValuesAsArray(float* outArray) const;

public:
	static Matrix44 MakeOrtho2D( const Vector2& mins, const Vector2& maxs );
	static Matrix44 MakeTranslation2D( const Vector2& translation );	
	static Matrix44 MakeOrtho3D( const Vector3& mins, const Vector3& maxs );

private:
	// Column Major (squirrels way)
	// Ix	Jx	Kx	Tx
	// Iy	Jy	Ky	Ty
	// Iz	Jz	Kz	Tz
	// Iw	Jw	Kw	Tw

	float	Ix, Iy, Iz, Iw,   Jx, Jy, Jz, Jw,   Kx, Ky, Kz, Kw,   Tx, Ty, Tz, Tw; // i, j, k and translation
};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [1/31/2019]
//====================================================================================