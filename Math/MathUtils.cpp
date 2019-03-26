#include "MathUtils.hpp"
#include <corecrt_math.h>

//===============================================================================================
float RangeMapFloat(float inValue, float inStart, float inEnd, float outStart, float outEnd)
{
	if(inStart == inEnd)
	{
		return (outStart + outEnd) * .5f;
	}

	// Translate
	float inRange = inEnd - inStart;
	float outRange = outEnd - outStart;
	float inRelativeToStart = inValue - inStart;
	// Scale
	float fractionIntoRange = inRelativeToStart / inRange;
	float outRelativeToStart = fractionIntoRange * outRange;
	// Translate
	return outRelativeToStart + outStart;
}

//-----------------------------------------------------------------------------------------------
float ConvertRadiansToDegrees(float radians)
{
	// (180/pi) * radians)
	return (180.f / PI) * radians;
}

//-----------------------------------------------------------------------------------------------
float ConvertDegreesToRadians(float degrees)
{
	// (pi / 180) * degrees
	return (PI / 180.f) * degrees;
}

//-----------------------------------------------------------------------------------------------
float CosDegrees(float degrees)
{
	return cosf(ConvertDegreesToRadians(degrees));
}

//-----------------------------------------------------------------------------------------------
float SinDegrees(float degrees)
{
	return sinf(ConvertDegreesToRadians(degrees));
}
