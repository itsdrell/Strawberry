#include "Rgba.hpp"
#include "Engine/Math/MathUtils.hpp"

//===============================================================================================
std::map<String, Rgba> Rgba::s_defaultColors;

Rgba Rgba::WHITE = Rgba(255, 255, 255, 255);

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
