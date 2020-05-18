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
	void RenderUI() const;
	void RenderTilePlacementPreview() const;
	void RenderTileSelectHoverOutline() const;

public:
	MapEditor*			m_mapEditor;

	TileSpriteInfo		m_selectedSpriteInfo;

private:
	Vector2					m_lastSelectedTilePosition;
	MeshBuilder*			m_tileSelectPreviewMB;
	std::vector<IntVector2>	m_tilesToChange;

private:
	AABB2		m_tileSelectBounds;
	AABB2		m_selectedTilePreviewBounds;
	AABB2		m_tilePreviewBounds;
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