#pragma once
#include "Game/Editor/EditorMode.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
class Map;
class Camera;
class TileEditor;
class CollisionEditor;

//====================================================================================
// Type Defs + Defines
//====================================================================================
constexpr float DEFAULT_ZOOMED_IN_LEVEL = .6f;
constexpr float MAX_ZOOMED_OUT_LEVEL = .95f; // anything higher than one breaks our lines?
constexpr float MIN_ZOOMED_IN_LEVEL = .2f;
constexpr float ZOOM_STEP_SIZE = .05f;

//====================================================================================
// ENUMS
//====================================================================================
enum MapEditorMode
{
	TILE_EDITOR_MODE,
	COLLISION_EDITOR_MODE,
	NUM_OF_MAP_EDITOR_MODE
};

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
	
	virtual void Enter() override {}
	virtual void Exit() override {}

public:
	void SetMode(MapEditorMode theMode);

private:
	void RenderUI() const;

private:
	void KeyboardMovement();

private:
	void GenerateAllBounds();

public:
	Map*		m_map;
	Camera*		m_camera;

public:
	MapEditorMode		m_currentMode = TILE_EDITOR_MODE;
	EditorMode*			m_mapEditorModes[NUM_OF_MAP_EDITOR_MODE];

private:
	AABB2		m_cameraBounds;
	AABB2		m_optionsBounds;

	float		m_currentZoom = DEFAULT_ZOOMED_IN_LEVEL;

	bool		m_showGrid = true;

	Vector2		m_cameraPosition = Vector2(0.f, 0.f);

public:
	friend class TileEditor;
	friend class CollisionEditor;
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