#include "Map.hpp"
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"

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
	float tileSize = 16;

	for(uint yIndex = 0; yIndex < (uint) m_dimensions.y; yIndex++)
	{	
		for (uint xIndex = 0; xIndex < (uint) m_dimensions.x; xIndex++)
		{
			uint currentTileIndex = (m_dimensions.x * yIndex) + xIndex;
			Tile currentTile = m_tiles.at(currentTileIndex);
			
			AABB2 currentBounds = AABB2(currentPos.x, currentPos.y, currentPos.x + tileSize, currentPos.y + tileSize);
			if (currentTile.m_spriteInfo == DEFAULT_TILE_SPRITE_INFO)
			{
				r->DrawAABB2Outline(currentBounds, Rgba(0, 0, 255, 255));
			}
			else
			{

			}

			currentPos.x += tileSize;
		}

		currentPos.x = 0;
		currentPos.y += tileSize;
	}

}
