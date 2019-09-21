#include "Engine/Math/Vectors/Vector2.hpp"
#include "Engine/Core/Utils/StringUtils.hpp"

//===============================================================================================
Vector2::Vector2()
{
	x = 0;
	y = 0;
}

//-----------------------------------------------------------------------------------------------
Vector2::Vector2(float theX, float theY)
{
	x = theX;
	y = theY;
}

//-----------------------------------------------------------------------------------------------
Vector2::Vector2(const std::string& theString)
{
	Strings theValues = SplitString(theString, ",");
	x = ParseString(theValues.at(0), 0.f);
	y = ParseString(theValues.at(1), 0.f);
}

//-----------------------------------------------------------------------------------------------
const Vector2 Vector2::operator+(const Vector2& vecToAdd) const
{
	return Vector2(vecToAdd.x + x, vecToAdd.y + y);
}

//-----------------------------------------------------------------------------------------------
std::string Vector2::ToString() const
{
	return std::to_string(x) + "," + std::to_string(y);
}
