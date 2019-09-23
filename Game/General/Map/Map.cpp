#include "Map.hpp"
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Engine/Renderer/Images/SpriteSheet.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Vectors/Vector2.hpp"

//===============================================================================================
Map::Map(const IntVector2& dimensions)
{
	m_dimensions = dimensions;
	m_totalAmountOfTiles = m_dimensions.x * m_dimensions.y;
	InitializeMap();
}

//-----------------------------------------------------------------------------------------------
void Map::InitializeMap()
{
	// Later will load the map file
	CreateNewMap();
}

//-----------------------------------------------------------------------------------------------
void Map::CreateNewMap()
{
	for (uint i = 0; i < m_totalAmountOfTiles; i++)
	{
		m_tiles.push_back(Tile());
	}
}

//-----------------------------------------------------------------------------------------------
void Map::Update()
{
}

//-----------------------------------------------------------------------------------------------
void Map::Render() const
{
	Renderer* r = Renderer::GetInstance();

	Vector2 currentPos = Vector2(0.f, 0.f);

	for(uint yIndex = 0; yIndex < (uint) m_dimensions.y; yIndex++)
	{	
		for (uint xIndex = 0; xIndex < (uint) m_dimensions.x; xIndex++)
		{
			uint currentTileIndex = (m_dimensions.x * yIndex) + xIndex;
			Tile currentTile = m_tiles.at(currentTileIndex);
			
			AABB2 currentBounds = AABB2(currentPos.x, currentPos.y, currentPos.x + TILE_SIZE, currentPos.y + TILE_SIZE);
			if (currentTile.m_spriteInfo == DEFAULT_TILE_SPRITE_INFO)
			{
				r->DrawAABB2Outline(currentBounds, Rgba(0, 0, 255, 255));
			}
			else
			{
				AABB2 uvs = g_theSpriteSheet->GetTexCoordsForSpriteIndex(currentTile.m_spriteInfo.y);
				r->DrawTexturedAABB2(currentBounds, *g_theSpriteSheet->m_texture, uvs.mins, uvs.maxs, Rgba(255,255,255,255));
				
				// may need to do the outline as a batched job cause hot damn
				r->DrawAABB2Outline(currentBounds, Rgba(171, 183, 183, 150));
			}

			currentPos.x += TILE_SIZE;
		}

		currentPos.x = 0;
		currentPos.y += TILE_SIZE;
	}

	r->DrawAABB2Outline(GetBounds(), Rgba(0, 0, 0, 255));
}

//-----------------------------------------------------------------------------------------------
AABB2 Map::GetBounds() const
{
	// assuming the origin is at 0,0
	float width = TILE_SIZE * m_dimensions.x;
	float height = TILE_SIZE * m_dimensions.y;
	return AABB2(0.f, 0.f, width, height);
}

//-----------------------------------------------------------------------------------------------
void Map::ChangeTileAtMousePos(const Vector2& mousePos, const TileSpriteInfo& spriteInfo)
{
	AABB2 mapBounds = GetBounds();

	int tileX = ClampFloat(RangeMapFloat(mousePos.x, mapBounds.mins.x, mapBounds.maxs.x, 0, m_dimensions.x), 0, m_dimensions.x - 1);
	int tileY = ClampFloat(RangeMapFloat(mousePos.y, mapBounds.mins.y, mapBounds.maxs.y, 0, m_dimensions.y), 0, m_dimensions.y - 1);

	int index = (tileY * m_dimensions.x) + tileX;
	m_tiles.at(index).m_spriteInfo = spriteInfo;
}
