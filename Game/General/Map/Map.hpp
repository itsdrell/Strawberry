#pragma once
#include "Game/General/Map/Tile.hpp"
#include <vector>


//====================================================================================
// Forward Declare
//====================================================================================
class AABB2;
class Vector2;
class Mesh;

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
	~Map() {}

private:
	void InitializeMap();
	void WriteFileHeader();
	void CreateNewMap();
	void CreateTilesFromData();
	void GenerateTileMesh();

public:
	void SaveMap();
	bool LoadMap();

public:
	void Update();
	void Render() const;

private:
	void RenderTiles() const;
	void RenderGrid() const;

public:
	AABB2 GetBounds() const;
	void ChangeTileAtMousePos(const Vector2& mousePos, const TileSpriteInfo& spriteInfo);

public:
	unsigned int			m_totalAmountOfTiles;
	IntVector2				m_dimensions;
	Mesh*					m_tileMesh;

private:
	Tiles					m_tiles;
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