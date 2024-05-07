#include "Map.hpp"
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Engine/Renderer/Images/SpriteSheet.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Vectors/Vector2.hpp"
#include "Engine/Renderer/Systems/MeshBuilder.hpp"
#include "Engine/Renderer/Systems/MeshBuilderStrawberry.hpp"
#include "Engine/Renderer/BuiltInShaders.hpp"
#include "Engine/Renderer/Components/Shader.hpp"
#include "Engine/Renderer/Images/Texture.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/RenderTypes.hpp"
#include "Engine/Core/Tools/ErrorWarningAssert.hpp"
#include "Engine/Core/Platform/File.hpp"

#include <iostream>
#include <stdio.h>
#include <fstream>

//===============================================================================================
Map::Map(const IntVector2& dimensions)
{
	m_dimensions = dimensions;
	m_totalAmountOfTiles = m_dimensions.x * m_dimensions.y;
	m_tileBuilder = new StrawberryMeshBuilder();
	m_tileMapShader = BuiltInShaders::CreateStrawberryShader();

	InitializeMap();
}

//-----------------------------------------------------------------------------------------------
Map::Map()
{
	m_totalAmountOfTiles = m_dimensions.x * m_dimensions.y;
	m_tileBuilder = new StrawberryMeshBuilder();
	m_tileMapShader = BuiltInShaders::CreateStrawberryShader();

	InitializeMap();
}

//-----------------------------------------------------------------------------------------------
Map::~Map()
{
	delete m_tileMesh;
	m_tileMesh = nullptr;

	delete m_tileBuilder;
	m_tileBuilder = nullptr;

	delete m_tileMapShader;
	m_tileMapShader = nullptr;
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
	String dataPath = "Projects/" + g_currentProjectName + "/Data/";
	String mapPath = dataPath + g_currentProjectName + ".mapdata";

	// make sure the directory exists
	CreateADirectory(dataPath.c_str());

	FILE* theFile;
	fopen_s(&theFile, mapPath.c_str(), "w");

	if (theFile == NULL)
		return;

	int amountOfTiles = m_dimensions.x * m_dimensions.y;
	m_fileData.clear();
	m_fileData.reserve(amountOfTiles);

	for (int i = 0; i < amountOfTiles; i++)
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
	String path = "Projects/" + g_currentProjectName + "/Data/" + g_currentProjectName + ".mapdata";
	
	std::ifstream infile(path.c_str());

	if (!infile.is_open())
	{
		PrintLog("ERROR: Unable to get map content from file: " + std::string(path.c_str()));
		return false;
	}

	char aChar;
	while (infile.get(aChar))
	{
		// We have to cast this to unsigned or the cast to uint16 will break
		unsigned char castChar = (unsigned char)aChar;
		
		m_fileData.push_back((uint16)castChar);
	}

	infile.close();

	// map tiles from data
	CreateTilesFromData();

	return true;
}

//-----------------------------------------------------------------------------------------------
Tile& Map::GetTileByIndex(int index)
{
	uint indexClamped = ClampInt(index, 0, ((int) m_tiles.size()) - 1);
	return m_tiles.at(indexClamped);
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
	TileSpriteInfo info = changedTile.m_spriteInfo;
	int spriteSheet = info.GetSpriteSheet();
	AABB2 uvs = g_allSpriteSheets[spriteSheet]->GetTexCoordsForSpriteIndex(changedTile.m_spriteInfo.GetSpriteIndex());

	int textureID = spriteSheet + 2;

	m_tileBuilder->ChangeAttributesForVertexIndex(tileIndex,		uvs.mins, textureID, Rgba::WHITE);
	m_tileBuilder->ChangeAttributesForVertexIndex(tileIndex + 1,	Vector2(uvs.maxs.x, uvs.mins.y), textureID, Rgba::WHITE);
	m_tileBuilder->ChangeAttributesForVertexIndex(tileIndex + 2,	Vector2(uvs.mins.x, uvs.maxs.y), textureID, Rgba::WHITE);
	m_tileBuilder->ChangeAttributesForVertexIndex(tileIndex + 3,	uvs.maxs, textureID, Rgba::WHITE);

	m_mapMeshIsDirty = true;
}

//-----------------------------------------------------------------------------------------------
void Map::GenerateTileMesh()
{
	PrintLog("Started Generate Tile Mesh");
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
				int spriteSheet = currentTile.m_spriteInfo.GetSpriteSheet();
				int spriteIndex = currentTile.m_spriteInfo.GetSpriteIndex();
				AABB2 uvs = g_allSpriteSheets[spriteSheet]->GetTexCoordsForSpriteIndex(spriteIndex);
				m_tileBuilder->AppendTexturedAABB2(currentBounds, uvs, spriteSheet + 2, Rgba::WHITE);
			}
			else
			{
				// We need to make a placeholder vertex that we will later change
				// so we use one has UVs of 0 to skip the expensive check, and set the color to black
				// so the map looks empty #hack
				m_tileBuilder->AppendTexturedAABB2(currentBounds, AABB2(0.f, 0.f, 0.f, 0.f), 0, Rgba(0, 0, 0, 50));
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
	m_tileMesh = m_tileBuilder->CreateMesh(false);

	PrintLog("finished");
}

//-----------------------------------------------------------------------------------------------
Tile& Map::GetTileByTilePos(const IntVector2& pos)
{
	Vector2 worldPos = Vector2(pos.x * TILE_SIZE, pos.y * TILE_SIZE);
	
	return GetTileByIndex(GetTileIndexFromWorldPos(worldPos));
}

//-----------------------------------------------------------------------------------------------
void Map::Update()
{
	if (m_mapMeshIsDirty)
	{
		if (m_tileMesh)
			delete m_tileMesh;

		m_tileMesh = m_tileBuilder->CreateMesh(false);

		m_mapMeshIsDirty = false;
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

	//-----------------------------------------------------------------------------------------------
	r->SetShader(m_tileMapShader);

	r->SetSamplerUniform("gDefaultTexDiffuse", 0);
	r->SetSamplerUniform("gDefaultFontTexDiffuse", 1);

	r->SetCurrentTexture(0, r->m_defaultTexture);
	r->SetCurrentTexture(1, r->m_defaultTexture); // just sticking something in the font slot

	// if you want more, you'll need to add them to the shader sampler
	for (uint i = 0; i < MAX_AMOUNT_OF_SPRITE_SHEETS; i++)
	{
		String name = "gSpriteSheetTexDiffuse_" + std::to_string(i);
		int slot = i + 2;

		r->SetSamplerUniform(name, slot);
		r->SetCurrentTexture(slot, g_allSpriteSheets[i]->m_texture);
	}

	r->BindCameraToShader(*r->m_currentCamera);

	r->DrawMesh(m_tileMesh, false);

	r->SetCurrentTexture();
	r->SetShader();
	r->BindCameraToShader(*r->m_currentCamera);
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
	// my tiles are only positive
	int tileX = (int)(pos.x / TILE_SIZE_FLOAT);
	int tileY = (int)(pos.y / TILE_SIZE_FLOAT);

	return (tileY * m_dimensions.x) + tileX;
}

//-----------------------------------------------------------------------------------------------
IntVector2 Map::GetTilePosFromWorldPos(const Vector2& pos)
{
	int tileX = (int)(pos.x / TILE_SIZE_FLOAT);
	int tileY = (int)(pos.y / TILE_SIZE_FLOAT);

	return IntVector2(tileX, tileY);
}

//-----------------------------------------------------------------------------------------------
TileSpriteInfo Map::GetTileInfoFromWorldPos(const Vector2& pos)
{
	int index = GetTileIndexFromWorldPos(pos);
	Tile& tile = GetTileByIndex(index);

	return tile.m_spriteInfo;
}

//-----------------------------------------------------------------------------------------------
void Map::ChangeTileAtMousePos(const Vector2& mousePos, TileSpriteInfo& spriteInfo)
{
	int index = GetTileIndexFromWorldPos(mousePos);
	Tile& theTile = m_tiles.at(index);

	// Hack : spriteInfo was missing collision so we make sure we have it in the info
	// so it doesn't get overriden when we change sprites
	int channelFlags = theTile.m_spriteInfo.GetCollisionChannelValue();
	spriteInfo.SetChannelBits((Byte)channelFlags);

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
