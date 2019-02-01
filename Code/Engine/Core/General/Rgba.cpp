#include "Rgba.hpp"

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
