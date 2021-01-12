#pragma once
#include "Engine/Math/Vectors/IntVector2.hpp"
#include "Game/General/Map/Tile.hpp"
#include "Game/General/Map/Map.hpp"
#include "Engine/Math/Vectors/Vector2.hpp"
#include <vector>

//====================================================================================
// Forward Declare
//====================================================================================
class EditorMode;

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
class IEditorAction
{
public:
	IEditorAction() {}
	~IEditorAction() {}

	virtual void Undo() = 0;

public:
	Map*			m_map = nullptr;
	IntVector2		m_tilePos;
};

//-----------------------------------------------------------------------------------------------
class TileChangeAction : public IEditorAction
{
public:
	TileChangeAction(const TileSpriteInfo& previousInfo, const IntVector2& tilePos, Map* theMap);
	virtual void Undo() override;

public:
	TileSpriteInfo		m_previousInfo;
};

//-----------------------------------------------------------------------------------------------
class MultiTileChangeAction : public IEditorAction
{
public:
	MultiTileChangeAction(const std::vector<TileChangeAction*>& tilesToChange);
	MultiTileChangeAction(const std::vector<TileChangeAction*>& tilesToChange, const Vector2& lastPosition, EditorMode* editor);
	virtual void Undo() override;

public:
	std::vector<TileChangeAction*>	m_tilesToChange;

	// This is used for the line drawing so it will go back to drawing from 
	// the previous pos instead of the old pos
	EditorMode*						m_editor = nullptr;
	Vector2							m_lastDrawnPosition = Vector2();
};


//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [12/9/2020]
//====================================================================================