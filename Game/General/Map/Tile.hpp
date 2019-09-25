#pragma once
#include "Engine/Math/Vectors/IntVector2.hpp"
#include "Engine/Core/General/EngineCommon.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
struct TileSpriteInfo;

//====================================================================================
// Type Defs + Defines
//====================================================================================

#define DEFAULT_TILE_SPRITE_INFO = TileSpriteInfo(0x1111, 0x1111)

constexpr int TILE_SIZE = 16;
constexpr uint16 DEFAULT_TILE_SPRITE_INFO_FLAG = 0b1111'1111'1111'1111;

constexpr uint16 SPRITE_INDEX_MASK	= 0b0000'0000'1111'1111;
constexpr uint16 SPRITE_SHEET_MASK	= 0b0000'0111'0000'0000;
constexpr uint16 CHANNELS_MASK		= 0b1111'1000'0000'0000;

//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================
struct TileSpriteInfo
{
	TileSpriteInfo() {};
	TileSpriteInfo(uint16 data) { m_data = data; }

public:
	inline int GetSpriteIndex() const;
	inline int GetSpriteSheet() const;
	inline bool IsDefault() { return m_data == DEFAULT_TILE_SPRITE_INFO_FLAG; }

public:
	inline void SetSpriteIndex(int spriteIndex);
	inline void SetSpriteSheet(int spriteSheetNumber);

private:
	// bits are right to left, dummy
	// 8 bits are for the index,			0b0000'0000'1111'1111;
	// 3 bits for the spritesheet index		0b0000'0111'0000'0000;
	// 5 flags used for physic/channels		0b1111'1000'0000'0000;
	uint16		m_data = DEFAULT_TILE_SPRITE_INFO_FLAG;
};

//====================================================================================
// Classes
//====================================================================================
class Tile
{
public:
	Tile() {}
	~Tile() {}

public:
	TileSpriteInfo	m_spriteInfo = TileSpriteInfo();
};

//====================================================================================
// Inline
//====================================================================================
int TileSpriteInfo::GetSpriteIndex() const
{
	return (int)(m_data & (SPRITE_INDEX_MASK));
}

//-----------------------------------------------------------------------------------------------
int TileSpriteInfo::GetSpriteSheet() const
{
	return (int)((m_data & SPRITE_SHEET_MASK) >> 8);
}

//-----------------------------------------------------------------------------------------------
void TileSpriteInfo::SetSpriteIndex(int spriteIndex) 
{
	uint16 value = m_data & ~SPRITE_INDEX_MASK;
	m_data = value | ((uint16)(spriteIndex));
}

//-----------------------------------------------------------------------------------------------
void TileSpriteInfo::SetSpriteSheet(int spriteSheetNumber)
{
	uint16 value = m_data & ~SPRITE_SHEET_MASK;
	m_data = value | (((uint16)(spriteSheetNumber)) << 8);
}

//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [9/21/2019]
//====================================================================================