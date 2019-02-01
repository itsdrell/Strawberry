#include "Matrix44.hpp"


//===============================================================================================
Matrix44::Matrix44()
{
	this->SetIdentity();
}

//-----------------------------------------------------------------------------------------------
Matrix44::Matrix44(const float* sixteenValuesBasisMajor)
{
	Ix = sixteenValuesBasisMajor[0];
	Iy = sixteenValuesBasisMajor[1];
	Iz = sixteenValuesBasisMajor[2];
	Iw = sixteenValuesBasisMajor[3];

	Jx= sixteenValuesBasisMajor[4];
	Jy= sixteenValuesBasisMajor[5];
	Jz= sixteenValuesBasisMajor[6];
	Jw= sixteenValuesBasisMajor[7];

	Kx = sixteenValuesBasisMajor[8];
	Ky = sixteenValuesBasisMajor[9];
	Kz = sixteenValuesBasisMajor[10];
	Kw = sixteenValuesBasisMajor[11];

	Tx = sixteenValuesBasisMajor[12];
	Ty = sixteenValuesBasisMajor[13];
	Tz = sixteenValuesBasisMajor[14];
	Tw = sixteenValuesBasisMajor[15];
}

//-----------------------------------------------------------------------------------------------
void Matrix44::SetIdentity()
{
	// Set the diagonals
	Ix = Jy = Kz = Tw = 1.0f;

	// Set the rest to 0
	Iy = Iz = Iw = 0.0f;
	Jx = Jz = Jw = 0.0f;
	Kx = Ky = Kw = 0.0f;
	Tx = Ty = Tz = 0.0f;
}

//-----------------------------------------------------------------------------------------------
void Matrix44::Append(const Matrix44& matrixToAppend)
{
	Matrix44 old = Matrix44(&Ix); // Use the constructor that takes the memory pointer

	Ix = (old.Ix * matrixToAppend.Ix) + (old.Jx * matrixToAppend.Iy) + (old.Kx * matrixToAppend.Iz) + (old.Tx * matrixToAppend.Iw);
	Iy = (old.Iy * matrixToAppend.Ix) + (old.Jy * matrixToAppend.Iy) + (old.Ky * matrixToAppend.Iz) + (old.Ty * matrixToAppend.Iw);
	Iz = (old.Iz * matrixToAppend.Ix) + (old.Jz * matrixToAppend.Iy) + (old.Kz * matrixToAppend.Iz) + (old.Tz * matrixToAppend.Iw);
	Iw = (old.Iw * matrixToAppend.Ix) + (old.Jw * matrixToAppend.Iy) + (old.Kw * matrixToAppend.Iz) + (old.Tw * matrixToAppend.Iw);

	Jx = (old.Ix * matrixToAppend.Jx) + (old.Jx * matrixToAppend.Jy) + (old.Kx * matrixToAppend.Jz) + (old.Tx * matrixToAppend.Jw);
	Jy = (old.Iy * matrixToAppend.Jx) + (old.Jy * matrixToAppend.Jy) + (old.Ky * matrixToAppend.Jz) + (old.Ty * matrixToAppend.Jw);
	Jz = (old.Iz * matrixToAppend.Jx) + (old.Jz * matrixToAppend.Jy) + (old.Kz * matrixToAppend.Jz) + (old.Tz * matrixToAppend.Jw);
	Jw = (old.Iw * matrixToAppend.Jx) + (old.Jw * matrixToAppend.Jy) + (old.Kw * matrixToAppend.Jz) + (old.Tw * matrixToAppend.Jw);

	Kx = (old.Ix * matrixToAppend.Kx) + (old.Jx * matrixToAppend.Ky) + (old.Kx * matrixToAppend.Kz) + (old.Tx * matrixToAppend.Kw);
	Ky = (old.Iy * matrixToAppend.Kx) + (old.Jy * matrixToAppend.Ky) + (old.Ky * matrixToAppend.Kz) + (old.Ty * matrixToAppend.Kw);
	Kz = (old.Iz * matrixToAppend.Kx) + (old.Jz * matrixToAppend.Ky) + (old.Kz * matrixToAppend.Kz) + (old.Tz * matrixToAppend.Kw);
	Kw = (old.Iw * matrixToAppend.Kx) + (old.Jw * matrixToAppend.Ky) + (old.Kw * matrixToAppend.Kz) + (old.Tw * matrixToAppend.Kw);

	Tx = (old.Ix * matrixToAppend.Tx) + (old.Jx * matrixToAppend.Ty) + (old.Kx * matrixToAppend.Tz) + (old.Tx * matrixToAppend.Tw);
	Ty = (old.Iy * matrixToAppend.Tx) + (old.Jy * matrixToAppend.Ty) + (old.Ky * matrixToAppend.Tz) + (old.Ty * matrixToAppend.Tw);
	Tz = (old.Iz * matrixToAppend.Tx) + (old.Jz * matrixToAppend.Ty) + (old.Kz * matrixToAppend.Tz) + (old.Tz * matrixToAppend.Tw);
	Tw = (old.Iw * matrixToAppend.Tx) + (old.Jw * matrixToAppend.Ty) + (old.Kw * matrixToAppend.Tz) + (old.Tw * matrixToAppend.Tw);
}

//-----------------------------------------------------------------------------------------------
void Matrix44::Translate2D(const Vector2& translation)
{
	Matrix44 translationMatrix = MakeTranslation2D(translation);
	Append(translationMatrix);
}

//-----------------------------------------------------------------------------------------------
Matrix44 Matrix44::MakeOrtho2D(const Vector2 & mins, const Vector2 & maxs)
{
	Matrix44 result = Matrix44(); // Get Identity 

	// Setting the diagonal
	result.Ix = (2)/(maxs.x - mins.x);
	result.Jy = (2)/(maxs.y - mins.y);

	// Set the T column
	result.Tx = -(maxs.x + mins.x) / (maxs.x - mins.x);
	result.Ty = -(maxs.y + mins.y) / (maxs.y - mins.y);
	result.Tw = 1.f;

	// The rest are all zeros
	return result;
}

//-----------------------------------------------------------------------------------------------
Matrix44 Matrix44::MakeTranslation2D(const Vector2& translation)
{
	Matrix44 result;
	result.Tx = translation.x;
	result.Ty = translation.y;

	return result;
}

//-----------------------------------------------------------------------------------------------
Matrix44 Matrix44::MakeOrtho3D(const Vector3& mins, const Vector3& maxs)
{
	Matrix44 result = Matrix44(); // Get Identity 

	// Setting the diagonal
	result.Ix = (2)/(maxs.x - mins.x);
	result.Jy = (2)/(maxs.y - mins.y);
	result.Kz = (2)/(maxs.z - mins.z);

	// Set the T column
	result.Tx = -(maxs.x + mins.x) / (maxs.x - mins.x);
	result.Ty = -(maxs.y + mins.y) / (maxs.y - mins.y);
	result.Tz = -(maxs.z + mins.z) / (maxs.z - mins.z);
	result.Tw = 1.f;

	// The rest are all zeros
	return result;
}
