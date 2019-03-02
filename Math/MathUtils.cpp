#include "MathUtils.hpp"

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
