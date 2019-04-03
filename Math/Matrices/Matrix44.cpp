#include "Matrix44.hpp"
#include "../MathUtils.hpp"


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
Matrix44::Matrix44(const Vector3& iBasis, const Vector3& jBasis, const Vector3& kBasis, const Vector3& translation /*= Vector3::ZERO*/)
{
	this->SetIdentity();

	Ix = iBasis.x;
	Iy = iBasis.y;
	Iz = iBasis.z;

	Jx = jBasis.x;
	Jy = jBasis.y;
	Jz = jBasis.z;

	Kx = kBasis.x;
	Ky = kBasis.y;
	Kz = kBasis.z;

	Tx = translation.x;
	Ty = translation.y;
	Tz = translation.z;
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
void Matrix44::SetPosition2D(const Vector2& pos)
{
	Tx = pos.x;
	Ty = pos.y;
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
void Matrix44::GetValuesAsArray(float* outArray) const
{
	outArray[0] =	Ix;
	outArray[1] =	Iy;
	outArray[2] =	Iz;
	outArray[3] =	Iw;

	outArray[4] =	Jx;
	outArray[5] =	Jy;
	outArray[6] =	Jz;
	outArray[7] =	Jw;

	outArray[8] =	Kx;
	outArray[9] =	Ky;
	outArray[10] =	Kz;
	outArray[11] =	Kw;

	outArray[12] =	Tx;
	outArray[13] =	Ty;
	outArray[14] =	Tz;
	outArray[15] =	Tw; 
}

//-----------------------------------------------------------------------------------------------
Vector3 Matrix44::GetForward() const
{
	return Vector3(Kx, Ky, Kz);
}

//-----------------------------------------------------------------------------------------------
Vector3 Matrix44::GetRight() const
{
	return Vector3(Ix, Iy, Iz);
}

//-----------------------------------------------------------------------------------------------
Vector3 Matrix44::GetUp() const
{
	return Vector3(Jx, Jy, Jz);
}

//-----------------------------------------------------------------------------------------------
Vector3 Matrix44::GetPosition() const
{
	return Vector3(Tx, Ty, Tz);
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

//-----------------------------------------------------------------------------------------------
Matrix44 Matrix44::MakeView(Vector3 position, Vector3 target, Vector3 up)
{
	//	Formula (THIS MAY BE A RIGHT HANDED BASIS)
	//	https://msdn.microsoft.com/en-us/library/windows/desktop/bb281710(v=vs.85).aspx
	// 		zaxis = normal(cameraTarget - cameraPosition)
	// 		xaxis = normal(cross(cameraUpVector, zaxis))
	// 		yaxis = cross(zaxis, xaxis)
	// 
	// 		xaxis.x           yaxis.x           zaxis.x          0
	// 		xaxis.y           yaxis.y           zaxis.y          0
	// 		xaxis.z           yaxis.z           zaxis.z          0
	// 		-dot(xaxis, cameraPosition)  -dot(yaxis, cameraPosition)  -dot(zaxis, cameraPosition)  1

	Matrix44 view;

	Vector3 z = target - position;
	Vector3 zaxis = Normalize(z);
	Vector3 cross = Cross(up, zaxis);
	Vector3 xaxis = Normalize(cross);
	Vector3 yaxis = Cross(zaxis, xaxis);

	view.Ix = xaxis.x;
	view.Iy = yaxis.x;
	view.Iz = zaxis.x;
	view.Iw = 0;

	view.Jx = xaxis.y;
	view.Jy = yaxis.y;
	view.Jz = zaxis.y;
	view.Jw = 0;

	view.Kx = xaxis.z;
	view.Ky = yaxis.z;
	view.Kz = zaxis.z;
	view.Kw = 0;

	view.Tx = -DotProduct(xaxis, position);
	view.Ty = -DotProduct(yaxis, position);
	view.Tz = -DotProduct(zaxis, position);
	view.Tw = 1;


	return view;
}

//-----------------------------------------------------------------------------------------------
Matrix44 Matrix44::LookAt(Vector3 position, Vector3 target, Vector3 up)
{
	Vector3 direction = target - position;
	Vector3 forward = Normalize(direction);
	Vector3 right = Cross(up, forward);
	Vector3 normalizeRight = Normalize(right);
	Vector3 newUp = Cross(forward, normalizeRight);

	// Create a matrix
	return Matrix44(normalizeRight, newUp, forward, position);
}

//-----------------------------------------------------------------------------------------------
Matrix44 Matrix44::MakeRotationDegreesAroundZ(float rotationDegrees)
{
	return MakeRotationDegreesAroundZ(rotationDegrees);
}

//-----------------------------------------------------------------------------------------------
Matrix44 Matrix44::MakeRotationDegreesAroundY(float rotationDegrees)
{
	//https://en.wikipedia.org/wiki/Rotation_matrix#In_three_dimensions

	Matrix44 result;

	float cosValue = CosDegrees(rotationDegrees);
	float sinValue = SinDegrees(rotationDegrees);

	result.Ix = cosValue;
	result.Iz = -sinValue;
	result.Kx = sinValue;
	result.Kz = cosValue;

	return result;
}

//-----------------------------------------------------------------------------------------------
Matrix44 Matrix44::MakeRotationDegreesAroundX(float rotationDegrees)
{
	// https://en.wikipedia.org/wiki/Rotation_matrix#In_three_dimensions
	Matrix44 result;

	float cosValue = CosDegrees(rotationDegrees);
	float sinValue = SinDegrees(rotationDegrees);

	result.Jy = cosValue;
	result.Jz = sinValue;
	result.Ky = -sinValue;
	result.Kz = cosValue;

	return result;
}

//-----------------------------------------------------------------------------------------------
Matrix44 Matrix44::MakeRotationDegrees2D(float rotationDegreesAboutZ)
{
	Matrix44 result;

	//result.RotateDegrees2D(rotationDegreesAboutZ);

	float cosValue = CosDegrees(rotationDegreesAboutZ);
	float sinValue = SinDegrees(rotationDegreesAboutZ);

	result.Ix = cosValue;
	result.Iy = sinValue;
	result.Jx = -sinValue;
	result.Jy = cosValue;

	return result;
}
