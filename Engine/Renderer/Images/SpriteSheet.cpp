#include "SpriteSheet.hpp"
#include "Engine/Renderer/Images/Texture.hpp"
#include "Engine/Math/MathUtils.hpp"

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

	// doing a precision fix
	float reallySmallNumber = .001f;
	//minx += reallySmallNumber;
	miny += reallySmallNumber;
	//maxx -= reallySmallNumber;
	maxy -= reallySmallNumber;


	// What this does is fix the weird texture flip!
	return AABB2(Vector2(minx, 1.f - maxy), Vector2(maxx, 1.f - miny));
}

//-----------------------------------------------------------------------------------------------
AABB2 SpriteSheet::GetTexCoordsForSpriteIndex(int spriteIndex) const
{
	int indexX = spriteIndex % m_spriteLayout.x;
	int indexY = (int)(spriteIndex / m_spriteLayout.x);

	return GetTexCoordsForSpriteCoords(IntVector2(indexX, indexY));
}

//-----------------------------------------------------------------------------------------------
AABB2 SpriteSheet::GetTexCoordsForSpriteIndexAndDimensions(int spriteIndex, const IntVector2& dimensions) const
{
	IntVector2 startingSpriteCoords = GetCoordsFromSpriteIndex(spriteIndex);
	IntVector2 endingSpriteCoords = startingSpriteCoords + (dimensions - IntVector2(1, 1));

	// drawing just one sprite case
	if (endingSpriteCoords == startingSpriteCoords)
		return GetTexCoordsForSpriteCoords(startingSpriteCoords);
	
	AABB2 startingUVs = GetTexCoordsForSpriteCoords(startingSpriteCoords);
	AABB2 endUVs = GetTexCoordsForSpriteCoords(endingSpriteCoords);

	// have to flip the y's since GetTexCoordsForSpriteCoords inverts the Y's
	return AABB2(startingUVs.mins.x, endUVs.mins.y, endUVs.maxs.x, startingUVs.maxs.y);
}

//-----------------------------------------------------------------------------------------------
int SpriteSheet::GetIndexFromSpriteCoords(const IntVector2 & spriteCoords) const
{
	// y * width + x
	int result = (spriteCoords.y * m_spriteLayout.x) + spriteCoords.x;
	return result;
}

//-----------------------------------------------------------------------------------------------
IntVector2 SpriteSheet::GetCoordsFromSpriteIndex(int index) const
{
	int indexX = index % m_spriteLayout.x;
	int indexY = (int)(index / m_spriteLayout.x);

	return IntVector2(indexX, indexY);
}

//-----------------------------------------------------------------------------------------------
int SpriteSheet::GetSpriteIndexFromPositionInBounds(const Vector2& pos, const AABB2& bounds)
{
	int tileX = (int)ClampFloat(RangeMapFloat(pos.x, bounds.mins.x, bounds.maxs.x, 0.f, (float) m_spriteLayout.x), 0.f, (float)(m_spriteLayout.x - 1));
	int tileY = (int)ClampFloat(RangeMapFloat(pos.y, bounds.mins.y, bounds.maxs.y, (float) m_spriteLayout.y, 0.f), 0.f, (float)(m_spriteLayout.y - 1));

	return (tileY * m_spriteLayout.x) + tileX;
}
