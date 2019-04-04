#include "SpriteSheet.hpp"
#include "Engine/Renderer/Images/Texture.hpp"

//===============================================================================================
SpriteSheet::SpriteSheet(Texture * theTexture, int tilesWide, int tilesHigh)
{
	m_texture = theTexture;
	m_spriteLayout = IntVector2(tilesWide, tilesHigh);
	m_dimensions = theTexture->GetDimensions();
}

AABB2 SpriteSheet::GetTexCoordsForSpriteCoords(const IntVector2 & spriteCoords) const
{
	// Create min 
	float minIndexX = (float)(spriteCoords.x);
	float minIndexY = (float)(spriteCoords.y);

	// Get fraction over
	float fractionX = minIndexX / (float)m_spriteLayout.x;
	float fractionY = minIndexY / (float)m_spriteLayout.y;

	// Get position
	float minx = fractionX;
	float miny = fractionY;

	// Create Max
	float maxIndexX = (float)(spriteCoords.x + 1);
	float maxIndexY = (float)(spriteCoords.y + 1);

	fractionX = (maxIndexX / m_spriteLayout.x);
	fractionY = (maxIndexY / m_spriteLayout.y);

	float maxx = fractionX;
	float maxy = fractionY;

	// What this does is fix the weird texture flip!
	return AABB2(Vector2(minx, 1.f - maxy), Vector2(maxx, 1.f - miny));
}

AABB2 SpriteSheet::GetTexCoordsForSpriteIndex(int spriteIndex) const
{
	int indexX = spriteIndex % m_spriteLayout.x;
	int indexY = (int)(spriteIndex / m_spriteLayout.x);

	return GetTexCoordsForSpriteCoords(IntVector2(indexX, indexY));
}

int SpriteSheet::GetIndexFromSpriteCoords(const IntVector2 & spriteCoords) const
{
	// y * width + x
	int result = (spriteCoords.y * m_spriteLayout.x) + spriteCoords.x;
	return result;
}

IntVector2 SpriteSheet::GetCoordsFromSpriteIndex(int index) const
{
	int indexX = index % m_spriteLayout.x;
	int indexY = (int)(index / m_spriteLayout.x);

	return IntVector2(indexX, indexY);
}