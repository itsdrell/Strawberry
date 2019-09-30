#pragma once
#include "Game/Editor/EditorMode.hpp"
#include "Game/General/Map/Tile.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
class MapEditor;
class Vector2;

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

private:
	void LeftClick();
	void RightClick();
	void SelectSpriteSheetTile(const Vector2& mousePos);
	void GenerateAllBounds();

private:
	void RenderUI() const;

public:
	MapEditor*			m_mapEditor;

	TileSpriteInfo		m_selectedSpriteInfo;

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