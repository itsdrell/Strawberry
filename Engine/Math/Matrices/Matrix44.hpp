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
	explicit Matrix44(const Vector3& iBasis, const Vector3& jBasis, const Vector3& kBasis, const Vector3& translation = Vector3::ZERO);

public:
	void SetIdentity();
	void SetPosition2D(const Vector2& pos);
	void Append( const Matrix44& matrixToAppend ); // a.k.a. Concatenate (right-multiply)
	void Translate2D( const Vector2& translation );

public:
	void GetValuesAsArray(float* outArray) const;

public:
	Vector3 GetForward() const;
	Vector3 GetRight() const;
	Vector3 GetUp() const;
	Vector3 GetPosition() const;

public:
	static Matrix44 MakeOrtho2D( const Vector2& mins, const Vector2& maxs );
	static Matrix44 MakeTranslation2D( const Vector2& translation );	
	static Matrix44 MakeOrtho3D( const Vector3& mins, const Vector3& maxs );
	static Matrix44 MakeView(Vector3 position, Vector3 target, Vector3 up = Vector3::UP);
	static Matrix44 LookAt(Vector3 position, Vector3 target, Vector3 up = Vector3::UP);

	static Matrix44 MakeRotationDegreesAroundZ(float rotationDegrees);
	static Matrix44 MakeRotationDegreesAroundY(float rotationDegrees);
	static Matrix44 MakeRotationDegreesAroundX(float rotationDegrees);
	static Matrix44 MakeRotationDegrees2D(float rotationDegreesAboutZ);

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