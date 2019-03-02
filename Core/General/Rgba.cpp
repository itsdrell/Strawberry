#include "Rgba.hpp"
#include "Engine/Math/MathUtils.hpp"

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
