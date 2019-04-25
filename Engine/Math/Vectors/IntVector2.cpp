#include "IntVector2.hpp"

//===============================================================================================
IntVector2::IntVector2()
{
	x = 0;
	y = 0;
}

//-----------------------------------------------------------------------------------------------
IntVector2::IntVector2(int theX, int theY)
{
	x = theX;
	y = theY;
}

//-----------------------------------------------------------------------------------------------
const IntVector2 IntVector2::operator+(const IntVector2& vecToAdd) const
{
	return IntVector2(vecToAdd.x + x, vecToAdd.y + y);
}

//-----------------------------------------------------------------------------------------------
const IntVector2 IntVector2::operator-(const IntVector2& vecToSubtract) const
{
	return IntVector2(x - vecToSubtract.x, y - vecToSubtract.y);
}

//-----------------------------------------------------------------------------------------------
const bool IntVector2::operator==(const IntVector2& vecToCompare) const
{
	if (vecToCompare.x == x && vecToCompare.y == y)
		return true;
	return false;
}
