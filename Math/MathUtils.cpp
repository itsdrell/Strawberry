#include "MathUtils.hpp"
#include "Engine/Math/Vectors/Vector2.hpp"
#include <corecrt_math.h>
#include <stdlib.h>

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

//-----------------------------------------------------------------------------------------------
float ATan2fDegrees(float x, float y)
{
	return ConvertRadiansToDegrees(atan2f(y, x));
}

//-----------------------------------------------------------------------------------------------
float GetRandomFloat(float min, float max)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float differenceBetween = max - min;
	float temp = random * differenceBetween;
	return min + temp;
}

//-----------------------------------------------------------------------------------------------
int GetRandomIntRange(int minInclusive, int maxInclusive)
{
	int theRange = (maxInclusive - minInclusive) + 1;
	int answer = minInclusive + GetRandomIntLessThan(theRange);
	return answer;
}

//-----------------------------------------------------------------------------------------------
float GetRandomFloatZeroToOne()
{
	return (float)rand() / (float)RAND_MAX;
}

//-----------------------------------------------------------------------------------------------
int GetRandomIntLessThan(int maxNotInclusive)
{
	return rand() % maxNotInclusive;
}

//-----------------------------------------------------------------------------------------------
bool Chance(float chance)
{
	float diceRoll = GetRandomFloat(0.f, 100.f);

	if (chance >= diceRoll)
		return true;

	return false;
}

//-----------------------------------------------------------------------------------------------
float Interpolate(float start, float end, float fractionTowardEnd)
{
	return start * (1 - fractionTowardEnd) + (fractionTowardEnd * end);
}

//-----------------------------------------------------------------------------------------------
float ClampFloat(float currentValue, float min, float max)
{
	if (currentValue < min)
		return min;
	if (currentValue > max)
		return max;

	return currentValue;
}

//-----------------------------------------------------------------------------------------------
int ClampInt(int inValue, int min, int max)
{
	if (inValue < min)
		return min;
	if (inValue > max)
		return max;
	return inValue;
}

//-----------------------------------------------------------------------------------------------
float GetFractionOf(float value)
{
	double wholeNumber;
	return modf((double)value, &wholeNumber);
}

//-----------------------------------------------------------------------------------------------
float DotProduct(const Vector2 & a, const Vector2 & b)
{
	return (a.x * b.x) + (a.y * b.y);
}

//-----------------------------------------------------------------------------------------------
float GetDistance(const Vector2& a, const Vector2& b)
{
	// This can be optimized later if needed, I prefer breaking it down this way
	float firstStep = (b.x - a.x) * (b.x - a.x);
	float secondStep = (b.y - a.y) * (b.y - a.y);
	float answer = sqrtf((secondStep + firstStep));

	return answer;
}
