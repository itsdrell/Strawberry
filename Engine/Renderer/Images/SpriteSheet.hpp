#pragma once
#include "Engine/Math/Vectors/IntVector2.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
class Texture;

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
class SpriteSheet
{
public:
	SpriteSheet(Texture* theTexture, int tilesWide, int tilesHigh);
	~SpriteSheet() {}

public:
	AABB2		GetTexCoordsForSpriteCoords(const IntVector2& spriteCoords) const;
	AABB2		GetTexCoordsForSpriteIndex(int spriteIndex) const;
	AABB2		GetTexCoordsForSpriteIndexAndDimensions(int spriteIndex, const IntVector2& dimensions) const;

	int			GetIndexFromSpriteCoords(const IntVector2& spriteCoords) const;
	IntVector2	GetCoordsFromSpriteIndex(int index) const;

public:
	Texture*		m_texture = nullptr;
	IntVector2		m_spriteLayout = IntVector2(0, 0);
	IntVector2		m_dimensions = IntVector2(0, 0);

};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [4/2/2019]
//====================================================================================