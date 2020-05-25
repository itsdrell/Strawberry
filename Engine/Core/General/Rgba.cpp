#include "Rgba.hpp"
#include "Engine/Math/MathUtils.hpp"

//===============================================================================================
std::map<String, Rgba> Rgba::s_defaultColors;

Rgba Rgba::STRAWBERRY_RED = Rgba(210, 74, 97, 255);
Rgba Rgba::WHITE = Rgba(255, 255, 255, 255);

const Rgba Rgba::BLACK = Rgba(0, 0, 0, 255);
const Rgba Rgba::CYAN = Rgba(0, 255, 255, 255);
const Rgba Rgba::MAGENTA = Rgba(255, 0, 255);
const Rgba Rgba::RED = Rgba(255, 0, 0, 255);
const Rgba Rgba::YELLOW = Rgba(255, 255, 0, 255);
const Rgba Rgba::GREEN = Rgba(0, 255, 0, 255);
const Rgba Rgba::BLUE = Rgba(0, 0, 255, 255);
const Rgba Rgba::ORANGE = Rgba(255, 165, 0, 255);
const Rgba Rgba::INDIGO = Rgba(75, 0, 130, 255);
const Rgba Rgba::VIOLET = Rgba(138, 43, 226, 255);

// rainbow ;D
const Rgba Rgba::RAINBOW_RED = Rgba(250, 165, 178, 179);
const Rgba Rgba::RAINBOW_ORANGE = Rgba(255, 206, 161, 255);
const Rgba Rgba::RAINBOW_YELLOW = Rgba(252, 252, 188, 255);
const Rgba Rgba::RAINBOW_GREEN = Rgba(185, 250, 207, 255);
const Rgba Rgba::RAINBOW_BLUE = Rgba(179, 210, 255, 255);
const Rgba Rgba::RAINBOW_INDIGO = Rgba(214, 174, 247, 255);
const Rgba Rgba::RAINBOW_VIOLET = Rgba(250, 193, 248, 255);

//===============================================================================================
Rgba::Rgba()
{
}

//-----------------------------------------------------------------------------------------------
Rgba::Rgba(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha /*= 255*/)
{
	r = red;
	g = green;
	b = blue;
	a = alpha;
}

//-----------------------------------------------------------------------------------------------
bool Rgba::operator==(const Rgba& compare) const
{
	if (this->r == compare.r && this->g == compare.g
		&& this->b == compare.b && this->a == compare.a)
	{
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------------------------
Vector4 Rgba::GetAsNormalizedVector4() const
{
	float fRed = (float)r;
	float fGreen = (float)g;
	float fBlue = (float)b;
	float fAlpha = (float)a;

	fRed = RangeMapFloat(fRed,0.f,255.f,0.f,1.f);
	fGreen = RangeMapFloat(fGreen,0.f,255.f,0.f,1.f);
	fBlue = RangeMapFloat(fBlue,0.f,255.f,0.f,1.f);
	fAlpha = RangeMapFloat(fAlpha,0.f,255.f,0.f,1.f);

	return Vector4(fRed,fGreen,fBlue,fAlpha);
}

//-----------------------------------------------------------------------------------------------
STATIC void Rgba::AddColorToMap(const String& name, const Rgba& theColor)
{
	s_defaultColors.insert ( std::pair<String, Rgba>(name, theColor));
}

//-----------------------------------------------------------------------------------------------
STATIC Rgba Rgba::GetColorByName(const String& name)
{
	std::map<String, Rgba>::iterator theIterator;

	for (theIterator = s_defaultColors.begin(); theIterator != s_defaultColors.end(); theIterator++)
	{
		if (theIterator->first == name)
			return theIterator->second;
	}

	return Rgba(255, 255, 255, 255);
}

//-----------------------------------------------------------------------------------------------
STATIC Rgba Rgba::GetRandomColor()
{
	int colorIdex = GetRandomIntRange(0, (int)(s_defaultColors.size()) - 1);
	std::map<String, Rgba>::iterator theIterator = s_defaultColors.begin();

	for (int i = 0; i < colorIdex; i++)
		theIterator++;

	return theIterator->second;
}

//===============================================================================================
Rgba Interpolate(const Rgba& start, const Rgba& end, float t)
{
	unsigned char r = (unsigned char)(Interpolate(start.r, end.r, t));
	unsigned char g = (unsigned char)(Interpolate(start.g, end.g, t));
	unsigned char b = (unsigned char)(Interpolate(start.b, end.b, t));
	unsigned char a = (unsigned char)(Interpolate(start.a, end.a, t));

	return Rgba(r, g, b, a);
}

//-----------------------------------------------------------------------------------------------
Rgba GetRainbowColor(int idx, int total /*= 7*/)
{
	float percent = (float)idx / (float)total;

	if (percent < (1.f / 7.f))
		return Rgba::RAINBOW_RED;
	if (percent >= (1.f / 7.f) && percent < (2.f / 7.f))
		return Rgba::RAINBOW_ORANGE;
	if (percent >= (2.f / 7.f) && percent < (3.f / 7.f))
		return Rgba::RAINBOW_YELLOW;
	if (percent >= (3.f / 7.f) && percent < (4.f / 7.f))
		return Rgba::RAINBOW_GREEN;
	if (percent >= (4.f / 7.f) && percent < (5.f / 7.f))
		return Rgba::RAINBOW_BLUE;
	if (percent >= (5.f / 7.f) && percent < (6.f / 7.f))
		return Rgba::RAINBOW_INDIGO;
	if (percent >= (6.f / 7.f))
		return Rgba::RAINBOW_VIOLET;

	return Rgba::WHITE;
}

//-----------------------------------------------------------------------------------------------
Rgba GetNextColorInRainbow(Rgba & currentColor)
{

	if (currentColor == Rgba::RAINBOW_RED)
		return Rgba::RAINBOW_ORANGE;
	if (currentColor == Rgba::RAINBOW_VIOLET)
		return Rgba::RAINBOW_RED;
	if (currentColor == Rgba::RAINBOW_INDIGO)
		return Rgba::RAINBOW_VIOLET;
	if (currentColor == Rgba::RAINBOW_BLUE)
		return Rgba::RAINBOW_INDIGO;
	if (currentColor == Rgba::RAINBOW_GREEN)
		return Rgba::RAINBOW_BLUE;
	if (currentColor == Rgba::RAINBOW_YELLOW)
		return Rgba::RAINBOW_GREEN;
	if (currentColor == Rgba::RAINBOW_ORANGE)
		return Rgba::RAINBOW_YELLOW;

	return Rgba::WHITE;
}

//-----------------------------------------------------------------------------------------------
Rgba GetRandomColorInRainbow()
{

	int d = GetRandomIntRange(0, 6);

	if (d == 0)
		return Rgba::RAINBOW_RED;
	if (d == 1)
		return Rgba::RAINBOW_ORANGE;
	if (d == 2)
		return Rgba::RAINBOW_YELLOW;
	if (d == 3)
		return Rgba::RAINBOW_GREEN;
	if (d == 4)
		return Rgba::RAINBOW_BLUE;
	if (d == 5)
		return Rgba::RAINBOW_INDIGO;
	if (d == 6)
		return Rgba::RAINBOW_VIOLET;

	return Rgba::WHITE;
}

//-----------------------------------------------------------------------------------------------
Rgba InterpolateRainbow(Rgba& currentColor, float percent)
{
	percent = ClampFloat(percent, 0.f, 1.f);

	Rgba nextColor = GetNextColorInRainbow(currentColor);

	return Interpolate(currentColor, nextColor, percent);
}
