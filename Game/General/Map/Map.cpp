#include "Map.hpp"
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Engine/Renderer/Images/SpriteSheet.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Vectors/Vector2.hpp"

#
#include <stdio.h>

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
	if(!LoadMap())
		CreateNewMap();
}

//-----------------------------------------------------------------------------------------------
void Map::WriteFileHeader()
{
	// TODO 
	// for validation between versions, maybe dimensions as well
}

//-----------------------------------------------------------------------------------------------
void Map::SaveMap()
{
#ifndef EMSCRIPTEN_PORT
	String path = "Projects/" + g_currentProjectName + "/" + g_currentProjectName + ".mapdata";

	FILE* theFile;
	fopen_s(&theFile, path.c_str(), "wb");

	if (theFile == NULL)
		return;

	m_fileData.reserve(m_dimensions.x * m_dimensions.y);

	for (int i = 0; i < m_tiles.size(); i++)
	{
		Tile currentTile = m_tiles.at(i);

		int value;
		if (currentTile.m_spriteInfo.GetSpriteSheet() != 0)
			value = currentTile.m_spriteInfo.GetSpriteSheet() * currentTile.m_spriteInfo.GetSpriteIndex();
		else
			value = currentTile.m_spriteInfo.GetSpriteIndex();

		m_fileData.push_back((uint16)(value)); 
	}

	// write header
	WriteFileHeader();

	fwrite(m_fileData.data(), sizeof(uint16), m_fileData.size(), theFile);

	// terminate
	fclose(theFile);

#endif
}

//-----------------------------------------------------------------------------------------------
bool Map::LoadMap()
{
#ifndef EMSCRIPTEN_PORT
	String path = "Projects/" + g_currentProjectName + "/" + g_currentProjectName + ".mapdata";
	
	FILE* theFile;
	fopen_s(&theFile, path.c_str(), "rb");

	if (theFile == NULL)
		return false;

	// read the buffer
	int c; // note: int, not char, required to handle EOF
	while ((c = fgetc(theFile)) != EOF)
	{
		putchar(c);
		m_fileData.push_back((uint16) c);
	}

	// terminate
	fclose(theFile);

	// map tiles from data
	CreateTilesFromData();

	return true;

#endif

	// if somehow we got here in web, id rather return false and create a new map idk??
	return false;
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
void Map::CreateTilesFromData()
{
	m_tiles.reserve((uint)(((float) m_fileData.size()) * .5f));
	for (uint i = 0; i < m_fileData.size(); i += 2)
	{
		uint16 rightBits = m_fileData.at(i);
		uint16 leftBits = m_fileData.at(i + 1);

		uint16 result = leftBits << 8;
		result |= rightBits;

		TileSpriteInfo theInfo = TileSpriteInfo(result);
		m_tiles.push_back(theInfo);
	}
	
	m_fileData.clear();
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
			if (currentTile.m_spriteInfo.IsDefault())
			{
				r->DrawAABB2Outline(currentBounds, Rgba(0, 0, 255, 255));
			}
			else
			{
				AABB2 uvs = g_theSpriteSheet->GetTexCoordsForSpriteIndex(currentTile.m_spriteInfo.GetSpriteIndex());
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
	float width = (float)(TILE_SIZE * m_dimensions.x);
	float height = (float)(TILE_SIZE * m_dimensions.y);
	return AABB2(0.f, 0.f, width, height);
}

//-----------------------------------------------------------------------------------------------
void Map::ChangeTileAtMousePos(const Vector2& mousePos, const TileSpriteInfo& spriteInfo)
{
	AABB2 mapBounds = GetBounds();

	int tileX = (int)ClampFloat(RangeMapFloat(mousePos.x, mapBounds.mins.x, mapBounds.maxs.x, 0.f, (float)m_dimensions.x), 0.f, (float)(m_dimensions.x - 1));
	int tileY = (int)ClampFloat(RangeMapFloat(mousePos.y, mapBounds.mins.y, mapBounds.maxs.y, 0.f, (float)m_dimensions.y), 0.f, (float)(m_dimensions.y - 1));

	int index = (tileY * m_dimensions.x) + tileX;
	m_tiles.at(index).m_spriteInfo = spriteInfo;
}
