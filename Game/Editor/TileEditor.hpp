#pragma once
#include "Game/Editor/EditorMode.hpp"
#include "Game/General/Map/Tile.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
class MapEditor;
class Vector2;
class MeshBuilder;

//====================================================================================
// Type Defs + Defines
//====================================================================================


//====================================================================================
// ENUMS
//====================================================================================
enum TileDrawModes
{
	TILE_DRAW_MODE_NORMAL,
	TILE_DRAW_MODE_FILL
};

//====================================================================================
// Structs
//====================================================================================


//====================================================================================
// Classes
//====================================================================================
class TileEditor : public EditorMode
{
	friend class MapEditor;

public:
	TileEditor( MapEditor* theMapEditor);
	~TileEditor();

public:
	virtual void Update() override;
	virtual void Render() const override;
	virtual void HandleInput() override;

	virtual void Enter() override {}
	virtual void Exit() override {}

private:
	void LeftClick();
	void RightClick();
	void SelectSpriteSheetTile(const Vector2& mousePos);
	void GenerateAllBounds();
	void CreateTilePlacementPreview();

private:
	void DrawModeNormal();
	void DrawModeFill();
	bool IsTileInsideSector(const IntVector2& min, const IntVector2& max, const IntVector2& pointToCheck);

private:
	void RenderUI() const;
	void RenderTilePlacementPreview() const;
	void RenderTileSelectHoverOutline() const;
	void RenderSpriteSheetButtons() const;

public:
	MapEditor*			m_mapEditor;

	TileSpriteInfo		m_selectedSpriteInfo;

private:
	Vector2					m_lastSelectedTilePosition;
	MeshBuilder*			m_tileSelectPreviewMB;
	std::vector<IntVector2>	m_tilesToChange;

	TileDrawModes			m_drawMode = TILE_DRAW_MODE_NORMAL;

private:
	AABB2		m_tileSelectBounds;
	AABB2		m_selectedTilePreviewBounds;
	AABB2		m_tilePreviewBounds;

private:
	AABB2		m_spriteSheetIndexBounds;
	AABB2		m_spriteSheetButtonBounds[4];
	
	int			m_selectedSpriteSheet = 0;
};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [9/29/2019]
//====================================================================================