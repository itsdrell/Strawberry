#include "Map.hpp"
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Engine/Renderer/Images/SpriteSheet.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Vectors/Vector2.hpp"
#include "Engine/Renderer/Systems/MeshBuilder.hpp"

#include <stdio.h>
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/RenderTypes.hpp"
#include "Engine/Core/Tools/ErrorWarningAssert.hpp"

//===============================================================================================
Map::Map(const IntVector2& dimensions)
{
	m_dimensions = dimensions;
	m_totalAmountOfTiles = m_dimensions.x * m_dimensions.y;
	m_tileBuilder = new MeshBuilder();
	InitializeMap();
}

//-----------------------------------------------------------------------------------------------
Map::Map()
{
	m_totalAmountOfTiles = m_dimensions.x * m_dimensions.y;
	m_tileBuilder = new MeshBuilder();
	InitializeMap();
}

//-----------------------------------------------------------------------------------------------
Map::~Map()
{
	delete m_tileMesh;
	m_tileMesh = nullptr;

	delete m_tileBuilder;
	m_tileBuilder = nullptr;
}

//-----------------------------------------------------------------------------------------------
void Map::InitializeMap()
{
	if(!LoadMap())
		CreateNewMap();

	GenerateTileMesh();
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

		m_fileData.push_back((currentTile.m_spriteInfo.GetData()));
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
void Map::UpdateTileMesh(int tileIndex, const Tile& changedTile)
{
	AABB2 uvs = g_theSpriteSheet->GetTexCoordsForSpriteIndex(changedTile.m_spriteInfo.GetSpriteIndex());

	// just editing the UVs of the meshbuilder for performance reasons!
	m_tileBuilder->ChangeUVOfVertexAtPosition(tileIndex, uvs.mins);
	m_tileBuilder->ChangeUVOfVertexAtPosition(tileIndex + 1, Vector2(uvs.maxs.x, uvs.mins.y));
	m_tileBuilder->ChangeUVOfVertexAtPosition(tileIndex + 2, Vector2(uvs.mins.x, uvs.maxs.y));
	m_tileBuilder->ChangeUVOfVertexAtPosition(tileIndex + 3, uvs.maxs);

	m_tileBuilder->ChangeColorOfVertexAtPosition(tileIndex, Rgba::WHITE);
	m_tileBuilder->ChangeColorOfVertexAtPosition(tileIndex + 1, Rgba::WHITE);
	m_tileBuilder->ChangeColorOfVertexAtPosition(tileIndex + 2, Rgba::WHITE);
	m_tileBuilder->ChangeColorOfVertexAtPosition(tileIndex + 3, Rgba::WHITE);

	m_mapMeshIsDirty = true;
}

//-----------------------------------------------------------------------------------------------
void Map::GenerateTileMesh()
{
	PrintLog("started Generate Tile Mesh");
	Vector2 currentPos = Vector2(0.f, 0.f);
	
	PrintLog("reserving");
	// 65536 verts, ind 98304 (trying to save memory)
	m_tileBuilder->m_vertices.reserve(65536);
	m_tileBuilder->m_indices.reserve(98304);

	PrintLog("starting for loop");
	for (uint yIndex = 0; yIndex < (uint)m_dimensions.y; yIndex++)
	{
		for (uint xIndex = 0; xIndex < (uint)m_dimensions.x; xIndex++)
		{
			uint currentTileIndex = (m_dimensions.x * yIndex) + xIndex;
			Tile currentTile = m_tiles.at(currentTileIndex);

			AABB2 currentBounds = AABB2(currentPos.x, currentPos.y, currentPos.x + TILE_SIZE, currentPos.y + TILE_SIZE);
			if (!currentTile.m_spriteInfo.IsDefault())
			{
				AABB2 uvs = g_theSpriteSheet->GetTexCoordsForSpriteIndex(currentTile.m_spriteInfo.GetSpriteIndex());
				m_tileBuilder->Add2DPlane(currentBounds, uvs, Rgba::WHITE);
			}
			else
			{
				// We need to make a placeholder vertex that we will later change
				// so we use one has UVs of 0 to skip the expensive check, and set the color to black
				// so the map looks empty #hack
				m_tileBuilder->Add2DPlane(currentBounds, AABB2(0.f,0.f,0.f,0.f), Rgba(0,255,0,50));
			}

			currentPos.x += TILE_SIZE;
		}

		currentPos.x = 0;
		currentPos.y += TILE_SIZE;
	}

	PrintLog("finished loop");
	if (m_tileMesh != nullptr)
	{
		PrintLog("deleting mesh");
		delete m_tileMesh;
	}
	PrintLog("creating mesh");
	m_tileMesh = m_tileBuilder->CreateMesh<Vertex3D_PCU>(false);

	PrintLog("finished");
}

//-----------------------------------------------------------------------------------------------
Tile& Map::GetTileByTilePos(const IntVector2& pos)
{
	return GetTileByIndex(GetTileIndexFromWorldPos(pos.ToVector2()));
}

//-----------------------------------------------------------------------------------------------
void Map::Update()
{
	if (m_mapMeshIsDirty)
	{
		if (m_tileMesh)
			delete m_tileMesh;

		m_tileMesh = m_tileBuilder->CreateMesh<Vertex3D_PCU>(false);
	}
}

//-----------------------------------------------------------------------------------------------
void Map::Render(bool showGrid) const
{
	RenderTiles();

	if(showGrid)
		RenderGrid();
}

//-----------------------------------------------------------------------------------------------
void Map::RenderTiles() const
{
	if (m_tileMesh == nullptr)
		return;
	
	Renderer* r = Renderer::GetInstance();
	r->SetLineWidth(2.f);
	r->DrawAABB2Outline(GetBounds(), Rgba::WHITE);

	r->SetCurrentTexture(0, g_theSpriteSheet->m_texture);
	r->DrawMesh(m_tileMesh, false);
}

//-----------------------------------------------------------------------------------------------
void Map::RenderGrid() const
{
	// TODO: Make this a mesh pls
	Renderer* r = Renderer::GetInstance();

	Rgba gridColor = Rgba(171, 183, 183, 150);
	AABB2 mapBounds = GetBounds();
	for (uint i = 1; i < (uint)m_dimensions.x; i++)
	{
		float xpos = RangeMapFloat((float)i, 0.f, (float) m_dimensions.x, mapBounds.mins.x, mapBounds.maxs.x);
		float ypos = RangeMapFloat((float)i, 0.f, (float) m_dimensions.y, mapBounds.mins.y, mapBounds.maxs.y);

		Vector2 verticalStart = Vector2(xpos, mapBounds.maxs.y);
		Vector2 verticalEnd = Vector2(xpos, mapBounds.mins.y);

		Vector2 horizontalStart = Vector2(mapBounds.mins.x, ypos);
		Vector2 horizontalEnd = Vector2(mapBounds.maxs.x, ypos);

		// outline of cells
		if (GetFractionOf((float)i / 16.f) == 0)
			r->SetLineWidth(6.f);
		else if (GetFractionOf((float)i / 8.f) == 0)
			r->SetLineWidth(4.f);
		else
			r->SetLineWidth(2.f);

		r->DrawLine2D(verticalStart, verticalEnd, gridColor);
		r->DrawLine2D(horizontalStart, horizontalEnd, gridColor);
	}
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
int Map::GetTileIndexFromWorldPos(const Vector2& pos)
{
	AABB2 mapBounds = GetBounds();

	int tileX = (int)ClampFloat(RangeMapFloat(pos.x, mapBounds.mins.x, mapBounds.maxs.x, 0.f, (float)m_dimensions.x), 0.f, (float)(m_dimensions.x - 1));
	int tileY = (int)ClampFloat(RangeMapFloat(pos.y, mapBounds.mins.y, mapBounds.maxs.y, 0.f, (float)m_dimensions.y), 0.f, (float)(m_dimensions.y - 1));

	return (tileY * m_dimensions.x) + tileX;
}

//-----------------------------------------------------------------------------------------------
void Map::ChangeTileAtMousePos(const Vector2& mousePos, TileSpriteInfo& spriteInfo)
{
	int index = GetTileIndexFromWorldPos(mousePos);
	Tile& theTile = m_tiles.at(index);

	// Hack : spriteInfo was missing collision so we make sure we have it in the info
	// so it doesn't get overriden when we change sprites
	int channelFlags = theTile.m_spriteInfo.GetCollisionChannelValue();
	spriteInfo.SetChannelBits(channelFlags);

	theTile.m_spriteInfo = spriteInfo;
	UpdateTileMesh(index * 4, theTile);
}

//-----------------------------------------------------------------------------------------------
void Map::ChangeTileAtTilePos(const IntVector2& tilePos, TileSpriteInfo& spriteInfo)
{
	ChangeTileAtMousePos(Vector2(tilePos.x * TILE_SIZE, tilePos.y * TILE_SIZE), spriteInfo);
}

//-----------------------------------------------------------------------------------------------
void Map::ChangeTilesCollisionChannel(const Vector2& mousePos, Byte flagsToChange)
{
	int index = GetTileIndexFromWorldPos(mousePos);
	m_tiles.at(index).m_spriteInfo.SetChannelBits(flagsToChange);
}
