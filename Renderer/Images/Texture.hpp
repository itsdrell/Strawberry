#pragma once
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/RenderTypes.hpp"
#include "Engine/Math/Vectors/IntVector2.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
class Image;

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
class Texture
{
	friend class Renderer; 

private:
	Texture();
	Texture(const String& filePath, bool flip = true);

	void PopulateFromData( unsigned char* imageData, const IntVector2& texelSize, int numComponents) ;
	bool CreateRenderTarget( int width, int height, eTextureFormat format );
	
	Texture* CreateFromImage(const Image& imageToCreateFrom);

public:
	IntVector2 GetDimensions() { return m_dimensions; } 
	uint GetID() const { return m_textureID; }

private:
	uint				m_textureID;
	IntVector2			m_dimensions;
	eTextureFormat		m_format;

	unsigned char*		m_data;
};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [1/31/2019]
//====================================================================================