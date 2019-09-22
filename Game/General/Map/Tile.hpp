#pragma once
#include "Engine/Math/Vectors/IntVector2.hpp"

//====================================================================================
// Forward Declare
//====================================================================================


//====================================================================================
// Type Defs + Defines
//====================================================================================

// Which spritesheet (x) and what index (y)
typedef IntVector2 TileSpriteInfo;

#define DEFAULT_TILE_SPRITE_INFO IntVector2(-1,-1)

//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================


//====================================================================================
// Classes
//====================================================================================
class Tile
{
public:
	Tile() {}
	~Tile() {}

public:
	TileSpriteInfo	m_spriteInfo = DEFAULT_TILE_SPRITE_INFO;
	bool			m_hasCollision = false;
};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [9/21/2019]
//====================================================================================