#pragma once
#include "Game/General/Map/Tile.hpp"
#include <vector>


//====================================================================================
// Forward Declare
//====================================================================================
class AABB2;
class Vector2;
class Mesh;
class Tile;
class MeshBuilder;
class IntVector2;

//====================================================================================
// Type Defs + Defines
//====================================================================================
typedef std::vector<Tile> Tiles;

//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================


//====================================================================================
// Classes
//====================================================================================
class Map
{
public:
	Map(const IntVector2& dimensions);
	Map();
	~Map();

private:
	void InitializeMap();
	void WriteFileHeader();
	void CreateNewMap();
	void CreateTilesFromData();
	void UpdateTileMesh(int tileIndex, const Tile& changedTile);
	void GenerateTileMesh();

public:
	void SaveMap();
	bool LoadMap();
	Tile& GetTileByIndex(int index);
	Tile& GetTileByTilePos(const IntVector2& pos);

public:
	void Update();
	void Render(bool showGrid = true) const;

private:
	void RenderTiles() const;
	void RenderGrid() const;

public:
	AABB2 GetBounds() const;
	int GetTileIndexFromWorldPos(const Vector2& pos);
	void ChangeTileAtMousePos(const Vector2& mousePos, TileSpriteInfo& spriteInfo);
	void ChangeTileAtTilePos(const IntVector2& tilePos, TileSpriteInfo& spriteInfo);
	void ChangeTilesCollisionChannel(const Vector2& mousePos, Byte flagsToChange);

public:
	unsigned int			m_totalAmountOfTiles;
	IntVector2				m_dimensions = IntVector2(128, 128);
	Mesh*					m_tileMesh = nullptr;
	MeshBuilder*			m_tileBuilder;
	bool					m_mapMeshIsDirty = false;

private:
	Tiles					m_tiles;
	Tiles					m_viewableTiles;
	std::vector<uint16>		m_fileData;
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