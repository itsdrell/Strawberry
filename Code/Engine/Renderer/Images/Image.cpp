#include "Image.hpp"

//===============================================================================================
Image::Image()
{
}

//-----------------------------------------------------------------------------------------------
Image::Image(const String & name, const IntVector2 & dimension, const Rgba & color)
{
	// this is creating an image with colors I give it (so no texture)
	m_dimensions = dimension;
	m_name = name;

	int amount = dimension.x * dimension.y;

	for(int i = 0; i < amount; i++)
	{
		m_colors.push_back(color);
	}
}

//-----------------------------------------------------------------------------------------------
Image::~Image()
{

}

//-----------------------------------------------------------------------------------------------
unsigned char * Image::GetColorCharPointer() const
{
	return (unsigned char*) m_colors.data();
}
