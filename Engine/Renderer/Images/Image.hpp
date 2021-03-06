#pragma once
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Math/Vectors/IntVector2.hpp"
#include "Engine/Core/General/Rgba.hpp"

//====================================================================================
// Forward Declare
//====================================================================================


//====================================================================================
// Type Defs + Defines
//====================================================================================


//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================


//====================================================================================
// Classes
//====================================================================================
class Image
{
public:
	Image();
	Image(const String& name, const IntVector2& dimension, const Rgba& color);
	~Image();

public:
	unsigned char* GetColorCharPointer() const;
	IntVector2 GetDimensions() const { return m_dimensions; }

private:
	IntVector2				m_dimensions;
	std::vector<Rgba>		m_colors;
	String					m_name;
};

//====================================================================================
// Standalone C Functions
//====================================================================================
void SaveImageAsPng(const Image& theImage, const String& filePath);

//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [1/31/2019]
//====================================================================================