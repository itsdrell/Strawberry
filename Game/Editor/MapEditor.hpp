#pragma once
#include "Game/Editor/EditorMode.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Game/General/Map/Tile.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
class Map;
class Camera;

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
class MapEditor : public EditorMode
{
public:
	MapEditor();
	~MapEditor();

public:
	virtual void Update() override;
	virtual void Render() const override;
	virtual void HandleInput() override;

private:
	void RenderUI() const;

private:
	void LeftClick();
	void KeyboardMovement();
	void SelectSpriteSheetTile(const Vector2& mousePos);

private:
	void GenerateAllBounds();

public:
	Map*		m_map;
	Camera*		m_camera;

private:
	AABB2		m_cameraBounds;
	AABB2		m_tileSelectBounds;
	AABB2		m_optionsBounds;
	AABB2		m_selectedTilePreviewBounds;
	AABB2		m_tilePreviewBounds;

	Vector2		m_cameraPosition = Vector2(0.f, 0.f);

	TileSpriteInfo	m_selectedSpriteInfo;
};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [9/15/2019]
//====================================================================================